#include "cube.h"

const static uint8_t icons_8x8[][8] PROGMEM = 
{
    {0x06, 0x09, 0x09, 0x06, 0x78, 0x84, 0x84, 0x48}, //86 celsius
    {0x06, 0x09, 0x09, 0x06, 0x08, 0xfc, 0x88, 0xc0}, //87 temperature
    {0x04, 0x0c, 0x1c, 0x3c, 0x3c, 0x1c, 0x0c, 0x04}, //147 arrow type 1 down
    {0x20, 0x30, 0x38, 0x3c, 0x3c, 0x38, 0x30, 0x20}, //148 arrow type 1 up
    {0x10, 0x30, 0x70, 0xff, 0xff, 0x70, 0x30, 0x10}, //151 arrow type 2 down
    {0x08, 0x0c, 0x0e, 0xff, 0xff, 0x0e, 0x0c, 0x08}, //152 arrow type 2 up
};

const static uint8_t icons_7x7[][7] PROGMEM = 
{
    {0x02, 0x05, 0x02, 0x38, 0x44, 0x44, 0x28}, //86 celsius
    {0x02, 0x05, 0x02, 0x08, 0x7c, 0x48, 0x60}, //87 temperature
    {0x04, 0x0c, 0x1c, 0x3c, 0x1c, 0x0c, 0x04}, //147 arrow type 1 down
    {0x10, 0x18, 0x1c, 0x1e, 0x1c, 0x18, 0x10}, //148 arrow type 1 up
    {0x08, 0x18, 0x38, 0x7f, 0x38, 0x18, 0x08}, //151 arrow type 2 down
    {0x08, 0x0c, 0x0e, 0x7f, 0x0e, 0x0c, 0x08}, //152 arrow type 2 up
};

void drawIcon7x7(byte index) {
  size_t s = sizeof icons_7x7[index];//можно так, а можно просто 7 
  for(unsigned int i = 0; i < s; i++) {
    oled.drawByte(pgm_read_byte(&(icons_7x7[index][i])));
  }
}

void drawIcon8x8(byte index) {
  size_t s = sizeof icons_8x8[index];//можна так, а можна просто 8 
  for(unsigned int i = 0; i < s; i++) {
    oled.drawByte(pgm_read_byte(&(icons_8x8[index][i])));
  }
}

void initOLED(void)
{
  oled.init();
  oled.flipH(true);
  oled.flipV(true);
  oled.clear();   // очистить дисплей (или буфер)
  oled.update();
}

void printTemp(float currentTemp, float targetTemp, int8_t xOffset = 0)
{
  int16_t newCurrentTemp = static_cast<int16_t>(round(currentTemp * 10.0f));
  int16_t newTargetTemp  = static_cast<int16_t>(round(targetTemp  * 10.0f));
  oled.setCursor(25 + xOffset, 2);
  oled.print(F("Температура:"));
  oled.setCursor(10 + xOffset, 4);
  oled.print(F("Текущая   ")); oled.print(currentTemp, 1); oled.setCursor(94 + xOffset, 4); drawIcon8x8(0);

  if(newCurrentTemp < newTargetTemp)
  {
    oled.setCursor(110 + xOffset, 4); 
    oled.print(F("  "));
    oled.setCursor(110 + xOffset, 4); 
    drawIcon8x8(5);
  }
  else if (newCurrentTemp > newTargetTemp)
  {  
    oled.setCursor(110 + xOffset, 4); 
    oled.print(F("  "));
    oled.setCursor(110 + xOffset, 4); 
    drawIcon8x8(4);
  }
  else 
  {
    oled.setCursor(110 + xOffset, 4); 
    oled.print(F("  "));
  }

  oled.setCursor(10 + xOffset, 6);
  oled.print(F("Целевая   ")); oled.print(targetTemp, 1); oled.setCursor(94 + xOffset, 6); drawIcon8x8(0);
  oled.update();
}

void tickCube(uint8_t x, uint8_t y)
{
  static uint8_t frameNum = 0;
  oled.drawBitmap(x, y, cube_bitmap_allArray[frameNum], 20, 20, 0, BUF_REPLACE);
  oled.update();
  frameNum++;
  if (frameNum > 9) frameNum = 0;
}