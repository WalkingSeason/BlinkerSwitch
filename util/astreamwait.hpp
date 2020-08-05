#ifndef String_class_h
#include <WString.h>
#endif
#ifndef Stream_h
#include <Stream.h>
#endif
String waitString(Stream &stream)
{
    String s;
    while (1)
    {
        if (stream.available())
        {
            s = stream.readString();
            break;
        }
    }
    return s;
}
String waitStringShow(Stream &stream)
{
    String s;
    while (1)
    {
        if (stream.available())
        {
            s = stream.readString();
            break;
        }
    }
    stream.println(s);
    return s;
}
void clearBuffer(Stream &stream)
{
    while (stream.available())
        stream.read();
}