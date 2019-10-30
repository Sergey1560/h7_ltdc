#ifndef LTDC_H
#define LTDC_H

#include "stm32h7xx.h"
#include "common_defs.h"


/*
*************************** Timings for TFT display**********************************
*
* HSW = (DISPLAY_HSYNC - 1)
* VSH = (DISPLAY_VSYNC - 1)
* AHBP = (DISPLAY_HSYNC + DISPLAY_HBP - 1)
* AVBP = (DISPLAY_VSYNC + DISPLAY_VBP - 1)
* AAW = (DISPLAY_HEIGHT + DISPLAY_VSYNC + DISPLAY_VBP - 1)
* AAH = (DISPLAY_WIDTH + DISPLAY_HSYNC + DISPLAY_HBP - 1)
* TOTALW = (DISPLAY_HEIGHT + DISPLAY_VSYNC + DISPLAY_VBP + DISPLAY_VFP - 1)
* TOTALH = (DISPLAY_WIDTH + DISPLAY_HSYNC + DISPLAY_HBP + DISPLAY_HFP - 1)
*
*/
#define DISPLAY_WIDTH  800
#define DISPLAY_HEIGHT 480

#define  DISPLAY_HSYNC            ((uint16_t)5)
#define  DISPLAY_VSYNC            ((uint16_t)2)
#define  DISPLAY_HBP              ((uint16_t)46)
#define  DISPLAY_VBP              ((uint16_t)23)

#define  DISPLAY_HFP              ((uint16_t)210)
#define  DISPLAY_VFP              ((uint16_t)22)

#define DISPLAY_HSW         (uint16_t)(DISPLAY_HSYNC - 1)
#define DISPLAY_VSH         (uint16_t)(DISPLAY_VSYNC - 1)
#define DISPLAY_AHBP        (uint16_t)(DISPLAY_HSYNC + DISPLAY_HBP - 1)  
#define DISPLAY_AVBP        (uint16_t)(DISPLAY_VSYNC + DISPLAY_VBP - 1)
#define DISPLAY_AAW         (uint16_t)(DISPLAY_WIDTH + DISPLAY_HSYNC + DISPLAY_HBP - 1)
#define DISPLAY_AAH         (uint16_t)(DISPLAY_HEIGHT + DISPLAY_VSYNC + DISPLAY_VBP - 1)
#define DISPLAY_TOTAL_H     (uint16_t)(DISPLAY_HEIGHT + DISPLAY_VSYNC + DISPLAY_VBP + DISPLAY_VFP - 1)
#define DISPLAY_TOTAL_W     (uint16_t)(DISPLAY_WIDTH + DISPLAY_HSYNC + DISPLAY_HBP + DISPLAY_HFP - 1)

#define LTDC_PIXEL_RGB565   2
#define LTDC_PIXEL_RGB888   1
#define LTDC_PIXEL_ARGB888  0



void ltdc_init(void);
void ltdc_gpio_init(void);
void ltdc_show(LTDC_Layer_TypeDef * layer,uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,uint32_t *data,uint8_t alpha,uint8_t pixel);




#endif



/*LTDC GPIO Configuration    
PE4     ------> LTDC_B0
PE5     ------> LTDC_G0
PE6     ------> LTDC_G1

PF10     ------> LTDC_DE

PA2     ------> LTDC_R1

PH8     ------> LTDC_R2
PH9     ------> LTDC_R3
PH10     ------> LTDC_R4
PH11     ------> LTDC_R5
PH12     ------> LTDC_R6
PH13     ------> LTDC_G2
PH14     ------> LTDC_G3
PH15     ------> LTDC_G4

PG6     ------> LTDC_R7
PG7     ------> LTDC_CLK
PG10    ------> LTDC_B2
PG11     ------> LTDC_B3
PG12     ------> LTDC_B1
PG13     ------> LTDC_R0

PI0     ------> LTDC_G5
PI1     ------> LTDC_G6
PI2     ------> LTDC_G7
PI4     ------> LTDC_B4
PI5     ------> LTDC_B5 
PI6     ------> LTDC_B6 
PI7     ------> LTDC_B7 
PI9     ------> LTDC_VSYNC
PI10     ------> LTDC_HSYNC
*/
