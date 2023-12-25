#include <WiFi.h>
#include <EEPROM.h>
#include <Arduino.h>
#include <ESPDateTime.h>
#include <FirebaseESP32.h>

#include "secrets.h"
#include "constants.h"
#include "led_indicators.h"

String readStringFromFlash(int);
void writeStringToFlash(const char *, int);
void setupDateTime();
void connecToFirebase();
void streamCallback(MultiPathStreamData);
void streamTimeoutCallback(bool);
void switchState(int, String);

char ssid[20];
char pass[20];

unsigned long resetMillis;

// Firebase
FirebaseAuth auth;
FirebaseConfig config;
FirebaseData fbdo;
FirebaseData stream;

unsigned long dataMillis = 0;

unsigned long previousMillis = 0;
unsigned long interval = 30000;

String parentPath = "/myHome/switches";
String childPath[5] = {"/switch1", "/switch2", "/switch3", "/switch4", "/switch5"};

void setup()
{
  Serial.begin(115200);

  pinMode(WiFi_rst, INPUT);

  pinMode(PIN_R, OUTPUT);
  pinMode(PIN_G, OUTPUT);
  pinMode(PIN_Y, OUTPUT);

  pinMode(PIN1, OUTPUT);
  pinMode(PIN2, OUTPUT);
  pinMode(PIN3, OUTPUT);
  pinMode(PIN4, OUTPUT);
  pinMode(PIN5, OUTPUT);
  pinMode(PIN6, OUTPUT);
  pinMode(PIN7, OUTPUT);
  pinMode(PIN8, OUTPUT);

  setupStartLED();

  if (!EEPROM.begin(EEPROM_SIZE))
  {
    Serial.println("Failed to initialize EEPROM!");
    delay(1000);
    return;
  }
  Serial.println("EEPROM initialized!");
  Serial.println("Reading ssid and password from EEPROM...");

  readStringFromFlash(0).toCharArray(ssid, 20);
  readStringFromFlash(20).toCharArray(pass, 20);

  WiFi.begin(ssid, pass);
  delay(3500);

  if (WiFi.status() != WL_CONNECTED)
  {
    WiFi.mode(WIFI_AP_STA);
    WiFi.beginSmartConfig();
    Serial.println("Waiting for SmartConfig. If it does not connect, try with different credentials.");
    while (!WiFi.smartConfigDone())
    {
      setupSmartConfigLED();
      Serial.print("*");
      delay(500);
    }
    Serial.println();
    Serial.println("SmartConfig done!");
    Serial.println("Connecting to WiFi...");
    while (WiFi.status() != WL_CONNECTED)
    {
      setupWifiLED();
      Serial.print(".");
      delay(500);
    }
    Serial.println("");
    wifiConnectedLED();
    Serial.println("WiFi connected!");
    // Serial.println("IP address: ");
    // Serial.println(WiFi.localIP());

    WiFi.SSID().toCharArray(ssid, 20);
    WiFi.psk().toCharArray(pass, 20);

    Serial.println("Storing ssid and password to EEPROM...");
    writeStringToFlash(ssid, 0);
    writeStringToFlash(pass, 20);
  }
  else
  {
    Serial.println("WiFi Connected!");
  }
  setupDateTime();
  connecToFirebase();

  // Stream data updates
  if (!Firebase.beginMultiPathStream(stream, parentPath))
    Serial.printf("sream begin error, %s\n\n", stream.errorReason().c_str());

  Firebase.setMultiPathStreamCallback(stream, streamCallback, streamTimeoutCallback);

  setupEndLED();

  Serial.println("Setup Complete!");
  Serial.println("------------------------------------");
}

void loop()
{
  // Reseting the WiFi credentials
  unsigned long currentMillis = millis();
  resetMillis = millis();
  while (digitalRead(WiFi_rst) == ON)
  {
    ;
    if (millis() - resetMillis >= 3000)
    {
      Serial.println("Reseting the WiFi credentials");
      for (int i = 0; i < EEPROM_SIZE; i++)
      {
        writeStringToFlash("", i);
      }
      EEPROM.end();
      Serial.println("Wifi credentials erased");
      Serial.println("Restarting the ESP");
      delay(500);
      ESP.restart();
    }
  }

  // Check WiFi connection status
  if (WiFi.status() != WL_CONNECTED)
  {
    wifiErrorLED();
    if (currentMillis - previousMillis >= interval)
    {
      Serial.print(millis());
      Serial.println("Reconnecting to WiFi...");
      WiFi.disconnect();
      WiFi.reconnect();
      previousMillis = currentMillis;
    }
  }
  else
  {
    // WiFi connected
    if (Firebase.ready())
    {
      dbConnectedLED();
      if (millis() - dataMillis > 10000)
      {
        dataMillis = millis();
        if (!Firebase.set(fbdo, "/myHome/deviceStats/dateTime", DateTime.toString().c_str()))
        {
          Serial.println(fbdo.errorReason());
        }
      }
    }
    else
    {
      dbErrorLED();
      Serial.println(fbdo.errorReason());
    }
    if (!stream.httpConnected())
    {
      // Server was disconnected!
      digitalWrite(PIN_R, HIGH);
      Serial.printf("Stream disconnected: %s\n", stream.errorReason());
    }
  }
}

void writeStringToFlash(const char *toStore, int startAddr)
{
  int i = 0;
  for (; i < LENGTH(toStore); i++)
  {
    EEPROM.write(startAddr + i, toStore[i]);
  }
  EEPROM.write(startAddr + i, '\0');
  EEPROM.commit();
}

String readStringFromFlash(int startAddr)
{
  char in[128];
  int i = 0;
  for (; i < 128; i++)
  {
    in[i] = EEPROM.read(startAddr + i);
  }
  return String(in);
}

void setupDateTime()
{
  DateTime.setServer("asia.pool.ntp.org");
  DateTime.setTimeZone("IST-5:30");
  DateTime.begin();
  if (!DateTime.isTimeValid())
  {
    Serial.println("Failed to get time from server.");
  }
}

void connecToFirebase()
{

  // Assign the project host and api key
  config.database_url = FIREBASE_DB_HOST;
  config.api_key = FIREBASE_WEB_API_KEY;

  // Assign the user sign in credentials
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  // Initialize the library with the Firebase authen and config.
  Firebase.begin(&config, &auth);

  Firebase.setMaxRetry(fbdo, 3);

  Firebase.setMaxErrorQueue(fbdo, 30);

  stream.keepAlive(5, 5, 1);

  // Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  Firebase.setReadTimeout(fbdo, 1000 * 60);
  Firebase.setwriteSizeLimit(fbdo, "small");
  Serial.println("------------------------------------");
  Serial.println("Firebase Connected...");
}

void streamCallback(MultiPathStreamData stream)
{
  size_t numChild = sizeof(childPath) / sizeof(childPath[0]);

  for (size_t i = 0; i < numChild; i++)
  {

    if (stream.get(childPath[i]))
    {
      switch (i)
      {
      case 0:
        switchState(PIN1, String(stream.value));
        break;
      case 1:
        switchState(PIN2, String(stream.value));
        break;
      case 2:
        switchState(PIN3, String(stream.value));
        break;
      case 3:
        switchState(PIN4, String(stream.value));
        break;
      case 4:
        switchState(PIN8, String(stream.value));
        break;

      default:
        break;
      }

      // Serial.printf("index: %d, path: %s, event: %s, type: %s,  s, stream.value.toInt(:%s", i, stream.dataPath.c_str(), stream.eventType.c_str(), stream.type.c_str(), streamc_str, stream.value.toInt().), i < numChild - 1 ? "\n" : "");
    }
  }
}

void streamTimeoutCallback(bool timeout)
{
  if (timeout)
    Serial.println("stream timed out, resuming...\n");

  if (!stream.httpConnected())
    Serial.printf("error code: %d, reason: %s\n\n", stream.httpCode(), stream.errorReason().c_str());
}

void switchState(int switchPin, String switchStats)
{
  if (switchStats.equals("true"))
  {
    digitalWrite(switchPin, ON);
  }
  else
  {
    digitalWrite(switchPin, OFF);
  }
  String isOn = switchStats.equals("true") ? "On" : "Off";
  String sName = switchPin == 16 ? "Switch1" : switchPin == 17 ? "Switch2"
                                           : switchPin == 18   ? "Switch3"
                                           : switchPin == 19   ? "Switch4"
                                           : switchPin == 27   ? "Switch5"
                                                               : "UnDefined";
  // Serial.println(sName +"\t:-\t" + isOn);
  Serial.printf("%s\t:-\t%s\n", sName.c_str(), isOn.c_str());
}
