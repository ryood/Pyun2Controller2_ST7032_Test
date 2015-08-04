/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include <project.h>
#include <stdio.h>
#include "ST7032.h"

/* ADC channels */
#define ADC_CH_POT1_N         (0x00u)
#define ADC_CH_POT2_N         (0x01u)
#define ADC_CH_POT3_N         (0x02u)
#define ADC_CH_JOY1_N         (0x03u)
#define ADC_CH_JOY2_N         (0x04u)

/* ADC limits */
#define ADC_LOW_LIMIT   ((int16)0x000)
#define ADC_HIGH_LIMIT  ((int16)0x7FF)

/* ADC limits trim */
#define ADC_LIMIT(x) \
    ((x)<ADC_LOW_LIMIT?ADC_LOW_LIMIT:((x)>=ADC_HIGH_LIMIT?ADC_HIGH_LIMIT:(x)))
    
/* 入力デバイス用変数 */                
int16 adcResult[ADC_SAR_Seq_TOTAL_CHANNELS_NUM];

struct ButtonStatus {
	uint8 all;
	uint8 green:  1;
	uint8 yellow: 1;
	uint8 red:    1;
	uint8 blue:   1;
	uint8 black:  1;
	uint8 white:  1;
};

struct ButtonStatus buttonStatus;
struct ButtonStatus prevButtonStatus;

/*======================================================
 * 入力処理 
 *
 *======================================================*/
// ADC
void pollingADC()
{
    ADC_SAR_Seq_StartConvert();
    while (ADC_SAR_Seq_IsEndConversion(ADC_SAR_Seq_RETURN_STATUS) == 0u) {
        // 変換終了を待つ
        ;
    }
    adcResult[ADC_CH_POT1_N] = ADC_LIMIT(ADC_SAR_Seq_GetResult16(ADC_CH_POT1_N));
    adcResult[ADC_CH_POT2_N] = ADC_LIMIT(ADC_SAR_Seq_GetResult16(ADC_CH_POT2_N));
    adcResult[ADC_CH_POT3_N] = ADC_LIMIT(ADC_SAR_Seq_GetResult16(ADC_CH_POT3_N));
    adcResult[ADC_CH_JOY1_N] = ADC_LIMIT(ADC_SAR_Seq_GetResult16(ADC_CH_JOY1_N));
    adcResult[ADC_CH_JOY2_N] = ADC_LIMIT(ADC_SAR_Seq_GetResult16(ADC_CH_JOY2_N));
}

// Buttons
void pollingSW()
{
    buttonStatus.green  = SW1_D_Read(); 
    buttonStatus.yellow = SW2_D_Read(); 
    buttonStatus.red    = SW3_D_Read(); 
    buttonStatus.blue   = SW4_D_Read(); 
    buttonStatus.black  = SW5_D_Read(); 
    buttonStatus.white  = SW6_D_Read(); 
    
    prevButtonStatus = buttonStatus;    
}

/*======================================================
 * メインルーチン 
 *
 *======================================================*/
int main()
{
    char LCD_LINE[17] = "";
    
    /* Init and start sequencing SAR ADC */
    ADC_SAR_Seq_Start();
    ADC_SAR_Seq_StartConvert();
    
    /* Init and start I2C master */
    I2CM_Start();
	CyDelay(500);
    
	CyGlobalIntEnable;
	
    /* Start I2C LCD */
    // LCDをRESET
	LCD_RESET_Write(0u);
	CyDelay(1);
	LCD_RESET_Write(1u);
	CyDelay(10);
	
	LCD_Init(32);
    
    LCD_Clear();
	LCD_Puts("Pyun2Controller2");
    LCD_SetPos(0, 1);
	LCD_Puts("Demonstration");
    CyDelay(500);

    for(;;)
    {
        pollingADC();        
        pollingSW();
        
#ifdef _ADC_8BIT
        sprintf(LCD_LINE, "%02x %02x %02x : %02x %02x",
            adcResult[ADC_CH_POT1_N] >> 3,
            adcResult[ADC_CH_POT2_N] >> 3,
            adcResult[ADC_CH_POT3_N] >> 3,
            adcResult[ADC_CH_JOY1_N] >> 3,
            adcResult[ADC_CH_JOY2_N] >> 3
        );
#else
        sprintf(LCD_LINE, "%03x%03x%03x:%03x%03x",
            adcResult[ADC_CH_POT1_N],
            adcResult[ADC_CH_POT2_N],
            adcResult[ADC_CH_POT3_N],
            adcResult[ADC_CH_JOY1_N],
            adcResult[ADC_CH_JOY2_N]
        );
#endif        

        LCD_SetPos(0, 0);
        LCD_Puts(LCD_LINE);
        
        sprintf(LCD_LINE, "%s %s %s %s : %s %s   ",
            buttonStatus.green ? "G" : "x",
            buttonStatus.yellow ? "Y" : "x",
            buttonStatus.red ? "R" : "x",
            buttonStatus.blue ? "B" : "x",
            buttonStatus.black ? "K" : "x",
            buttonStatus.white ? "W" : "x"
        );            
            
        LCD_SetPos(0, 1);
        LCD_Puts(LCD_LINE);        
        
        //CyDelay(1000);
    }
}

/* [] END OF FILE */
