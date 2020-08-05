#ifndef Stream_h
#include <Stream.h>
#endif
#ifndef String_class_h
#include <WString.h>
#endif
#define BLINKER_WIFI
#include <Blinker.h>
BlinkerButton *Button1;
extern const uint8_t SWITCH_1;
extern uint8_t switch1State;
void button1_callback(const String &state)
{
    digitalWrite(SWITCH_1, !digitalRead(SWITCH_1));
    BLINKER_LOG("get button state: ", state);
    if (state == BLINKER_CMD_BUTTON_TAP)
    {
        if (!switch1State)
        {
            BLINKER_LOG("Toggle on!");
            digitalWrite(SWITCH_1, (switch1State = HIGH));
            Button1->color("#7cb342");
            Button1->text("开");
            Button1->print("on");
        }
        else
        {
            BLINKER_LOG("Toggle off!");
            digitalWrite(SWITCH_1, (switch1State = LOW));
            Button1->color("#000000");
            Button1->text("关");
            Button1->print("off");
        }
    }
}
void dataRead(const String &data)
{
    BLINKER_LOG("Blinker readString: ", data);
    Blinker.vibrate();
    uint32_t BlinkerTime = millis();
    Blinker.print("millis", BlinkerTime);
}
void setupBlinker(Stream &stream, String &blinkerAuth, String &ssid, String &password)
{
    if (Button1 == nullptr)
        Button1 = new BlinkerButton("Button1");
    BLINKER_DEBUG.stream(stream);
    Blinker.begin(&(blinkerAuth[0]), &(ssid[0]), &(password[0]));
    Blinker.attachData(dataRead);
    Button1->attach(button1_callback);
}