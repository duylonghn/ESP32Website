#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <SD.h>
#include <ElegantOTA.h>
#include "WebSerialMonitor.h"
#include "WebReadSDCard.h"

const char* ssid = "Duy Long";
const char* password = "Khong@biet";

#define SD_CS 5
#define SD_SCK 18
#define SD_MISO 19
#define SD_MOSI 23
SPIClass spiSD(VSPI);

#define FW_VERSION "1.0.0"

AsyncWebServer server(80);
WebSerialMonitor webSerial(&server);
WebReadSDCard webSD(&server, FW_VERSION);

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Kết nối WiFi
  WiFi.begin(ssid, password);
  Serial.print("🔌 Đang kết nối WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ Kết nối WiFi thành công");
  Serial.println("📶 IP: " + WiFi.localIP().toString());

  // Khởi tạo thẻ SD
  spiSD.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);
  if (!SD.begin(SD_CS, spiSD)) {
    Serial.println("⚠️ Không thể khởi tạo thẻ SD!");
    return;
  }
  Serial.println("💾 Thẻ SD đã sẵn sàng");

  // Khởi tạo WebSerialMonitor
  webSerial.begin();

  // Khởi tạo WebReadSDCard
  webSD.begin();
  
  // Bắt đầu server
  server.begin();
  ElegantOTA.begin(&server);
  webSerial.println("✅ Server đã khởi động");
}

void loop() {
  ElegantOTA.loop();
}
