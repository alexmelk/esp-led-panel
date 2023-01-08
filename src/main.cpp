#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#include <SmartHomeCore.h>

#include "ArduinoJson.h"
#include "AsyncJson.h"

float brtn = 1000;
uint8_t ledPin = D7;
uint8_t stripLength = 64;
Adafruit_NeoPixel strip =
    Adafruit_NeoPixel(stripLength, ledPin, NEO_RGB + NEO_KHZ800);
SmartHomeCore core = SmartHomeCore("✅ wifi-led-panel ✅", "3866210000");

void setup(void) {
  Serial.begin(9600);
  strip.begin();
  strip.clear();
  core.begin();
  core.server.on("/api/color", HTTP_POST, [](AsyncWebServerRequest *request) {
    Serial.println("changeUrl");
    int w = 0;
    int r = request->arg("r").toInt();
    int g = request->arg("g").toInt();
    int b = request->arg("b").toInt();
    uint32_t c = (uint32_t)b | ((uint32_t)r << 8) | ((uint32_t)g << 16) |
                 ((uint32_t)w << 24);
    Serial.println(c);
    strip.clear();
    strip.show();
    strip.fill(c, 0, stripLength);
    strip.show();
    request->send(200);
  });
}

void loop(void) {}
