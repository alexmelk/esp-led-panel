#include <Arduino.h>
#include <SmartHomeCore.h>
#include <Adafruit_NeoPixel.h>

using namespace shCore;

float brtn = 1000;
uint8_t ledPin = D7;
uint8_t stripLength = 64;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(stripLength, ledPin, NEO_RGB + NEO_KHZ800);

void display(int row, int column, int green, int red, int blue)
{
  if (row == 0)
  {
    strip.setPixelColor(column, green, red, blue);
  }
  if ((row % 2) == 0)
  {
    strip.setPixelColor(8 * row + column, green, red, blue);
  }
  else
  {
    strip.setPixelColor(8 * row + (7 - column), green, red, blue);
  }
}

void setup(void)
{
  setSSIDwifiAP("✅ wifi-led-panel ✅");
  setPassWiFiAP("3866210000");
  // registrateEvent("/off", off);
  // registrateEvent("/on", on);
  Serial.begin(9600);
  strip.begin();
  strip.clear();
  coreInit();
}

int micVolume()
{
  int mic = analogRead(A0);
  int minMic = 500;
  if (mic < minMic)
    mic = minMic;
  return map(mic, minMic, 1024, 0, 7);
}

void displayPick(int value)
{
  strip.clear();
  for (int i = 0; i <= value; i++)
  {
    display(7 - i, 1, 10, 100, 10);
  }
  strip.show();
}

void loop(void)
{
  coreHandle();
  Serial.println(micVolume());
  displayPick(micVolume());
  delay(10);
}

void displayPicks(int value)
{
}
