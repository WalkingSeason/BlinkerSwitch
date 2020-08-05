#ifndef _SPI_H_INCLUDED
#include <SPI.h>
#endif
#ifndef TwoWire_h
#include <Wire.h>
#endif
#ifndef _ADAFRUIT_GFX_H
#include <Adafruit_GFX.h>
#endif
#ifndef _Adafruit_SSD1306_H_
#include <Adafruit_SSD1306.h>
#endif
#include "db/matrixlcd.hpp"
extern PROGMEM const uint8_t LOGO[];
struct dispdata
{
    int8_t RST = -1;
    uint8_t IIC_BEGIN = 0x3c;
    uint8_t DY = 64;
    uint16_t DX = 128;
} const DISP;
Adafruit_SSD1306* setupDisplay()
{
    Adafruit_SSD1306 *ssd1306=new Adafruit_SSD1306(DISP.DX, DISP.DY, &Wire, DISP.RST);
    ssd1306->begin(SSD1306_SWITCHCAPVCC, DISP.IIC_BEGIN);
    ssd1306->setTextWrap(false);
    ssd1306->setTextSize(1);
    ssd1306->setTextColor(WHITE);
    ssd1306->setCursor(0, 0);
    ssd1306->clearDisplay();
    ssd1306->drawBitmap(0, 0, LOGO, 128, 64, WHITE);
    ssd1306->display();
    ssd1306->clearDisplay();
    ssd1306->setCursor(0, 0);
    return ssd1306;
}