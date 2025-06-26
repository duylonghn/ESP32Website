#include <WiFi.h>
#include <SPI.h>
#include <SD.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include "WebReadSDCard.h"

const char* ssid = "REPLACE_WITH_YOUR_SSID";
const char* password = "REPLACE_WITH_YOUR_PASS";

#define FW_VERSION "1.0.0" // Phiên bản của firmware

#define SD_CS 5
#define SD_SCK 18
#define SD_MISO 19
#define SD_MOSI 23
SPIClass spiSD(VSPI);

AsyncWebServer server(80);
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
  Serial.println();
  Serial.print("📶 Đã kết nối, IP: ");
  Serial.println(WiFi.localIP());

  // Khởi tạo thẻ SD
  spiSD.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);
  if (!SD.begin(SD_CS, spiSD)) {
    Serial.println("⚠️ Không thể khởi tạo thẻ SD!");
    return;
  }
  Serial.println("💾 Thẻ SD đã sẵn sàng");

  // Khởi tạo Web Server xử lý SD
  webSD.begin();
}

void loop() {
  // Không cần làm gì trong loop khi dùng AsyncWebServer
}
