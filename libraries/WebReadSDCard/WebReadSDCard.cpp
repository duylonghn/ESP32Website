#include "WebReadSDCard.h"
#include "ReadSDCard.h"

WebReadSDCard::WebReadSDCard(AsyncWebServer* server, const char* version)
  : _server(server), _version(version) {}

String WebReadSDCard::formatSize(uint64_t bytes) {
  if (bytes >= (1024ULL * 1024ULL * 1024ULL)) return String((float)bytes / (1024 * 1024 * 1024), 2) + " GB";
  if (bytes >= (1024ULL * 1024ULL)) return String((float)bytes / (1024 * 1024), 2) + " MB";
  if (bytes >= 1024) return String((float)bytes / 1024, 2) + " KB";
  return String(bytes) + " B";
}

void WebReadSDCard::begin() {
  handleRoot();
  handleSystemInfo();
  handleListFiles();
  handleUpload();
  handleFileAction();
  handleRename();
  handleReboot();
  handleOTA();

  _server->begin();
  Serial.println("✅ Truy cập: http://" + WiFi.localIP().toString() + "/readsd để vào WebReadSDCard");
}

void WebReadSDCard::handleRoot() {
  _server->on("/readsd", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", readsdcard);
  });
}

void WebReadSDCard::handleSystemInfo() {
  _server->on("/systeminfo", HTTP_GET, [this](AsyncWebServerRequest *request) {
    String json = "{\"VERSION\":\"" + String(_version) + "\",\"SD\":{";

    if (SD.begin()) {
      json += "\"total\":\"" + formatSize(SD.totalBytes()) + "\",";
      json += "\"used\":\"" + formatSize(SD.usedBytes()) + "\",";
      json += "\"free\":\"" + formatSize(SD.totalBytes() - SD.usedBytes()) + "\"";
    } else {
      json += "\"total\":\"0 GB\",\"used\":\"0 GB\",\"free\":\"0 GB\"";
    }
    json += "}}";
    request->send(200, "application/json", json);
  });
}

void WebReadSDCard::handleListFiles() {
  _server->on("/listfiles", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (!request->hasParam("folder")) {
      request->send(400, "text/plain", "Missing 'folder' parameter");
      return;
    }

    String folder = request->getParam("folder")->value();
    if (!folder.startsWith("/")) folder = "/" + folder;

    File dir = SD.open(folder);
    if (!dir || !dir.isDirectory()) {
      request->send(404, "text/plain", "Folder not found: " + folder);
      return;
    }

    String output = "";
    File file = dir.openNextFile();
    while (file) {
      String fullPath = String(file.name());
      int lastSlash = fullPath.lastIndexOf('/');
      String fileName = (lastSlash >= 0) ? fullPath.substring(lastSlash + 1) : fullPath;

      if (fileName.length() > 0) {
        output += (file.isDirectory() ? "Fo " : "Fi ") + fileName;
        if (!file.isDirectory()) {
          output += ": " + String(file.size());
        }
        output += "\n";
      }
      file = dir.openNextFile();
    }

    request->send(200, "text/plain", output);
  });
}

void WebReadSDCard::handleUpload() {
  _server->on("/upload", HTTP_POST, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "OK");
  }, [](AsyncWebServerRequest *request, String filename, size_t index,
        uint8_t *data, size_t len, bool final) {
    static File uploadFile;
    static String folder = "/";

    if (index == 0) {
      if (request->hasParam("folder", true)) {
        folder = request->getParam("folder", true)->value();
        if (!folder.startsWith("/")) folder = "/" + folder;
        if (!SD.exists(folder)) SD.mkdir(folder);
      }
      uploadFile = SD.open(folder + "/" + filename, FILE_WRITE);
    }

    if (uploadFile) uploadFile.write(data, len);
    if (final && uploadFile) uploadFile.close();
  });
}

void WebReadSDCard::handleFileAction() {
  _server->on("/file", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (!request->hasParam("name") || !request->hasParam("action")) {
      request->send(400, "text/plain", "Missing parameters");
      return;
    }

    String name = request->getParam("name")->value();
    if (!name.startsWith("/")) name = "/" + name;
    String action = request->getParam("action")->value();

    if (action == "create") {
      name.replace("//", "/");
      if (SD.mkdir(name)) {
        request->send(200, "text/plain", "Folder created: " + name);
      } else {
        request->send(500, "text/plain", "Failed to create folder: " + name);
      }
    } else if (action == "download") {
      File file = SD.open(name);
      if (!file || file.isDirectory()) {
        request->send(404, "text/plain", "File not found or is directory");
        return;
      }
      request->send(file, name, "application/octet-stream", true);
    } else if (action == "delete") {
      File target = SD.open(name);
      if (!target) {
        request->send(404, "text/plain", "Target not found");
        return;
      }

      if (target.isDirectory()) {
        if (target.openNextFile()) {
          request->send(400, "text/plain", "Folder not empty");
        } else {
          SD.rmdir(name);
          request->send(200, "text/plain", "Deleted folder: " + name);
        }
      } else {
        target.close();
        SD.remove(name);
        request->send(200, "text/plain", "Deleted file: " + name);
      }
    } else {
      request->send(400, "text/plain", "Unknown action");
    }
  });
}

void WebReadSDCard::handleRename() {
  _server->on("/rename", HTTP_POST, [](AsyncWebServerRequest *request) {
    if (!request->hasParam("filePath", true) || !request->hasParam("fileName", true)) {
      request->send(400, "text/plain", "Missing parameters");
      return;
    }

    String oldPath = request->getParam("filePath", true)->value();
    String newName = request->getParam("fileName", true)->value();
    if (!oldPath.startsWith("/")) oldPath = "/" + oldPath;
    String newPath = oldPath.substring(0, oldPath.lastIndexOf("/")) + "/" + newName;

    if (SD.rename(oldPath, newPath)) {
      request->send(200, "text/plain", "Renamed successfully");
    } else {
      request->send(500, "text/plain", "Rename failed");
    }
  });
}

void WebReadSDCard::handleReboot() {
  _server->on("/reboot", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "Rebooting...");
    delay(500);
    ESP.restart();
  });
}

void WebReadSDCard::handleOTA() {
  _server->on("/OTA", HTTP_POST, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "OK");
  });

  _server->on("/OTAFILE", HTTP_POST, [](AsyncWebServerRequest *request) {},
    [](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data,
       size_t len, bool final) {
      static bool updateStarted = false;
      if (index == 0) {
        updateStarted = Update.begin(UPDATE_SIZE_UNKNOWN);
      }
      if (updateStarted) {
        Update.write(data, len);
        if (final) {
          if (Update.end(true)) {
            request->send(200, "text/plain", "OTA Success. Rebooting...");
            delay(1000);
            ESP.restart();
          } else {
            request->send(500, "text/plain", "OTA Failed");
          }
        }
      }
    });
}
