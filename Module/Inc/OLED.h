#ifndef __OLED_H
#define __OLED_H


#include "main.h"

#define SSD1306_ADDRESS 0X78


void OLED_Init(void);
void OLED_Clear(void);
void OLED_WriteCommand(uint8_t Command);
void OLED_WriteData(uint8_t *Data, uint8_t count);
void OLED_WriteSingleData(uint8_t Data);
void OLED_SetCursor(uint16_t Page, uint16_t X);
void OLED_ShowImage(uint16_t X, uint16_t Y, uint8_t Width, uint8_t Height, const uint8_t *Image);
void OLED_ClearArea(int16_t X, int16_t Y, uint8_t Width, uint8_t Height);

#endif
