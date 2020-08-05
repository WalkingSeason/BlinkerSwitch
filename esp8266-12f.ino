#include <WString.h>

//eeprom
const uint16_t EEPROM_SSID_ADDR = 0; //0-63
const uint16_t EEPROM_SSID_LEN = 64;
const uint16_t EEPRIM_PSK_ADDR = 64; //64-127
const uint16_t EEPROM_PSK_LEN = 64;
const uint16_t EEPROM_NTPCLINET_ADDR = 128; //128-255
const uint16_t EEPROM_NTPCLIENT_LEN = 128;
const uint16_t EEPROM_BLINKER_AUTH_ADDR = 256; //256-271
const uint16_t EEPROM_BLINKER_AUTH_LEN = 16;

//引脚定义
const uint8_t SWITCH_1 = 16;
uint8_t switch1State = LOW;

#include "util/astreamwait.hpp"
#include "util/aeeprom.hpp"

#include "initssd1306.hpp"
#include "initwifi.hpp"
#include "initntp.hpp"
#include "initblinker.hpp"

Adafruit_SSD1306 *ssd1306;
NTPClient *timeClient;

NTPClient *changeNTPClientSerial()
{
    String s;
    Serial.println("Enter the new NTP server address: ");
    eepromWriteString(EEPROM_NTPCLINET_ADDR, EEPROM_NTPCLIENT_LEN,(s = waitStringShow(Serial)));
    return setupNTPClient(Serial, s);
}
void changeIoTKey()
{
    String s,ssid,psk;
    Serial.println("Enter the Alibaba Cloud IoT key: ");
    eepromWriteString(EEPROM_BLINKER_AUTH_ADDR, EEPROM_BLINKER_AUTH_LEN, (s = waitStringShow(Serial)));
    setupBlinker(Serial, s, (ssid = WiFi.SSID()), (psk = WiFi.psk()));
}
void updateSSD1306()
{
    ssd1306->clearDisplay();
    ssd1306->setCursor(0, 0);
    ssd1306->print("SSID:");
    ssd1306->println(WiFi.SSID());
    ssd1306->print("IP:");
    ssd1306->println(WiFi.localIP());
    ssd1306->println(timeClient != nullptr ? timeClient->getFormattedTime() : "Time asynchronous.");
    ssd1306->print("Switch:");
    ssd1306->println(switch1State ? "on" : "off");
    ssd1306->display();
}

void setup()
{
    String s, ssid, psk;
    pinMode(SWITCH_1, OUTPUT);
    digitalWrite(SWITCH_1, switch1State);
    Serial.begin(74880);
    ssd1306 = setupDisplay();
    setupWiFi(Serial);
    timeClient = setupNTPClient(Serial, (s = eepromReadString(EEPROM_NTPCLINET_ADDR, EEPROM_NTPCLIENT_LEN)));
    setupBlinker(Serial, (s = eepromReadString(EEPROM_BLINKER_AUTH_ADDR, EEPROM_BLINKER_AUTH_LEN)), (ssid = WiFi.SSID()), (psk = WiFi.psk()));
}

void loop()
{
    if (Serial.available())
    {
        clearBuffer(Serial);
        Serial.println("1. Change NTP server address");
        Serial.println("2. Change Alibaba Cloud IoT key");
        switch (waitString(Serial).toInt())
        {
        case 1:
            timeClient = changeNTPClientSerial();
            break;
        case 2:
            changeIoTKey();
            break;
        }
    }
    Blinker.run();
    updateSSD1306();
    delay(50);
}