#ifndef WIFIUDP_H
#include <WiFiUdp.h>
#endif
#ifndef Stream_h
#include <Stream.h>
#endif
#ifndef String_class_h
#include <WString.h>
#endif
#include <NTPClient.h>
NTPClient *setupNTPClient(Stream &stream,String &ntpClient)
{
    WiFiUDP *ntpUDP = new WiFiUDP();
    NTPClient *timeClient = new NTPClient(*ntpUDP, &(ntpClient[0]), 60 * 60 * 8 /*UTC+8*/, 1000 * 60 * 5 /*update every 5 min*/);
    timeClient->begin();
    delay(500);
    if (timeClient->update())
    {
        stream.println("Success get NTPClient time:" + timeClient->getFormattedTime());
        return timeClient;
    }
    else
    {
        stream.println("failed get NTPClient time.");
    }
    delete ntpUDP;
    delete timeClient;
    return nullptr;
}