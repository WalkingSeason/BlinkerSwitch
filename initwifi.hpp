#ifndef String_class_h
#include <WString.h>
#endif
#ifndef Stream_h
#include <Stream.h>
#endif
#ifndef WiFi_h
#include <ESP8266WiFi.h>
#endif
#include "util/aeeprom.hpp"
#include "util/astreamwait.hpp"
extern const uint16_t EEPROM_SSID_ADDR;
extern const uint16_t EEPROM_SSID_LEN;
extern const uint16_t EEPRIM_PSK_ADDR;
extern const uint16_t EEPROM_PSK_LEN;
void setupWiFi(Stream &stream)
{
    //Reset WiFi Mode
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();

    //read inf from EEPROM
    String *ssid = new String();
    String *password = new String();
    *ssid = eepromReadString(EEPROM_SSID_ADDR, EEPROM_SSID_LEN);
    *password = eepromReadString(EEPROM_PSK_ADDR, EEPROM_PSK_LEN);

    //try connect to WiFi
    WiFi.begin(*ssid, *password);
    stream.print("\n\nConnecting to ");
    stream.println(*ssid);
    for (uint16_t i = 1; WiFi.status() != WL_CONNECTED; ++i)
    {
        delay(100);
        if (!(i % 10))
        {
            stream.print(".");
        }
        if (stream.available())
        {
            WiFi.disconnect();
            clearBuffer(stream);
            stream.println("\nscan start");
            int n = WiFi.scanNetworks();
            stream.println("scan done");
            if (n == 0)
            {
                stream.println("no networks found");
            }
            else
            {
                stream.print(n);
                stream.println(" networks found");
                for (int i = 0; i < n; ++i)
                {
                    // Print SSID and RSSI for each network found
                    stream.print(i + 1);
                    stream.print(": ");
                    stream.print(WiFi.SSID(i));
                    stream.print(" (");
                    stream.print(WiFi.RSSI(i));
                    stream.print(")");
                    stream.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
                    delay(10);
                }
            }
            stream.println("");
            stream.println("Choose network number:");
            String s;
            *ssid = WiFi.SSID(waitString(stream).toInt() - 1);
            eepromWriteString(eepromSSIDAddr, 64, *ssid);
            stream.print("Enter the password for ");
            stream.println(*ssid);
            *password = waitString(stream);
            eepromWriteString(eepromPasswordAddr, 64, *password);
            WiFi.begin(*ssid, *password);
        }
    }
    stream.println("");
    stream.print("WiFi connected:");
    stream.println(WiFi.SSID());
    stream.print("IP address: ");
    stream.println(WiFi.localIP());
}