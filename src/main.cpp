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
// #include <index.h>
#include <mqtt.h>
#include "esp32/rom/rtc.h"

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
#define WIFI_TIMEOUT 1000 * 10               // 10 seconds
#define WIFI_RECONNECT_TIMEOUT 1000 * 60 * 5 // 5 minutes
#define PASSWORD_HIDDEN "**********"
#define MQTT_TOPIC_CMD "cmd"
#define MQTT_TOPIC_STATUS "status"
#define SYSINFO_UPDATE_INTERVAL 1000 * 30
#define FILELIST_CHUNK_SIZE 10
#define DEFAULT_USERNAME "admin"
#define DEFAULT_PASSWORD "admin"
#define DEFAULT_VOLUME 5
#define DEFAULT_BALANCE 0
#define DEFAULT_MQTT_TOPIC_PREFIX "soundboard/"
#define DEFAULT_MQTT_PORT "1883"

// Audioplay
Audio audio;
// LED
SingleLED led(NEOPIXEL, NEO_GRB + NEO_KHZ800);
// MQTT Client
WiFiClient espClient;
MQTT mqtt(espClient);

// Webserver
AsyncWebServer server(80);
AsyncWebSocket websocket("/ws");
std::vector<uint32_t> authenticatedClients;

// Variables
int currentVolume = DEFAULT_VOLUME;
int currentBalance = DEFAULT_BALANCE;

// Settings
bool settingsValid = false;
String settings_ssid;
String settings_psk;
String settings_hostname = DEFAULT_HOSTNAME;
String settings_mqtt_broker;
String settings_mqtt_port = DEFAULT_MQTT_PORT;
String settings_mqtt_user;
String settings_mqtt_pass;
String settings_mqtt_prefix = DEFAULT_MQTT_TOPIC_PREFIX;
String settings_username = DEFAULT_USERNAME;
String settings_password = DEFAULT_PASSWORD;
int settings_volume = DEFAULT_VOLUME;   // 0-21
int settings_balance = DEFAULT_BALANCE; // -16 to 16
bool shouldReboot = false;              // flag to use from web update to reboot the ESP
unsigned long lastSysInfoUpdate = 0;
unsigned long lastWifiReconnect = 0;
bool fsInternalMounted = false;
bool fsExternalMounted = false;

extern const uint8_t file_index_html_start[] asm("_binary_html_index_html_gz_start");
extern const uint8_t file_index_html_end[] asm("_binary_html_index_html_gz_end");

typedef enum DATA_TYPE
{
  DATA_SYSINFO,
  DATA_ALL,
  DATA_FILES
} sendDataType;

void showAction()
{
  led.blink(100, 255, 255, 255);
}

void readSettings()
{

  if (!FILESYSTEM.exists("/settings.json"))
  {
    logln(F("Settings file not found"));
    return;
  }

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
    if (doc.containsKey("username"))
    {
      settings_username = doc["username"].as<String>();
    }
    if (doc.containsKey("password"))
    {
      settings_password = doc["password"].as<String>();
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
    if (doc.containsKey("mqtt_broker"))
    {
      settings_mqtt_broker = doc["mqtt_broker"].as<String>();
    }
    if (doc.containsKey("mqtt_port"))
    {
      settings_mqtt_port = doc["mqtt_port"].as<String>();
    }
    if (doc.containsKey("mqtt_user"))
    {
      settings_mqtt_user = doc["mqtt_user"].as<String>();
    }
    if (doc.containsKey("mqtt_pass"))
    {
      settings_mqtt_pass = doc["mqtt_pass"].as<String>();
    }
    if (doc.containsKey("mqtt_prefix"))
    {
      settings_mqtt_prefix = doc["mqtt_prefix"].as<String>();
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
  doc["username"] = settings_username;
  doc["password"] = settings_password;
  doc["volume"] = settings_volume;
  doc["balance"] = settings_balance;
  doc["mqtt_broker"] = settings_mqtt_broker;
  doc["mqtt_port"] = settings_mqtt_port;
  doc["mqtt_user"] = settings_mqtt_user;
  doc["mqtt_pass"] = settings_mqtt_pass;

  // trim mqtt_prefix
  settings_mqtt_prefix.trim();
  // check if mqtt_prefix ends with a slash
  if (settings_mqtt_prefix[settings_mqtt_prefix.length() - 1] != '/')
  {
    settings_mqtt_prefix += "/";
  }
  doc["mqtt_prefix"] = settings_mqtt_prefix;

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

bool checkCredentials(AsyncWebServerRequest *request)
{
  if (request->authenticate(settings_username.c_str(), settings_password.c_str()))
  {
    return true;
  }

  AsyncWebServerResponse *r = request->beginResponse(401);

  // WebAPI should set the X-Requested-With to prevent browser internal auth dialogs
  if (!request->hasHeader("X-Requested-With"))
  {
    r->addHeader("WWW-Authenticate", "Basic realm=\"Login Required\"");
  }
  request->send(r);

  return false;
}

void textAllAuthenticatedClients(const String &message)
{
  for (uint32_t clientid : authenticatedClients)
  {
    websocket.text(clientid, message);
  }
}

void sendStatus(String status)
{
  showAction();
  websocket.textAll("{\"status\":\"" + status + "\"}");
  if (mqtt.isConnected())
  {
    mqtt.publish(MQTT_TOPIC_STATUS, ("{\"status\":\"" + status + "\"}").c_str());
  }
}

void sendPopup(String msg, String icon = "loading", int refresh = 0)
{
  showAction();
  textAllAuthenticatedClients("{\"popup\":\"" + msg + "\", \"icon\":\"" + icon + "\", \"refresh\":" + String(refresh) + "}");
}

void sendData(sendDataType type, String requestID = "", uint32_t offset = 0)
{
  showAction();
  // Create a JSON document
  StaticJsonDocument<3000> doc;

  doc["requestID"] = requestID;

  if (type == DATA_SYSINFO || type == DATA_ALL)
  {

    JsonObject sysinfo = doc.createNestedObject("sysinfo");
    sysinfo["Hostname"] = settings_hostname;
    sysinfo["Uptime"] = millisToTime(millis());
    sysinfo["CPU0 reset reason"] = resetReason(rtc_get_reset_reason(0));
    sysinfo["CPU1 reset reason"] = resetReason(rtc_get_reset_reason(1));
    sysinfo["MQTT"] = (mqtt.isConnected() ? "Connected" : "Disconnected (" + String(mqtt.state()) + ")");
    if (fsInternalMounted)
    {
      sysinfo["FS (internal)"] = String(formatFileSize(FILESYSTEM.usedBytes())) + "/" + String(formatFileSize(FILESYSTEM.totalBytes())) + " (" + String(FILESYSTEM.usedBytes() / (float)FILESYSTEM.totalBytes() * 100.0) + "%)";
    }
    else
    {
      sysinfo["FS (internal)"] = "Not mounted";
    }
    if (fsExternalMounted)
    {
      sysinfo["FS (external)"] = String(formatFileSize(SD.usedBytes())) + "/" + String(formatFileSize(SD.totalBytes())) + " (" + String(SD.usedBytes() / (float)SD.totalBytes() * 100.0) + "%)";
    }
    else
    {
      sysinfo["FS (external)"] = "Not mounted";
    }
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
    sysinfo["WiFi RSSI"] = String(RSSI2Quality(WiFi.RSSI())) + "% (" + String(WiFi.RSSI()) + " dBm)";
    sysinfo["Free Heap"] = ESP.getFreeHeap();
    sysinfo["Free PSRAM"] = ESP.getFreePsram();
    sysinfo["Chip Revision"] = ESP.getChipRevision();
    sysinfo["SDK"] = ESP.getSdkVersion();
    sysinfo["CPU Freq."] = ESP.getCpuFreqMHz();
    sysinfo["Flash Chip Size"] = ESP.getFlashChipSize();
    sysinfo["Compiled"] = String(__DATE__) + " " + String(__TIME__);

    doc["hostname_header"] = settings_hostname;
  }

  if (type == DATA_ALL)
  {

    // Add data to the JSON document
    doc["start_volume"] = settings_volume;
    doc["cur_volume"] = currentVolume;
    doc["start_balance"] = settings_balance;
    doc["cur_balance"] = currentBalance;
    doc["ssid"] = settings_ssid;
    if (settings_psk.length() > 0)
    {
      doc["psk"] = PASSWORD_HIDDEN;
    }
    else
    {
      doc["psk"] = "";
    }
    doc["mqtt_broker"] = settings_mqtt_broker;
    doc["mqtt_port"] = settings_mqtt_port;
    doc["mqtt_user"] = settings_mqtt_user;
    if (settings_mqtt_pass.length() > 0)
    {
      doc["mqtt_pass"] = PASSWORD_HIDDEN;
    }
    else
    {
      doc["mqtt_pass"] = "";
    }
    doc["mqtt_prefix"] = settings_mqtt_prefix;

    if (fsExternalMounted)
    {
      doc["fs_info"] = String(formatFileSize(SD.usedBytes())) + " of " + String(formatFileSize(SD.totalBytes())) + " used (" + String(SD.usedBytes() / (float)SD.totalBytes() * 100.0) + "%)";
    }
    else
    {
      doc["fs_info"] = "Not mounted";
    }
    doc["hostname"] = settings_hostname;
    doc["username"] = settings_username;
    if (settings_password.length() > 0)
    {
      doc["password"] = PASSWORD_HIDDEN;
    }
    else
    {
      doc["password"] = "";
    }
    doc["status"] = "Reeeaaaady...";
  }

  if (type == DATA_FILES || type == DATA_ALL)
  {
    uint32_t count = 0;
    // Filelist
    JsonArray filesArray = doc.createNestedArray("fs");

    if (fsExternalMounted)
    {
      File root = SD.open("/");
      if (!root)
      {
        logln(F("Failed to open directory"));
        return;
      }

      File file = root.openNextFile();
      while (file)
      {
        if (String(file.name()).indexOf("System Volume Information") == -1)
        {
          if (count >= offset && count <= offset + FILELIST_CHUNK_SIZE)
          {
            // Filter System Volume Information

            JsonObject fileObj = filesArray.createNestedObject();
            fileObj["index"] = count;
            fileObj["name"] = String(file.name());
            fileObj["size"] = String(file.size());
          }
          count++;
        }
        file = root.openNextFile();
        if (count >= offset + FILELIST_CHUNK_SIZE)
        {
          if (file)
          {
            JsonObject morefiles = doc.createNestedObject("morefiles");
            morefiles["offset"] = count;
          }
          break;
        }
      }
      root.close();
    }
  }

  // Serialize the JSON document to a string
  String jsonString;
  serializeJson(doc, jsonString);
  if (type == DATA_SYSINFO)
  {
    websocket.textAll(jsonString);
  }
  else
  {
    textAllAuthenticatedClients(jsonString);
  }

  if (mqtt.isConnected() && type == DATA_SYSINFO) // Only short messages are sent to MQTT
  {
    // logf("Sending MQTT status\n");
    // logf("> Length: %i\n", jsonString.length());
    // logf("> Content: %s\n", jsonString.c_str());
    mqtt.publish(MQTT_TOPIC_STATUS, jsonString.c_str());
  }
}

void sendUnauthenticatedData(uint32_t clientid)
{
  sendData(DATA_SYSINFO);
}

void handleMqttMessage(char *topic, byte *payload, unsigned int length)
{
  showAction();
  String status = "";
  logf("handleMqttMessage\n>Content: ");
#ifdef DEBUG
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
#endif
  logf("> Lenght: %i\n", length);
  logf("> Topic: %s\n", topic);

  if (length)
  {
    StaticJsonDocument<256> doc;
    DeserializationError err = deserializeJson(doc, payload);
    if (err)
    {
      logf("> JSON: deserializeJson() failed: %s", err.c_str());
    }
    else
    {
      if (doc.containsKey("volume"))
      {
        currentVolume = doc["volume"].as<int>();
        status = "Volume: " + String(map(currentVolume, 0, 21, 0, 100)) + "%";
        audio.setVolume(currentVolume);
      }
      if (doc.containsKey("balance"))
      {
        currentBalance = doc["balance"].as<int>();
        status = "Balance: " + String(currentBalance);
        audio.setBalance(currentBalance * -1);
      }
      if (doc.containsKey("play"))
      {
        const char *filename = doc["play"].as<const char *>();
        status = "Play " + String(filename);
        audio.stopSong();
        audio.connecttoFS(SD, filename);
      }
      if (doc.containsKey("stop"))
      {
        audio.stopSong();
        status = "Stop";
      }
      if (doc.containsKey("tts") && doc.containsKey("lang"))
      {
        const char *text = doc["tts"].as<const char *>();
        const char *lang = doc["lang"].as<const char *>();

        logf("TTS (%s): %s\n", lang, text);
        audio.stopSong();
        audio.connecttospeech(text, lang);
      }
      if (doc.containsKey("status"))
      {
        sendData(DATA_SYSINFO);
      }
    }
  }

  if (status.length() > 0)
  {
    sendStatus(status);
  }
}

bool checkWSAuth(uint32_t clientid)
{
  if (std::find(authenticatedClients.begin(), authenticatedClients.end(), clientid) == authenticatedClients.end())
  {
    return false;
  }
  else
  {
    return true;
  }
}

void handleWebSocketMessage(uint32_t clientid, void *arg, uint8_t *data, size_t len)
{
  AwsFrameInfo *info = (AwsFrameInfo *)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
  {
    data[len] = 0;
    String requestID;
    logf("handleWebSocketMessage: %s\n", (char *)data);

    // get websocket client id

    StaticJsonDocument<256> doc;
    deserializeJson(doc, (char *)data);
    if (doc.containsKey("requestID"))
    {
      requestID = doc["requestID"].as<String>();
    }

    if (doc.containsKey("login"))
    {
      const char *auth = doc["login"].as<const char *>();
      // base64 to
      if (BasicAuthHash(settings_username.c_str(), settings_password.c_str()).equalsIgnoreCase(auth))
      {
        logf("Client #%u Authenticated\n", clientid);
        websocket.text(clientid, "{\"auth_status:\": \"Authentication successful\"}");
        authenticatedClients.push_back(clientid);
      }
      else
      {
        logf("Client ID #%u Not Authenticated\n", clientid);
        websocket.text(clientid, "{\"auth_status:\": \"Authentication failed\"}");
        return;
      }
    }
    else if (doc.containsKey("logout"))
    {
      authenticatedClients.erase(std::remove(authenticatedClients.begin(), authenticatedClients.end(), clientid), authenticatedClients.end());
      logf("Client #%u Logged out\n", clientid);
      websocket.text(clientid, "{\"auth_status:\": \"Logged out\"}");
    }
    else if (doc.containsKey("getData"))
    {
      if (checkWSAuth(clientid))
      {
        // Send full data if authenticated
        sendData(DATA_ALL, requestID);
      }
      else
      {
        // Send short data if not authenticated
        sendUnauthenticatedData(clientid);
      }
    }

    // only authenticated clients can send commands
    if (checkWSAuth(clientid))
    {
      if (doc.containsKey("getFiles"))
      {
        uint32_t offset = doc["offset"].as<uint32_t>();

        sendData(DATA_FILES, requestID, offset);
      }
      else if (doc.containsKey("reboot"))
      {
        sendPopup("Rebooting...", "loading", 10);
        shouldReboot = true;
      }
      else if (doc.containsKey("formatfs"))
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
          sendPopup("Formating internal filesystem failed!\n", "error");
        }
      }
      else if (doc.containsKey("resetconfig"))
      {
        logf("Resetting configuration...");
        if (FILESYSTEM.remove("/settings.json"))
        {
          logf("done!\n");
          ESP.restart();
        }
        else
        {
          logf("failed!\n");
          sendPopup("Resetting configuration failed!\n", "error");
        }
      }
      else if (doc.containsKey("tts") && doc.containsKey("lang"))
      {
        const char *text = doc["tts"].as<const char *>();
        const char *lang = doc["lang"].as<const char *>();

        logf("TTS (%s): %s\n", lang, text);
        audio.stopSong();
        audio.connecttospeech(text, lang);
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
        audio.setBalance(currentBalance * -1);
      }
      else if (doc.containsKey("play"))
      {
        const char *filename = doc["play"].as<const char *>();
        sendStatus(String("Play ") + String(filename));
        audio.stopSong();
        audio.connecttoFS(SD, filename);
      }
      else if (doc.containsKey("stop"))
      {
        audio.stopSong();
        sendStatus("Stop");
      }
      else if (doc.containsKey("delete"))
      {
        const char *filename = doc["delete"].as<const char *>();
        sendStatus("Delete " + String(filename));
        if (SD.remove("/" + String(filename)))
        {
          sendStatus("Deleted " + String(filename));
          sendData(DATA_ALL, requestID);
        }
        else
        {
          sendPopup("Failed to delete " + String(filename) + "!", "error");
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
        audio.setBalance(currentBalance * -1);
      }
      else if (doc.containsKey("settings"))
      {

        File file = FILESYSTEM.open("/settings.json", "w");
        if (!file)
        {
          sendPopup("Failed to open file for writing!", "error");
          logln("Failed to open file for writing");
          return;
        }

        // Update current settings variables
        if (doc["settings"].containsKey("ssid"))
        {
          settings_ssid = doc["settings"]["ssid"].as<String>();
        }
        // Update PSK only if not the "hidden value" is sent
        if (doc["settings"].containsKey("psk") && strcmp(doc["settings"]["psk"].as<const char *>(), PASSWORD_HIDDEN) != 0)
        {
          settings_psk = doc["settings"]["psk"].as<String>();
        }
        if (doc["settings"].containsKey("hostname"))
        {
          settings_hostname = doc["settings"]["hostname"].as<String>();
        }
        if (doc["settings"].containsKey("username"))
        {
          settings_username = doc["settings"]["username"].as<String>();
        }
        if (doc["settings"].containsKey("password") && strcmp(doc["settings"]["password"].as<const char *>(), PASSWORD_HIDDEN) != 0)
        {
          settings_password = doc["settings"]["password"].as<String>();
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
        if (doc["settings"].containsKey("mqtt_broker"))
        {
          settings_mqtt_broker = doc["settings"]["mqtt_broker"].as<String>();
        }
        if (doc["settings"].containsKey("mqtt_port"))
        {
          settings_mqtt_port = doc["settings"]["mqtt_port"].as<String>();
        }
        if (doc["settings"].containsKey("mqtt_user"))
        {
          settings_mqtt_user = doc["settings"]["mqtt_user"].as<String>();
        }
        if (doc["settings"].containsKey("mqtt_pass") && strcmp(doc["settings"]["mqtt_pass"].as<const char *>(), PASSWORD_HIDDEN) != 0)
        {
          settings_mqtt_pass = doc["settings"]["mqtt_pass"].as<String>();
        }
        if (doc["settings"].containsKey("mqtt_prefix"))
        {
          settings_mqtt_prefix = doc["settings"]["mqtt_prefix"].as<String>();
        }

        writeSettings();

        sendPopup("Settings saved. Rebooting...", "loading", 10);
        shouldReboot = true;
      }
    }
  }
}

void onMQTTEvent(MQTTEventType event)
{
  switch (event)
  {
  case MQTT_EVT_CONNECT:
    logln("MQTT (re)connected");
    mqtt.subscribe(MQTT_TOPIC_CMD);
    logf("> Subscribed to: %s\n", (settings_mqtt_prefix + MQTT_TOPIC_CMD).c_str());
    mqtt.publish(MQTT_TOPIC_STATUS, "{\"status\":\"Connected\"}");
    break;
  case MQTT_EVT_DISCONNECT:
    break;
  case MQTT_EVT_CONNECT_FAILED:
    log("MQTT connection failed: ");
    logln(mqtt.state());
  }
  sendData(DATA_SYSINFO);
}

void onWSEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
  switch (type)
  {
  case WS_EVT_CONNECT:
    logf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
    break;
  case WS_EVT_DISCONNECT:
    logf("WebSocket client #%u disconnected\n", client->id());
    authenticatedClients.erase(std::remove(authenticatedClients.begin(), authenticatedClients.end(), client->id()), authenticatedClients.end());
    break;
  case WS_EVT_DATA:
    handleWebSocketMessage(client->id(), arg, data, len);
    break;
  case WS_EVT_PONG:
  case WS_EVT_ERROR:
    break;
  }
}

void initMQTT()
{
  mqtt.onMessage(handleMqttMessage);
  mqtt.onEvent(onMQTTEvent);
  mqtt.setPrefix(settings_mqtt_prefix.c_str());
  mqtt.setBufferSize(1024);
  mqtt.begin(settings_mqtt_broker.c_str(), settings_mqtt_port.toInt(), WiFi.getHostname(), settings_mqtt_user.c_str(), settings_mqtt_pass.c_str());
  mqtt.setLWT(MQTT_TOPIC_STATUS, 0, true, "{\"status\":\"Disconnected\"}");
  mqtt.connect();
}

void initWebSocket()
{

  websocket.onEvent(onWSEvent);
  server.addHandler(&websocket);
}

void handleUpload(AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data, size_t len, bool final)
{
  if (!checkCredentials(request))
  {
    return;
  }

  if (!index)
  {
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
  if (!checkCredentials(request))
  {
    return;
  }

  if (!index)
  {
    audio.stopSong();
    logln("Flashing firmware starting...");
    if (!Update.begin(UPDATE_SIZE_UNKNOWN, U_FLASH))
    {
      logln("> Flashing firmware failed!");
#ifdef DEBUG
      Update.printError(Serial);
#endif
    }
  }

  if (!Update.hasError())
  {
    if (Update.write(data, len) != len)
    {
#ifdef DEBUG
      Update.printError(Serial);
#endif
    }
  }

  if (final)
  {
    if (!Update.end(true))
    {
      logln("> Flasing firmware failed!");
#ifdef DEBUG
      Update.printError(Serial);
#endif
    }
    else
    {
      logln(F("> Flasing firmware complete!"));
#ifdef DEBUG
      Serial.flush();
#endif
    }
  }
}

void responseBinaryDataWithETagCache(AsyncWebServerRequest *request, const String &contentType, const String &contentEncoding, const uint8_t *content, size_t len)
{
  auto md5 = MD5Builder();
  md5.begin();
  md5.add(const_cast<uint8_t *>(content), len);
  md5.calculate();

  String expectedEtag;
  expectedEtag = "\"";
  expectedEtag += md5.toString();
  expectedEtag += "\"";

  bool eTagMatch = false;
  if (request->hasHeader("If-None-Match"))
  {
    const AsyncWebHeader *h = request->getHeader("If-None-Match");
    eTagMatch = h->value().equals(expectedEtag);
  }

  logf("Serving %s (%s) %s\n", request->url().c_str(), contentType.c_str(), eTagMatch ? "from cache" : "");
  showAction();

  // begin response 200 or 304
  AsyncWebServerResponse *response;
  if (eTagMatch)
  {
    response = request->beginResponse(304);
  }
  else
  {
    response = request->beginResponse(200, contentType, content, len);
    if (contentEncoding.length() > 0)
    {
      response->addHeader("Content-Encoding", contentEncoding);
    }
  }

  // HTTP requires cache headers in 200 and 304 to be identical
  response->addHeader("Cache-Control", "public, must-revalidate");
  response->addHeader("ETag", expectedEtag);

  request->send(response);
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
    bool toggle = false;

    while (WiFi.status() != WL_CONNECTED)
    {
      led.toggleColor(0, 0, 255); // Blue. Connecting...
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

    WiFi.setAutoReconnect(true);

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
  led.setBrightness(64);
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
      logln(F("Restarting..."));
      sleep(5000);
      ESP.restart();
    }
  }
  else
  {
    logln(F("Internal filesystem mounted successfully"));
    logf("> %s of %s used (%0.0f%%)\n", formatFileSize(FILESYSTEM.usedBytes()), formatFileSize(FILESYSTEM.totalBytes()), FILESYSTEM.usedBytes() / (float)FILESYSTEM.totalBytes() * 100.0);
    fsInternalMounted = true;
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
    fsExternalMounted = true;
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
  server.on("/", HTTP_GET, [&](AsyncWebServerRequest *request)
            { responseBinaryDataWithETagCache(request, "text/html", "gzip", file_index_html_start, file_index_html_end - file_index_html_start); });

  server.on("/index.html", HTTP_GET, [&](AsyncWebServerRequest *request)
            { responseBinaryDataWithETagCache(request, "text/html", "gzip", file_index_html_start, file_index_html_end - file_index_html_start); });

  // https://github.com/AR-D-R/ESP32-OTA-File-management/blob/main/OTA_file_management.ino
  server.on(
      "/update", HTTP_POST, [](AsyncWebServerRequest *request)
      {
        // on finished upload, create response
        if (!checkCredentials(request))
        {
          return;
        } else {
    shouldReboot = !Update.hasError();
    AsyncWebServerResponse *response = request->beginResponse(200, "application/json", shouldReboot ? "{ \"success\": true, \"refresh\": 10}" : "{ \"success\": false}");
    response->addHeader("Connection", "close");
    request->send(response); 
} }, handleUpdate);

  // handle file downloads at subfolder /download/<filename>
  server.on("/download", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              if (!checkCredentials(request))
              {
                return;
              }
              if (request->hasParam("file"))
              {
                String filename = request->getParam("file")->value();
                logf("Download file: %s\n", filename.c_str());
                AsyncWebServerResponse *response = request->beginResponse(SD, "/" + filename, "application/octet-stream", true);
                response->addHeader("Content-Disposition", "attachment; filename=" + filename);
                request->send(response);
              }
              else
              {
                request->send(400, "text/plain", "400: Invalid Request");
              } });

  // run handleUpload function when any file is uploaded
  server.on(
      "/upload", HTTP_POST, [](AsyncWebServerRequest *request) { /*request->send(200);*/ },
      handleUpload);

  server.on("/auth", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              if (!checkCredentials(request))
              {
                return;
              }
              request->send(200, "application/json", "{ \"success\": true}"); });

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
  audio.setBalance(settings_balance * -1);

  // MQTT
  if (settingsValid)
  {
    initMQTT();
  }

  logln(F("Setup done."));
  delay(100);

  if (WiFi.getMode() == WIFI_STA)
  {
    // WiFi Station. Status Green. Ready...
    led.setColor(0, 255, 0);
  }
  else
  {
    // WiFi AP. Status Violet. Ready...
    led.setColor(148, 0, 211);
  }
}

void loop()
{
  websocket.cleanupClients();
  audio.loop();
  vTaskDelay(1);

  // Disable MQTT in AP mode
  if (WiFi.status() == WL_CONNECTED)
  {
    mqtt.loop();
  }

  // Reboot if we had a SSID configured but are in AP mode
  if (millis() - lastWifiReconnect > WIFI_RECONNECT_TIMEOUT && WiFi.getMode() == WIFI_AP && settings_ssid != "")
  {
    sendStatus("Rebooting to switch to station mode...");
    logln(F("Rebooting to switch to station mode..."));
    shouldReboot = true;
  }

  // Check if we should reboot
  if (shouldReboot)
  {
    logln("> Rebooting...");
    delay(100);
    ESP.restart();
  }

  // send actual system info every x seconds
  // via websocket and MQTT
  if (millis() - lastSysInfoUpdate > SYSINFO_UPDATE_INTERVAL)
  {
    sendData(DATA_SYSINFO);
    lastSysInfoUpdate = millis();
  }

  vTaskDelay(1);
}

// optional
void audio_info(const char *info)
{
#ifdef DEBUG
  Serial.print("info        ");
  Serial.println(info);
#endif
}
void audio_id3data(const char *info)
{ // id3 metadata
#ifdef DEBUG
  Serial.print("id3data     ");
  Serial.println(info);
#endif
}

// end of file
void audio_eof_mp3(const char *info)
{
  logf("audio_eof_mp3: %s\n", info);
  sendStatus("End of file " + String(info));
}
