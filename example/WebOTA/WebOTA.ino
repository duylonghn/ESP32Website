#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ElegantOTA.h>

const char* ssid = "REPLACE_WITH_YOUR_SSID";
const char* password = "REPLACE_WITH_YOUR_PASS";

AsyncWebServer server(80);

void setup(void) {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  WiFi.begin(ssid, password);
  Serial.print("🔌 Đang kết nối WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("📶 Đã kết nối, IP: ");
  Serial.println(WiFi.localIP());

  server.begin();
  Serial.println("✅ Truy cập: http://" + WiFi.localIP().toString() + "/updateota để vào WebOTA");

  ElegantOTA.begin(&server);
}

void loop(void) {
  ElegantOTA.loop();
}