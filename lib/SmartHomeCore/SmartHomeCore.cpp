#include <SmartHomeCore.h>

SmartHomeCore::SmartHomeCore(String ssid, String pass) {
  this->ssid = ssid;
  this->pass = pass;
};

void SmartHomeCore::begin() {
  initSerial();
  initPins();
  initFS();
  initWiFi();
  initServer(port);
}

String SmartHomeCore::sendWifiList(void) {
  String http;
  int n = WiFi.scanNetworks();
  if (n == 0) {
    http += "<option value='-1'>No Networks!</option>";
  } else {
    Serial.println("FindWifi:");
    http += "<option selected>Choose...</option>";
    for (int i = 0; i < n; ++i) {
      http += "<option value='";
      http += WiFi.SSID(i) + "'>" + WiFi.SSID(i) + "</option>";
      Serial.println(WiFi.SSID(i));
      delay(1);
    }
  }
  return http;
}
void SmartHomeCore::blink(int num, int delayMs) {
  for (int i = 0; i < num; i++) {
    digitalWrite(LED_BUILTIN, LOW);
    delay(delayMs);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(delayMs);
  }
}
void SmartHomeCore::clearAll() {
  LittleFS.remove("/wifiConf.conf");
  LittleFS.remove("/device.conf");
  LittleFS.remove("/devices.conf");

  //_server.send(200, "text/html", "clear Ok");
  Serial.println("clear Ok");

  WiFi.disconnect();
  delay(1000);
  ESP.restart();
}
File SmartHomeCore::openFile(String Filename) {
  File f = LittleFS.open("/" + Filename, "r");
  if (!f) {
    Serial.println("file open failed");
    Serial.println(Filename);
  }
  return f;
}
void SmartHomeCore::createOrErase(String Filename, String Text) {
  File f = LittleFS.open("/" + Filename, "w");
  if (!f) {
    Serial.println("file open failed");
    Serial.println(Filename);
  }
  f.print(Text);
  f.close();
}
void SmartHomeCore::onRequest(AsyncWebServerRequest *request) {
  // Handle Unknown Request
  request->send(404);
}
void SmartHomeCore::initSerial() {
  Serial.begin(serialSpeed);
  Serial.setTimeout(serialTimeOut);
}
void SmartHomeCore::initFS() { LittleFS.begin(); }
void SmartHomeCore::initPins() { pinMode(LED_BUILTIN, OUTPUT); }
void SmartHomeCore::initWiFi() {
  File f = openFile("wifiConf.conf");
  if (!f) {
    WiFi.softAPConfig(apIP, apIP, subnet);
    bool connect =
        WiFi.softAP(ssid, pass, wifiChannel, hiddenWifi, maxConnection);
    IPAddress myIP = WiFi.softAPIP();
    if (connect) {
      Serial.println("Успешно");
    } else {
      Serial.println("Ошибка");
    }
    Serial.println("AP IP address: ");
    Serial.println(myIP);
    blink(2, 100);
  } else {
    json.clear();

    Serial.println("Client starting...");

    String str = f.readString();
    Serial.println(str);

    deserializeJson(json, str);
    String wifiSSID = json["wifiSSID"];
    String wifiPass = json["wifiPass"];

    WiFi.mode(WIFI_AP_STA);
    WiFi.begin(wifiSSID, wifiPass);

    int i = 25;
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");

      if (i == 0) {
        Serial.println("Remove: /wifiConf.conf");
        clearAll();
        delay(1000);
      } else {
        i--;
      }
    }
    Serial.println(WiFi.localIP());
    blink(5, 100);
  }
}
void SmartHomeCore::initServer(int port) {
  server.serveStatic("/", LittleFS, "/")
      .setDefaultFile("default.html")
      .setCacheControl("max-age=600");
  server.begin();
  endpoints();
}

void SmartHomeCore::endpoints() {
  server.on("/index", HTTP_ANY, [](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/index.html");
  });
  server.on("/", HTTP_ANY, [](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/index.html");
  });
  server.on("/getWifiList", HTTP_GET, [&](AsyncWebServerRequest *request) {
    String http = sendWifiList();
    request->send(200, "text/plain", http);
  });
  server.onNotFound(
      [&](AsyncWebServerRequest *request) { onRequest(request); });
}
