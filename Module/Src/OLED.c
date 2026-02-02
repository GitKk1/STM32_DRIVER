#include "OLED.h"
#include "i2c.h"

uint8_t OLED_DisplayBuf[8][128];

/**
 * 函    数：OLED写命令
 * 参    数：Command 要写入的命令值，范围：0x00~0xFF
 * 返 回 值：无
 */
void OLED_WriteCommand(uint8_t Command)
{
	uint8_t comd_array[2] = {0};
	comd_array[1] = Command;
	HAL_I2C_Master_Transmit(&hi2c1, SSD1306_ADDRESS, comd_array, 2, HAL_MAX_DELAY);
}

/**
 * 函    数：OLED写指定长度的数据
 * 参    数：Data 要写入数据的起始地址
 * 参    数：Count 要写入数据的数量
 * 返 回 值：无
 */
void OLED_WriteData(uint8_t *Data, uint8_t count)
{
	uint8_t data_array[2] = {0};
	data_array[0] = 0x40;
	for (uint8_t a = 0; a < count; a++)
	{
		data_array[1] = Data[a];
		HAL_I2C_Master_Transmit(&hi2c1, SSD1306_ADDRESS, data_array, 2, HAL_MAX_DELAY);
	}
}

/**
 * 函    数：OLED写一个字节数据
 * 参    数：Data 要写入数据
 * 返 回 值：无
 */
void OLED_WriteSingleData(uint8_t Data)
{
	uint8_t data_array[2] = {0};
	data_array[0] = 0x40;
	data_array[1] = Data;
	HAL_I2C_Master_Transmit(&hi2c1, SSD1306_ADDRESS, data_array, 2, HAL_MAX_DELAY);
}

/**
 * 函    数：OLED初始化
 * 参    数：无
 * 返 回 值：无
 * 说    明：使用前，需要调用此初始化函数
 */
void OLED_Init(void)
{
	HAL_Delay(100);
	/*写入一系列的命令，对OLED进行初始化配置*/
	OLED_WriteCommand(0xAE); // 设置显示开启/关闭，0xAE关闭，0xAF开启

	OLED_WriteCommand(0xD5); // 设置显示时钟分频比/振荡器频率
	OLED_WriteCommand(0x80); // 0x00~0xFF

	OLED_WriteCommand(0xA8); // 设置多路复用率
	OLED_WriteCommand(0x3F); // 0x0E~0x3F

	OLED_WriteCommand(0xD3); // 设置显示偏移
	OLED_WriteCommand(0x00); // 0x00~0x7F

	OLED_WriteCommand(0x40); // 设置显示开始行，0x40~0x7F

	OLED_WriteCommand(0xA1); // 设置左右方向，0xA1正常，0xA0左右反置

	OLED_WriteCommand(0xC8); // 设置上下方向，0xC8正常，0xC0上下反置

	OLED_WriteCommand(0xDA); // 设置COM引脚硬件配置
	OLED_WriteCommand(0x12);

	OLED_WriteCommand(0x81); // 设置对比度
	OLED_WriteCommand(0xCF); // 0x00~0xFF

	OLED_WriteCommand(0xD9); // 设置预充电周期
	OLED_WriteCommand(0xF1);

	OLED_WriteCommand(0xDB); // 设置VCOMH取消选择级别
	OLED_WriteCommand(0x30);

	OLED_WriteCommand(0xA4); // 设置整个显示打开/关闭

	OLED_WriteCommand(0xA6); // 设置正常/反色显示，0xA6正常，0xA7反色

	OLED_WriteCommand(0x8D); // 设置充电泵
	OLED_WriteCommand(0x14);

	OLED_WriteCommand(0xAF); // 开启显示
	HAL_Delay(100);
	OLED_Clear();
}

/**
 * 函    数：OLED设置字符显示位置
 * 参    数：Page 指定光标所在的页，范围：0~7
 * 参    数：X 指定光标所在的X轴坐标，范围：0~127
 * 返 回 值：无
 * 说    明：OLED默认的Y轴，只能8个Bit为一组写入，即1页等于8个Y轴坐标
 */
void OLED_SetCursor(uint16_t Page, uint16_t X)
{
	/*通过指令设置页地址和列地址*/
	OLED_WriteCommand(0xB0 | (Page));			 // 设置页位置
	OLED_WriteCommand(0x10 | ((X & 0xF0) >> 4)); // 设置X位置高4位
	OLED_WriteCommand(0x00 | (X & 0x0F));		 // 设置X位置低4位
}

/**
 * 函    数：清除屏幕显示
 * 参    数：无
 * 返 回 值：无
 * 说    明：无
 */
void OLED_Clear(void)
{
	uint8_t a, b;
	for (a = 0; a <= 7; a++)
	{
		for (b = 0; b <= 127; b++)
		{
			OLED_SetCursor(a, b);
			OLED_WriteSingleData(0x00);
		}
	}
}

/**
 * 函    数：清除指定OLED区域显示
 * 参    数：X 指定区域左上角的横坐标，范围：-32768~32767，屏幕区域：1~128
 * 参    数：Y 指定区域左上角的纵坐标，范围：-32768~32767，屏幕区域：1~64
 * 参    数：Width 指定区域的宽度，范围：0~128
 * 参    数：Height 指定区域的高度，范围：0~64
 * 返 回 值：无
 * 说    明：调用此函数后，要想真正地呈现在屏幕上，还需调用更新函数
 */
void OLED_ClearArea(int16_t X, int16_t Y, uint8_t Width, uint8_t Height)
{
	int16_t a, b;
	int8_t Page, Last_Page, Shift;

	Page = (Y - 1) / 8;						   // 计算起始页
	Shift = (Y - 1) % 8;					   // 计算偏移量
	for (b = 0; b < (Height - 1) / 8 + 1; b++) /*遍历指定图像涉及的相关页*/
	{
		for (a = 0; a < Width; a++) /*遍历指定图像涉及的相关列*/
		{
			if (X + a >= 0 && X + a <= 127 && Page + b >= 0 && Page + b <= 7) // 超出屏幕的内容不显示
			{
				if (Shift == 0)//没有偏移量清除现存
				{
					OLED_DisplayBuf[Page + b][X + a] = 0x00;
				}
				else//有偏移量清除现存
				{
					if (b == 0)
						OLED_DisplayBuf[Page + b][X + a] &= 0xFF >> (Shift);
					else if (b > 0 && b < ((Height - 1) / 8))
						OLED_DisplayBuf[Page + b][X + a] &= 0x00;
					else
						OLED_DisplayBuf[Page + b][X + a] &= 0xFF << (Shift);
				}
			}
		}
	}

	Page = (Y - 1) / 8;						  // 首页
	Last_Page = (Y - 1 + Height - 1) / 8 + 1; // 计算尾页
	for (a = Page; a < Last_Page; a++)		  /*遍历指定区域涉及的相关页*/
	{
		/*设置光标位置为相关页的指定列*/
		OLED_SetCursor(a, X - 1);
		/*连续写入Width个数据，将显存数组的数据写入到OLED硬件*/
		OLED_WriteData(&OLED_DisplayBuf[a][X], Width);
	}
}

/**
 * 函    数：OLED显示图像
 * 参    数：X 指定图像左上角的横坐标，范围：-32768~32767，屏幕区域：1~128
 * 参    数：Y 指定图像左上角的纵坐标，范围：-32768~32767，屏幕区域：1~64
 * 参    数：Width 指定图像的宽度，范围：0~128
 * 参    数：Height 指定图像的高度，范围：0~64
 * 参    数：Image 指定要显示的图像
 * 返 回 值：无
 * 说    明：调用此函数后，要想真正地呈现在屏幕上，还需调用更新函数
 */
void OLED_ShowImage(uint16_t X, uint16_t Y, uint8_t Width, uint8_t Height, const uint8_t *Image)
{
	uint16_t a = 0, b = 0;
	int8_t Page, Last_Page, Shift;
	if ((X <= 0) || (Y <= 0)) // 错误显示范围退出
	{
		return;
	}
	/*将图像所在区域清空*/
	// OLED_ClearArea(X, Y, Width, Height);

	/*(Height - 1) / 8 + 1的目的是Height / 8并向上取整*/
	for (b = 0; b < (Height - 1) / 8 + 1; b++) /*遍历指定图像涉及的相关页*/
	{
		/*遍历指定图像涉及的相关列*/
		for (a = 0; a < Width; a++)
		{
			if (X + a >= 0 && X + a <= 127) // 超出屏幕的内容不显示
			{
				Page = (Y - 1) / 8;
				Shift = (Y - 1) % 8;

				if (Page + b >= 0 && Page + b <= 7) // 超出屏幕的内容不显示
				{
					/*显示图像在当前页的内容*/
					OLED_DisplayBuf[Page + b][X + a] |= Image[b * Width + a] << (Shift);
				}

				if (Page + b + 1 >= 0 && Page + b + 1 <= 7) // 超出屏幕的内容不显示
				{
					/*显示图像在下一页的内容*/
					OLED_DisplayBuf[Page + b + 1][X + a] |= Image[b * Width + a] >> (8 - Shift);
				}
			}
		}
	}
	Page = (Y - 1) / 8;
	Last_Page = (Y - 1 + Height - 1) / 8 + 1; // 末页计算
	/*遍历指定区域涉及的相关页*/
	for (b = Page; b < Last_Page; b++)
	{
		/*设置光标位置为相关页的指定列*/
		OLED_SetCursor(b, X - 1);
		/*连续写入Width个数据，将显存数组的数据写入到OLED硬件*/
		OLED_WriteData(&OLED_DisplayBuf[b][X], Width);
	}
}
