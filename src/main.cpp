#include <Arduino.h>
#include <SmartHomeCore.h>
#include <Adafruit_NeoPixel.h>

using namespace shCore;

float brtn = 1000;
uint8_t ledPin = D7;
uint8_t stripLength = 64;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(stripLength, ledPin, NEO_RGB + NEO_KHZ800);

// void display(int row, int column, int green, int red, int blue)
// {
//   if (row == 0)
//   {
//     strip.setPixelColor(column, green, red, blue);
//   }
//   if ((row % 2) == 0)
//   {
//     strip.setPixelColor(8 * row + column, green, red, blue);
//   }
//   else
//   {
//     strip.setPixelColor(8 * row + (7 - column), green, red, blue);
//   }
// }

void changeColor()
{
  Serial.println("changeUrl");
  int w = 0;
  int r = getFromServer("r").toInt();
  int g = getFromServer("g").toInt();
  int b = getFromServer("b").toInt();
  uint32_t c = (uint32_t)b | ((uint32_t)r << 8) | ((uint32_t)g << 16) | ((uint32_t)w << 24);
  Serial.println(c);
  strip.clear();
  strip.show();
  strip.fill(c, 0, stripLength);
  strip.show();
  sendToServer(200, "text/plain", "ok");
}

void setup(void)
{
  setSSIDwifiAP("✅ wifi-led-panel ✅");
  setPassWiFiAP("3866210000");
  registrateEvent("/change-color", changeColor);
  Serial.begin(9600);
  strip.begin();
  strip.clear();
  coreInit();
}

// int micVolume()
// {
//   int mic = analogRead(A0);
//   int minMic = 500;
//   if (mic < minMic)
//     mic = minMic;
//   return map(mic, minMic, 1024, 0, 7);
// }

// void displayPick(int value)
// {
//   strip.clear();
//   for (int i = 0; i <= value; i++)
//   {
//     display(7 - i, 1, 10, 100, 10);
//   }
//   strip.show();
// }

void loop(void)
{
  coreHandle();
  delay(1);
}
