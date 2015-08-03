#ifndef INCLUDED_ST7032_H_
#define INCLUDED_ST7032_H_

/*
    include元で
    I2C masterコンポーネントを「I2C_M」という名前で作成してください。
*/
    
#include <project.h>

/* contrast: LCDのコントラストの設定 [0.. 63] */
void LCD_Init(uint8 contrast);
uint32 LCD_Write(uint8 *buffer);
void LCD_Cmd(uint8 cmd);
void LCD_Data(uint8 data);
void LCD_Clear();
void LCD_SetPos(uint32 x, uint32 y);
void LCD_Puts(char8 *s);

#endif // INCLUDED_ST7032_H_