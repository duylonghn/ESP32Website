#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "WebSerialMonitor.h"
#include "SerialMonitor.h"

const char* ssid = "SSID";
const char* password = "PASS";

AsyncWebServer server(80);
WebSerialMonitor webSerial(&server);

void setup() {
  Serial.begin(115200);
  delay(100);

  WiFi.begin(ssid, password);
  Serial.print("🔌 Đang kết nối WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ Kết nối WiFi thành công");
  Serial.println("📶 IP: " + WiFi.localIP().toString());
  // Khởi tạo WebSerial
  webSerial.begin();

  // Tạo lệnh xử lý từ Serial
  webSerial.cmdFromWeb([](const String& cmd) {
    if (cmd.equalsIgnoreCase("REBOOT")) {                 // Lệnh nhập vào Serial
      webSerial.println("♻️ ESP đang khởi động lại..."); // Thông báo hiện ra
      delay(1000);
      ESP.restart();   // Thực hiện
    } else {
      webSerial.println("❌ Lệnh không hợp lệ");
    }
  });

  server.begin();
}

void loop() {
  webSerial.println("⏱ ESP32 đang chạy...");
  delay(5000);
}
