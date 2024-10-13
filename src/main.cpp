#include "Arduino.h"
#include <ArduinoJson.h>
#include "Audio.h"
#include "SD.h"
#include <LittleFS.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Update.h>
#include <SPI.h>
#include <FS.h>
#include "SingleLED.h"
#include "util.h"
#include <Ticker.h>
#include <index.h>

// I2S Pins
#define I2S_DOUT 25
#define I2S_LRC 26
#define I2S_BCLK 27
// SD Card Pins
#define SD_CS 5
#define SPI_SCK 18
#define SPI_MISO 19
#define SPI_MOSI 23
// Other Pins
#define LED 2 // Blue onboard LED
#define NEOPIXEL 13

// Defaul values
#define BRIGHTNESS_DEFAULT 50
#define DEFAULT_HOSTNAME "SoundBoard"
#define WIFI_AP_SSID "SoundBoard"
#define WIFI_AP_PSK "12345678x!"
#define WIFI_TIMEOUT 1000 * 10 // 10 seconds
#define WIFI_PSK_HIDDEN "**********"
#define U_PART U_SPIFFS

// Audioplay
Audio audio;
// LED
Adafruit_NeoPixel neopixel(1, NEOPIXEL, NEO_GRB + NEO_KHZ800);
SingleLED led(neopixel);
// LED Ticker
Ticker ledTicker;

// Webserver
AsyncWebServer server(80);
AsyncWebSocket websocket("/ws");

// Variables
int currentVolume = 0;
int currentBalance = 0;

// Settings
bool settingsValid = false;
String settings_ssid;
String settings_psk;
String settings_hostname = DEFAULT_HOSTNAME;
int settings_volume = 5;  // 0-21
int settings_balance = 0; // -16 to 16

void showAction()
{
  led.saveColor();
  led.off();
  ledTicker.attach_ms(100, []() {
    led.restoreColor();
    ledTicker.detach();
  });
}

void readSettings()
{
  // Open the file for reading
  File file = FILESYSTEM.open("/settings.json", "r");

  if (!file)
  {
    logln(F("Failed to open file for reading"));
    return;
  }

  // Allocate a temporary JsonDocument
  StaticJsonDocument<512> doc;

  // Parse the JSON data from the file
  DeserializationError error = deserializeJson(doc, file);

  if (error)
  {
    logln(F("Failed to parse JSON"));
    return;
  }
  else
  {
    if (doc.containsKey("ssid"))
    {
      settings_ssid = doc["ssid"].as<String>();
    }
    if (doc.containsKey("psk"))
    {
      settings_psk = doc["psk"].as<String>();
    }
    if (doc.containsKey("hostname"))
    {
      settings_hostname = doc["hostname"].as<String>();
    }
    if (doc.containsKey("volume"))
    {
      settings_volume = doc["volume"].as<int>();
      currentVolume = settings_volume;
    }
    if (doc.containsKey("balance"))
    {
      settings_balance = doc["balance"].as<int>();
      currentBalance = settings_balance;
    }

    settingsValid = true;
  }

  // Close the file
  file.close();
}

void writeSettings()
{
  // Allocate a temporary JsonDocument
  StaticJsonDocument<512> doc;

  doc["ssid"] = settings_ssid;
  doc["psk"] = settings_psk;
  doc["hostname"] = settings_hostname;
  doc["volume"] = settings_volume;
  doc["balance"] = settings_balance;

  // Open the file for writing
  File file = FILESYSTEM.open("/settings.json", "w");
  if (!file)
  {
    logln(F("Failed to open file for writing"));
    return;
  }

  // Serialize JSON to file
  if (serializeJson(doc, file) == 0)
  {
    logln(F("Failed to write to file"));
  }
}

void sendStatus(String status, int refresh = 0)
{
  showAction();
  websocket.textAll("{\"status\":\"" + status + "\", \"refresh\":" + String(refresh) + "}");
}

void sendAlert(String alert, int refresh = 0)
{
  showAction();
  websocket.textAll("{\"alert\":\"" + alert + "\", \"refresh\":" + String(refresh) + "}");
}

void sendData()
{
  showAction();
  // Create a JSON document
  StaticJsonDocument<3000> doc;

  // Add data to the JSON document
  doc["status"] = "Reeeaaaady...";
  doc["start_volume"] = settings_volume;
  doc["cur_volume"] = currentVolume;
  doc["start_balance"] = settings_balance;
  doc["cur_balance"] = currentBalance;
  doc["ssid"] = settings_ssid;
  if (settings_psk.length() > 0)
  {
    doc["psk"] = WIFI_PSK_HIDDEN;
  }
  else
  {
    doc["psk"] = "";
  }
  doc["fs_info"] = String(formatFileSize(SD.usedBytes())) + " of " + String(formatFileSize(SD.totalBytes())) + " used (" + String(SD.usedBytes() / (float)SD.totalBytes() * 100.0) + "%)";
  doc["hostname"] = settings_hostname;
  doc["hostname_header"] = settings_hostname;

  JsonObject sysinfo = doc.createNestedObject("sysinfo");
  sysinfo["Hostname"] = settings_hostname;
  sysinfo["FS (internal)"] = String(formatFileSize(FILESYSTEM.usedBytes())) + " of " + String(formatFileSize(FILESYSTEM.totalBytes())) + " used (" + String(FILESYSTEM.usedBytes() / (float)FILESYSTEM.totalBytes() * 100.0) + "%)";
  sysinfo["FS (external)"] = String(formatFileSize(SD.usedBytes())) + " of " + String(formatFileSize(SD.totalBytes())) + " used (" + String(SD.usedBytes() / (float)SD.totalBytes() * 100.0) + "%)";
  sysinfo["Wifi mode"] = (WiFi.getMode() == WIFI_AP ? "AccessPoint" : (WiFi.getMode() == WIFI_STA ? "Station" : "Unkown"));
  if (WiFi.getMode() == WIFI_STA)
  {
    sysinfo["SSID"] = WiFi.SSID();
    sysinfo["IP address"] = WiFi.localIP().toString();
    sysinfo["Subnet mask"] = WiFi.subnetMask().toString();
    sysinfo["Gateway"] = WiFi.gatewayIP().toString();
    sysinfo["MAC address"] = WiFi.macAddress();
  }
  else if (WiFi.getMode() == WIFI_AP)
  {
    sysinfo["SSID"] = WiFi.softAPSSID();
    sysinfo["IP address"] = WiFi.softAPIP().toString();
    sysinfo["MAC address"] = WiFi.softAPmacAddress();
  }
  sysinfo["Free Heap"] = ESP.getFreeHeap();
  sysinfo["Free PSRAM"] = ESP.getFreePsram();
  sysinfo["Chip Revision"] = ESP.getChipRevision();
  sysinfo["SDK"] = ESP.getSdkVersion();
  sysinfo["CPU Freq."] = ESP.getCpuFreqMHz();
  sysinfo["Flash Chip Size"] = ESP.getFlashChipSize();
  sysinfo["Compiled"] = String(__DATE__) + " " + String(__TIME__);

  // Filelist
  JsonArray filesArray = doc.createNestedArray("fs");

  File root = SD.open("/");
  if (!root)
  {
    logln(F("Failed to open directory"));
    return;
  }

  File file = root.openNextFile();
  while (file)
  {
    // Filter System Volume Information
    if (String(file.name()).indexOf("System Volume Information") == -1)
    {
      JsonObject fileObj = filesArray.createNestedObject();
      fileObj["name"] = String(file.name());
      fileObj["size"] = String(file.size());
    }

    file = root.openNextFile();
  }
  root.close();

  // Serialize the JSON document to a string
  String jsonString;
  serializeJson(doc, jsonString);
  websocket.textAll(jsonString);
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len)
{
  AwsFrameInfo *info = (AwsFrameInfo *)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
  {
    data[len] = 0;
    logf("handleWebSocketMessage: %s\n", (char *)data);

    StaticJsonDocument<256> doc;
    deserializeJson(doc, (char *)data);

    if (doc.containsKey("cmd"))
    {
      const char *cmd = doc["cmd"].as<const char *>();
      if (strcmp(cmd, "reboot") == 0)
      {
        sendAlert(String("Rebooting..."), 10);
        sleep(100);
        ESP.restart();
      }
      else if (strcmp(cmd, "reset") == 0)
      {
        logf("Formating internal filesystem...");
        if (FILESYSTEM.format())
        {
          logf("done!\n");
          ESP.restart();
        }
        else
        {
          logf("failed!\n");
          sendAlert("Formating internal filesystem failed!\n");
        }
      }
    }
    else if (doc.containsKey("play"))
    {
      const char *filename = doc["play"].as<const char *>();
      sendStatus(String("Play ") + String(filename));
      audio.connecttoFS(SD, filename);
    }
    else if (doc.containsKey("delete"))
    {
      const char *filename = doc["delete"].as<const char *>();
      sendStatus("Delete " + String(filename));
      if (SD.remove("/" + String(filename)))
      {
        sendStatus("Deleted " + String(filename));
        sendData();
      }
      else
      {
        sendAlert("Failed to delete " + String(filename) + "!");
      }
    }
    else if (doc.containsKey("volume"))
    {
      currentVolume = doc["volume"].as<int>();
      sendStatus(String("Volume: " + String(map(currentVolume, 0, 21, 0, 100))) + "%");
      audio.setVolume(currentVolume);
    }
    else if (doc.containsKey("balance"))
    {
      currentBalance = doc["balance"].as<int>();
      sendStatus(String("Balance: ") + String(currentBalance));
      audio.setBalance(currentBalance);
    }
    else if (doc.containsKey("settings"))
    {

      File file = FILESYSTEM.open("/settings.json", "w");
      if (!file)
      {
        sendAlert(String("Failed to open file for writing!"));
        logln("Failed to open file for writing");
        return;
      }

      // Update current settings variables
      if (doc["settings"].containsKey("ssid"))
      {
        settings_ssid = doc["settings"]["ssid"].as<String>();
      }
      // Update PSK only if not the "hidden value" is sent
      if (doc["settings"].containsKey("psk") && strcmp(doc["settings"]["psk"].as<const char *>(), WIFI_PSK_HIDDEN) != 0)
      {
        settings_psk = doc["settings"]["psk"].as<String>();
      }
      if (doc["settings"].containsKey("hostname"))
      {
        settings_hostname = doc["settings"]["hostname"].as<String>();
      }
      if (doc["settings"].containsKey("volume"))
      {
        int temp = doc["settings"]["volume"].as<int>();
        if (temp >= 0 && temp <= 21)
        {
          settings_volume = temp;
        }
      }
      if (doc["settings"].containsKey("balance"))
      {
        int temp = doc["settings"]["balance"].as<int>();
        if (temp >= -16 && temp <= 16)
        {
          settings_balance = temp;
        }
      }

      writeSettings();

      if (doc.containsKey("reboot"))
      {
        sendAlert(String("Settings saved. Rebooting..."), 10);
        sleep(100);
        ESP.restart();
      }
      else
      {
        sendAlert(String("Settings saved."));
      }
    }
    else if (doc.containsKey("getData"))
    {
      sendData();
    }
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
  switch (type)
  {
  case WS_EVT_CONNECT:
    logf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
    break;
  case WS_EVT_DISCONNECT:
    logf("WebSocket client #%u disconnected\n", client->id());
    break;
  case WS_EVT_DATA:
    handleWebSocketMessage(arg, data, len);
    break;
  case WS_EVT_PONG:
  case WS_EVT_ERROR:
    break;
  }
}

void initWebSocket()
{
  websocket.onEvent(onEvent);
  server.addHandler(&websocket);
}

void handleUpload(AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data, size_t len, bool final)
{
  if (!index)
  {
    showAction();
    logf("Upload started: %s\n", filename.c_str());
    request->_tempFile = SD.open("/" + filename, "w");
  }
  if (len)
  {
    // stream the incoming chunk to the opened file
    // logf("> Write file: %s (index=%zu, len=%zu)\n", filename.c_str(), index, len);
    request->_tempFile.write(data, len);
  }
  if (final)
  {
    request->_tempFile.close();
    logf("Upload completed: %s (%d bytes)\n", filename.c_str(), index + len);
    AsyncWebServerResponse *response = request->beginResponse(200, "application/json", "{ \"success\": true}");
    request->send(response);
  }
}

void handleUpdate(AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data, size_t len, bool final)
{

  if (!index)
  {
    audio.stopSong();
    showAction();
    logln("Flashing firmware starting...");
    // if filename includes spiffs, update the spiffs partition
    int cmd = (filename.indexOf("spiffs") > -1) ? U_PART : U_FLASH;
    if (!Update.begin(UPDATE_SIZE_UNKNOWN, cmd))
    {
      logln("Flashing firmware failed!");
#ifdef DEBUG
      Update.printError(Serial);
#endif
    }
  }

  if (Update.write(data, len) != len)
  {
#ifdef DEBUG
    Update.printError(Serial);
#endif
  }

  if (final)
  {
    AsyncWebServerResponse *response = request->beginResponse(200, "application/json", "{ \"success\": true, \"refresh\": 10}");
    request->send(response);
    if (!Update.end(true))
    {
      logln("Flasing firmware failed!");
#ifdef DEBUG
      Update.printError(Serial);
#endif
    }
    else
    {
      logln(F("> Flasing firmware complete!"));
      ESP.restart();
    }
  }
}

void handleRoot(AsyncWebServerRequest *request)
{
  logln(F("Displaying index.html"));
  showAction();
  request->send(200, "text/html", index_html);
}

void handleNotFound(AsyncWebServerRequest *request)
{
  showAction();
  // handling pre-flight options like CORS (Cross Origin Resource Sharing)
  if (request->method() == HTTP_OPTIONS)
  {
    logln(F("Handling pre-flight request"));
    request->send(200);
  }
  else
  {
    logf("Not found: %s\n", request->url().c_str());
    request->send(404, "text/plain", "Not found");
  }
}

void setupWiFiStation()
{
  log(F("Connecting to WiFi"));
  if (settingsValid)
  {
    WiFi.mode(WIFI_STA);
    WiFi.begin(settings_ssid, settings_psk);

    unsigned long startTime = millis();

    while (WiFi.status() != WL_CONNECTED)
    {
      if (millis() - startTime >= WIFI_TIMEOUT)
      {
        logln(F("\nWiFi connection timeout, switching to AP mode"));
        return;
      }
      else
      {
        log(F("."));
      }
      delay(100);
    }

    logln(F("\nConnected to the WiFi network"));
    logf("> Hostname:    %s\n", WiFi.getHostname());
    logf("> IP address:  %s\n", WiFi.localIP().toString().c_str());
    logf("> Subnet mask: %s\n", WiFi.subnetMask().toString().c_str());
    logf("> Gateway:     %s\n", WiFi.gatewayIP().toString().c_str());
    logf("> MAC address: %s\n", WiFi.macAddress().c_str());
    return;
  }
  else
  {
    logln(F("\nNo valid settings found, switching to AP mode"));
    return;
  }
}

void setupWiFiAP()
{
  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
  delay(100);
  WiFi.mode(WIFI_AP);
  WiFi.softAP(WIFI_AP_SSID, WIFI_AP_PSK);

  logln(F("Started in AP mode"));
  logf("> Hostname:    %s\n", WiFi.getHostname());
  logf("> IP address:  %s\n", WiFi.softAPIP().toString().c_str());
  logf("> MAC address: %s\n", WiFi.softAPmacAddress().c_str());
}

void setup()
{
  led.begin();
  led.setBrightness(128);
  led.setColor(255, 165, 0); // Orange. Booting...

  // Power off onboard LED
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW); // LED OFF

  // Serial
  Serial.begin(115200);
  WiFi.mode(WIFI_OFF);

  delay(1000);

  logln(F("***************************"));
  logln(F("*        SoundBoard       *"));
  logln(F("***************************"));

  // Mount internal filesystem
  if (!FILESYSTEM.begin(false /* false: Do not format if mount failed */))
  {
    logln(F("Failed to mount internal filesystem"));
    if (!FILESYSTEM.begin(true /* true: format */))
    {
      logln(F("Failed to format internal filesystem"));
    }
    else
    {
      logln(F("Internal filesystem formatted successfully"));
    }
  }
  else
  {
    logln(F("Internal filesystem mounted successfully"));
    logf("> %s of %s used (%0.0f%%)\n", formatFileSize(FILESYSTEM.usedBytes()), formatFileSize(FILESYSTEM.totalBytes()), FILESYSTEM.usedBytes() / (float)FILESYSTEM.totalBytes() * 100.0);
  }

  // Read settings
  readSettings();

  // SD Card
  pinMode(SD_CS, OUTPUT);
  digitalWrite(SD_CS, HIGH);
  SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);
  SD.begin(SD_CS);

  if (!SD.begin(SD_CS))
  {
    logln(F("Failed to mount SD card"));
  }
  else
  {
    logln(F("SD mounted successfully"));
    logf("> %s of %s used (%0.0f%%)\n", formatFileSize(SD.usedBytes()), formatFileSize(SD.totalBytes()), SD.usedBytes() / (float)SD.totalBytes() * 100.0);
  }

  // WiFi
  if (!settings_hostname.isEmpty())
  {
    logf("Setting hostname to %s\n", settings_hostname.c_str());
    WiFi.setHostname(settings_hostname.c_str());
  }
  setupWiFiStation();
  if (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    setupWiFiAP();
  }

  // WebServer
  server.on("/", handleRoot);

  // https://github.com/AR-D-R/ESP32-OTA-File-management/blob/main/OTA_file_management.ino
  server.on(
      "/update", HTTP_POST,
      [](AsyncWebServerRequest *request) {},
      handleUpdate);

  // run handleUpload function when any file is uploaded
  server.on(
      "/upload", HTTP_POST, [](AsyncWebServerRequest *request) { /*request->send(200);*/ },
      handleUpload);

  // not found handler
  server.onNotFound(handleNotFound);

  // Normal Webserver
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods", "*");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "*");
  server.begin();
  logln(F("Webserver server started"));

  // Websocket Server
  initWebSocket();
  logln(F("WebSocket server started"));

  // Audio init
  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  audio.setVolume(settings_volume);
  audio.setBalance(settings_balance);

  logln(F("Setup done."));
  led.setColor(0, 255, 0); // Green. Ready...
}

void loop()
{
  websocket.cleanupClients();
  audio.loop();
}

// optional
void audio_info(const char *info)
{
  Serial.print("info        ");
  Serial.println(info);
}
void audio_id3data(const char *info)
{ // id3 metadata
  Serial.print("id3data     ");
  Serial.println(info);
}

// end of file
void audio_eof_mp3(const char *info)
{
  logf("audio_eof_mp3: %s\n", info);
  sendStatus("End of file " + String(info));
}