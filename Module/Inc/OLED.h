#ifndef __OLED_H
#define __OLED_H


#include "main.h"

#define SSD1306_ADDRESS 0X78

/*FontSize参数取值*/
/*此参数值不仅用于判断，而且用于计算横向字符偏移，默认值为字体像素宽度*/
#define OLED_8X16				8
#define OLED_6X8				6


void OLED_Init(void);
void OLED_Clear(void);
void OLED_WriteCommand(uint8_t Command);
void OLED_WriteData(uint8_t *Data, uint8_t count);
void OLED_WriteSingleData(uint8_t Data);
void OLED_SetCursor(uint16_t Page, uint16_t X);
void OLED_ShowImage(uint16_t X, uint16_t Y, uint8_t Width, uint8_t Height, const uint8_t *Image);
void OLED_ClearArea(int16_t X, int16_t Y, uint8_t Width, uint8_t Height);
void OLED_ShowChar(int16_t X, int16_t Y, char Char);
void OLED_ShowSpecifiedChar(int16_t X, int16_t Y, char Char, uint8_t FontSize);
void OLED_ShowString(int16_t X, int16_t Y, char *String);
void OLED_ShowSpecifiedString(int16_t X, int16_t Y, char *String, uint8_t FontSize);
void OLED_ShowNum(int16_t X, int16_t Y, uint32_t Number, uint8_t Length);
void OLED_ShowSpecifiedNum(int16_t X, int16_t Y, uint32_t Number, uint8_t Length, uint8_t FontSize);
void OLED_ShowSignedNum(int16_t X, int16_t Y, int32_t Number, uint8_t Length);
void OLED_ShowSpecifiedSignedNum(int16_t X, int16_t Y, int32_t Number, uint8_t Length, uint8_t FontSize);
void OLED_ShowHexNum(int16_t X, int16_t Y, uint32_t Number, uint8_t Length);
void OLED_ShowSpecifiedHexNum(int16_t X, int16_t Y, uint32_t Number, uint8_t Length, uint8_t FontSize);
void OLED_ShowFloatNum(int16_t X, int16_t Y, double Number, uint8_t IntLength, uint8_t FraLength);
void OLED_ShowSignifiedFloatNum(int16_t X, int16_t Y, double Number, uint8_t IntLength, uint8_t FraLength, uint8_t FontSize);

#endif
