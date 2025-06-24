#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SD.h>
#include "WebSerialMonitor.h"
#include "WebReadSDCard.h"

const char* ssid = "SSID";
const char* password = "PASS";

#define SD_CS 5
#define SD_SCK 18
#define SD_MISO 19
#define SD_MOSI 23
SPIClass spiSD(VSPI);

AsyncWebServer server(80);
WebSerialMonitor webSerial(&server);
WebReadSDCard webSD(&server);

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
  webSerial.println("✅ Server đã khởi động");
}

void loop() {
}
