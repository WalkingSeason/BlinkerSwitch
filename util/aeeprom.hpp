#ifndef EEPROM_h
#include <EEPROM.h>
#endif
#ifndef String_class_h
#include <WString.h>
#endif
String eepromReadString(int addr, int length)
{
    EEPROM.begin(addr + length);
    String s = "";
    while (EEPROM.read(addr) != '\0')
    {
        s += (char)EEPROM.read(addr++);
    }
    EEPROM.end();
    return s;
}
void eepromWriteString(int addr, int length, String &s)
{
    EEPROM.begin(addr + length);
    unsigned int i = 0;
    while (s[i] != '\0')
    {
        EEPROM.write(addr++, s[i++]);
    }
    EEPROM.write(addr, s[i]);
    EEPROM.end();
}