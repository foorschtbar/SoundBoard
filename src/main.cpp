#include "Arduino.h"
#include "WiFi.h"
#include "Audio.h"
#include <WiFi.h>
#include "SD.h"
#include <SPI.h>
#include <FS.h>
#include <WiFiClient.h>
#include "SingleLED.h"
#include "util.h"

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

Audio audio;
SingleLED led(NEOPIXEL, NEO_GRB + NEO_KHZ800);

void setup()
{

 led.begin();
 led.setBrightness(128);
 led.setColor(255, 0, 0);

 pinMode(LED, OUTPUT);
 digitalWrite(LED, LOW); // LED OFF

 Serial.begin(115200);
 WiFi.mode(WIFI_OFF);

 pinMode(SD_CS, OUTPUT);
 digitalWrite(SD_CS, HIGH);
 SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);
 SD.begin(SD_CS);

 delay(3000);

 logln(F("***************************"));
 logln(F("*        SoundBoard       *"));
 logln(F("***************************"));

 if (!SD.begin(SD_CS))
 {
   logln(F("Failed to mount SD card"));
  }
  else
  {
    logln(F("SD mounted successfully"));
    logf("%s of %s used (%f/%f%%)\n", formatFileSize(SD.usedBytes()), formatFileSize(SD.totalBytes()), SD.usedBytes(), SD.totalBytes() / (float)SD.totalBytes() * 100.0);
  }

  digitalWrite(LED, HIGH); // LED OFF
  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  audio.setVolume(5);
  // audio.setBalance(0);

  // list files
  log(F("List files in root:\n"));
  File root = SD.open("/");
  // print number of files in root

  File file = root.openNextFile();
  while (file)
  {
    logf("- %s (%s)\n", file.name(), formatFileSize(file.size()));
    file = root.openNextFile();
  }
  root.close();

  audio.connecttoFS(SD, "Olsen-Banden.mp3");
  // audio.connecttoFS(SD, "stereo-test.mp3");
  // audio.connecttoFS(SD, "/winxp.mp3");
  // audio.connecttoFS(SD, "winxpshutdown.mp3");
  //audio.connecttoFS(SD, "101 - Paul Van Dyk - For An Angel (PVD Remix 09).mp3");
  // audio.connecttospeech("Mirco hat einen kleinen....KOFFERRAUM!", "de",);
  // audio.connecttohost("https://stream.sunshine-live.de/2000er/mp3-192/stream.sunshine-live.de/");
}

void loop()
{
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
void audio_eof_mp3(const char *info)
{ // end of file
  Serial.print("eof_mp3     ");
  Serial.println(info);
}