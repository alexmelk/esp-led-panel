#include <Arduino.h>

#include <SmartHomeCore.h>
using namespace shCore;

void getTemperature()
{
  sendToServer(200, "text/html", "10");
  blink(1, 100);
}

void setup(void)
{
  setSSIDwifiAP("WiFi Toggle");
  registrateEvent("/getTemperature", getTemperature);

  coreInit();
}

void loop(void)
{
  coreHandle();
  delay(1);
}
