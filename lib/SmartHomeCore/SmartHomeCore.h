#pragma once
#include <Arduino.h>
// for wifi
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include <WiFiClient.h>

#include <ArduinoJson.hpp>

class SmartHomeCore {
 public:
  int serialSpeed = 115200;
  int serialTimeOut = 10;
  int port = 80;
  String ssid = "SmartHomeCore";
  String pass = "123123123";
  int wifiChannel = 8;
  int maxConnection = 1;
  int hiddenWifi = 0;
  int jsonLength = 2048;
  IPAddress apIP = IPAddress(77, 77, 77, 77);
  IPAddress subnet = IPAddress(255, 255, 255, 0);
  DynamicJsonDocument json = DynamicJsonDocument(jsonLength);
  AsyncWebServer server = AsyncWebServer(port);

  SmartHomeCore(String ssid, String pass);
  SmartHomeCore(void);
  void begin();

 private:
  String sendWifiList(void);
  void blink(int num, int delayMs);
  void clearAll();
  File openFile(String Filename);
  void createOrErase(String Filename, String Text);
  void onRequest(AsyncWebServerRequest *request);
  void initSerial();
  void initFS();
  void initPins();
  void initWiFi();
  void initServer(int port);

  void endpoints();
};
