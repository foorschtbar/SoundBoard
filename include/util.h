#pragma once
#include <Arduino.h>
#include <libb64/cencode.h>

boolean check_serial_write();
void set_serial_write_failed(boolean failed);

template <typename... T>
inline void log(T... args)
{
#ifdef DEBUG
    if (check_serial_write())
    {
        set_serial_write_failed(Serial.print(args...) == 0);
    }
#endif
}

template <typename... T>
inline void logln(T... args)
{
#ifdef DEBUG
    if (check_serial_write())
    {
        set_serial_write_failed(Serial.println(args...) == 0);
    }
#endif
}

template <typename... T>
inline void logf(T... args)
{
#ifdef DEBUG
    if (check_serial_write())
    {
        set_serial_write_failed(Serial.printf(args...) == 0);
    }
#endif
}

size_t hex_decode(const String &hex, uint8_t *buf, size_t len);
void print_byte(uint8_t byte);
String formatFileSize(size_t fileSize);
long RSSI2Quality(long dBm);
String millisToTime(unsigned long millis);
String resetReason(int reason);
String BasicAuthHash(const char *username, const char *password);