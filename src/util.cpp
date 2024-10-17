#include "util.h"

static boolean serial_write_failed = false;

void set_serial_write_failed(boolean failed)
{
    serial_write_failed = failed;
}

size_t hex_decode(const String &hex, uint8_t *buf, size_t len)
{
    size_t skip = 0;
    for (size_t i = 0; i < len; i++)
    {
        char high_nibble = hex[2 * i + skip];
        char low_nibble = hex[2 * i + 1 + skip];
        if (high_nibble == '\0' || low_nibble == '\0')
        {
            return i;
        }
        if (high_nibble <= ' ' || low_nibble <= ' ')
        {
            skip++;
            i--;
            continue;
        }
        buf[i] = (high_nibble >= 'a' ? (high_nibble - 'a' + 10) : high_nibble >= 'A' ? (high_nibble - 'A' + 10)
                                                                                     : high_nibble - '0')
                 << 4;
        buf[i] |= low_nibble >= 'a' ? (low_nibble - 'a' + 10) : low_nibble >= 'A' ? (low_nibble - 'A' + 10)
                                                                                  : low_nibble - '0';
    }
    return len;
}

const char *bit_rep[16] = {
    [0] = "0000",
    [1] = "0001",
    [2] = "0010",
    [3] = "0011",
    [4] = "0100",
    [5] = "0101",
    [6] = "0110",
    [7] = "0111",
    [8] = "1000",
    [9] = "1001",
    [10] = "1010",
    [11] = "1011",
    [12] = "1100",
    [13] = "1101",
    [14] = "1110",
    [15] = "1111",
};

void print_byte(uint8_t byte)
{
    logf("%s%s", bit_rep[byte >> 4], bit_rep[byte & 0x0F]);
}

boolean check_serial_write()
{
    if (!Serial)
        return false;
    if (!Serial.availableForWrite() && serial_write_failed)
        return false;
    return true;
}

String formatFileSize(size_t fileSize)
{
    if (fileSize < 1024)
    {
        return String(fileSize) + " bytes";
    }
    else if (fileSize < 1024 * 1024)
    {
        return String(fileSize / 1024.0, 2) + " KB"; // 2 decimal places
    }
    else if (fileSize < 1024 * 1024 * 1024)
    {
        return String(fileSize / (1024.0 * 1024), 2) + " MB"; // 2 decimal places
    }
    else
    {
        return String(fileSize / (1024.0 * 1024 * 1024), 2) + " GB"; // 2 decimal places
    }
}

long RSSI2Quality(long dBm)
{
    if (dBm <= -100)
        return 0;
    else if (dBm >= -50)
        return 100;
    else
        return 2 * (dBm + 100);
}

String millisToTime(unsigned long millis)
{
    unsigned long seconds = millis / 1000;
    unsigned long minutes = seconds / 60;
    unsigned long hours = minutes / 60;
    seconds %= 60;
    minutes %= 60;
    return (hours < 10 ? "0" + String(hours) : String(hours)) + ":" + (minutes < 10 ? "0" + String(minutes) : String(minutes)) + ":" + (seconds < 10 ? "0" + String(seconds) : String(seconds));
}

String resetReason(int reason)
{
    switch (reason)
    {
    case 1:
        return "POWERON_RESET";
        break; /**<1,  Vbat power on reset*/
    case 3:
        return "SW_RESET";
        break; /**<3,  Software reset digital core*/
    case 4:
        return "OWDT_RESET";
        break; /**<4,  Legacy watch dog reset digital core*/
    case 5:
        return "DEEPSLEEP_RESET";
        break; /**<5,  Deep Sleep reset digital core*/
    case 6:
        return "SDIO_RESET";
        break; /**<6,  Reset by SLC module, reset digital core*/
    case 7:
        return "TG0WDT_SYS_RESET";
        break; /**<7,  Timer Group0 Watch dog reset digital core*/
    case 8:
        return "TG1WDT_SYS_RESET";
        break; /**<8,  Timer Group1 Watch dog reset digital core*/
    case 9:
        return "RTCWDT_SYS_RESET";
        break; /**<9,  RTC Watch dog Reset digital core*/
    case 10:
        return "INTRUSION_RESET";
        break; /**<10, Instrusion tested to reset CPU*/
    case 11:
        return "TGWDT_CPU_RESET";
        break; /**<11, Time Group reset CPU*/
    case 12:
        return "SW_CPU_RESET";
        break; /**<12, Software reset CPU*/
    case 13:
        return "RTCWDT_CPU_RESET";
        break; /**<13, RTC Watch dog Reset CPU*/
    case 14:
        return "EXT_CPU_RESET";
        break; /**<14, for APP CPU, reseted by PRO CPU*/
    case 15:
        return "RTCWDT_BROWN_OUT_RESET";
        break; /**<15, Reset when the vdd voltage is not stable*/
    case 16:
        return "RTCWDT_RTC_RESET";
        break; /**<16, RTC Watch dog reset digital core and rtc module*/
    default:
        return "NO_MEAN";
    }
}

String BasicAuthHash(const char *username, const char *password)
{
    if (username == NULL || password == NULL)
        return emptyString;

    size_t toencodeLen = strlen(username) + strlen(password) + 1;

    char *toencode = new char[toencodeLen + 1];
    if (toencode == NULL)
    {
        return emptyString;
    }
    char *encoded = new char[base64_encode_expected_len(toencodeLen) + 1];
    if (encoded == NULL)
    {
        delete[] toencode;
        return emptyString;
    }
    sprintf_P(toencode, PSTR("%s:%s"), username, password);
    if (base64_encode_chars(toencode, toencodeLen, encoded) > 0)
    {
        String res = String(encoded);
        delete[] toencode;
        delete[] encoded;
        return res;
    }
    delete[] toencode;
    delete[] encoded;
    return emptyString;
}