#ifndef WEBREADSDCARD_H
#define WEBREADSDCARD_H

#include <Arduino.h>
#include <WiFi.h>
#include <SD.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Update.h>

class WebReadSDCard {
  public:
    WebReadSDCard(AsyncWebServer* server, const char* version);
    void begin();

  private:
    AsyncWebServer* _server;
    const char* _version;

    String formatSize(uint64_t bytes);
    void handleRoot();
    void handleSystemInfo();
    void handleListFiles();
    void handleUpload();
    void handleFileAction();
    void handleRename();
    void handleReboot();
    void handleOTA();
};

#endif
