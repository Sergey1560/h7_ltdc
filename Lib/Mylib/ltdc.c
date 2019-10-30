#include "ltdc.h"

void ltdc_show(LTDC_Layer_TypeDef * layer,uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,uint32_t *data,uint8_t alpha,uint8_t pixel){
    uint16_t width,height;
    uint8_t pixel_width;

    if(x1 > x0){
        width=x1-x0;
    }else{
      return;
    }

    if(y1 > y0){
        height=y1-y0;
    }else{
      return;
    }

    switch (pixel)
    {
    case LTDC_PIXEL_ARGB888:
        pixel_width=4;
        break;
    case LTDC_PIXEL_RGB888:
        pixel_width=3;
        break;
    case LTDC_PIXEL_RGB565:
        pixel_width=2;
        break;
    
    default:
        return;
        break;
    }

    
    layer->WHPCR = (((x1 + DISPLAY_AHBP - 1) << LTDC_LxWHPCR_WHSPPOS_Pos) | ((x0+DISPLAY_AHBP) << LTDC_LxWHPCR_WHSTPOS_Pos));
    layer->WVPCR = (((y1 + DISPLAY_AVBP - 1) << LTDC_LxWVPCR_WVSPPOS_Pos) |((y0+DISPLAY_AVBP) << LTDC_LxWVPCR_WVSTPOS_Pos));
    layer->CFBAR = (uint32_t)data;
    layer->CACR = alpha;
    //Формат пикселя
    layer->PFCR = (pixel << LTDC_LxPFCR_PF_Pos);
    
    layer->CFBLR = (((width * pixel_width) << LTDC_LxCFBLR_CFBP_Pos) | ((width * pixel_width  + 3)  << LTDC_LxCFBLR_CFBLL_Pos));
    layer->CFBLNR = height;	
    __DSB();
    layer->CR |= LTDC_LxCR_LEN;
    LTDC->SRCR = LTDC_SRCR_IMR;

}


void ltdc_init(void){
    ltdc_gpio_init();
 
    RCC->APB3ENR |= RCC_APB3ENR_LTDCEN;
    //Horizontal Synchronization Width и Vertical Synchronization Height
    LTDC->SSCR |= (DISPLAY_HSW << LTDC_SSCR_HSW_Pos | (DISPLAY_VSH << LTDC_SSCR_VSH_Pos));
    //Accumulated Horizontal back porch и Accumulated Vertical back porch
    LTDC->BPCR |= (DISPLAY_AHBP << LTDC_BPCR_AHBP_Pos | DISPLAY_AVBP << LTDC_BPCR_AVBP_Pos);
    //Accumulated Active Width и Accumulated Active Height
    LTDC->AWCR |= (DISPLAY_AAH << LTDC_AWCR_AAH_Pos | DISPLAY_AAW << LTDC_AWCR_AAW_Pos);
    //Total Width и Total Height
    LTDC->TWCR |= (DISPLAY_TOTAL_W << LTDC_TWCR_TOTALW_Pos | DISPLAY_TOTAL_H << LTDC_TWCR_TOTALH_Pos);
    //Фон
    LTDC->BCCR = 0xFFFF; 

    LTDC->IER |= LTDC_IER_TERRIE|LTDC_IER_FUIE;

    LTDC->GCR |= LTDC_GCR_LTDCEN;
	NVIC_EnableIRQ(LTDC_ER_IRQn);
	NVIC_SetPriority (LTDC_ER_IRQn, 0);
};


void LTDC_ER_IRQHandler(void){

    //ERROR("LTDC Error, LTDC_ISR = %0X",LTDC->ISR);
    if( (LTDC->ISR & LTDC_ISR_FUIF) > 0){
        LTDC->ICR |= LTDC_ICR_CFUIF;    
    } 

    if( (LTDC->ISR & LTDC_ISR_TERRIF) > 0){
        LTDC->ICR |= LTDC_ICR_CTERRIF;
    } 

}


void ltdc_gpio_init(void){
    GPIO_TypeDef *PORT;


// A,E,F,G,H,I
    RCC->AHB4ENR|=( RCC_AHB4ENR_GPIOAEN|\
                    RCC_AHB4ENR_GPIOEEN|\
                    RCC_AHB4ENR_GPIOFEN|\
                    RCC_AHB4ENR_GPIOGEN|\
                    RCC_AHB4ENR_GPIOHEN|\
                    RCC_AHB4ENR_GPIOIEN);


/****************************************** GPIOA **************************************/
    /* 2 */
    PORT=GPIOA;
    //Сброс бит
    PORT->MODER &= ~(GPIO_MODER_MODER2);
    //Режим AF
    PORT->MODER |= (GPIO_MODER_MODER2_1);
    //Сброс бит в режим OUT_PP
    PORT->OTYPER &= ~(GPIO_OTYPER_OT_2);
    //Сброс бит
    PORT->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR2);
    //Скорость 
    PORT->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR2);
    PORT->OSPEEDR |= (S_VH << GPIO_OSPEEDER_OSPEEDR2_Pos);
    //Подтяжки отключены
    
    PORT->PUPDR &= ~(GPIO_PUPDR_PUPDR2);
    //AF 14
    PORT->AFR[0] &= ~(GPIO_AFRL_AFRL2);
    PORT->AFR[0] |= (14 << GPIO_AFRL_AFRL2_Pos);


/****************************************** GPIOE **************************************/
    /* 4,5,6 */
    PORT=GPIOE;

    //Сброс бит
    PORT->MODER &= ~(GPIO_MODER_MODER4|\
                     GPIO_MODER_MODER5|\
                     GPIO_MODER_MODER6);
    //Режим AF
    PORT->MODER |= (GPIO_MODER_MODER4_1|\
                    GPIO_MODER_MODER5_1|\
                    GPIO_MODER_MODER6_1);
    
    //Сброс бит в режим OUT_PP
    PORT->OTYPER &= ~(GPIO_OTYPER_OT_4|\
                      GPIO_OTYPER_OT_5|\
                      GPIO_OTYPER_OT_6);
    //Сброс бит
    PORT->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR4|\
                       GPIO_OSPEEDER_OSPEEDR5|\
                       GPIO_OSPEEDER_OSPEEDR6);

    //Скорость 
    PORT->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR4|\
                       GPIO_OSPEEDER_OSPEEDR5|\
                       GPIO_OSPEEDER_OSPEEDR6);

    PORT->OSPEEDR |= (S_VH << GPIO_OSPEEDER_OSPEEDR4_Pos)|\
                     (S_VH << GPIO_OSPEEDER_OSPEEDR5_Pos)|\
                     (S_VH << GPIO_OSPEEDER_OSPEEDR6_Pos);

    //Подтяжки отключены
    PORT->PUPDR &= ~(GPIO_PUPDR_PUPDR4|\
                     GPIO_PUPDR_PUPDR5|\
                     GPIO_PUPDR_PUPDR6);

    //AF 14
    PORT->AFR[0] &= ~(GPIO_AFRL_AFRL4|GPIO_AFRL_AFRL5|GPIO_AFRL_AFRL6);
    
    PORT->AFR[0] |= (14 << GPIO_AFRL_AFRL4_Pos)|\
                    (14 << GPIO_AFRL_AFRL5_Pos)|\
                    (14 << GPIO_AFRL_AFRL6_Pos);

/****************************************** GPIOF **************************************/
    /* 10 */
    PORT=GPIOF;
    //Сброс бит
    PORT->MODER &= ~(GPIO_MODER_MODER10);
    //Режим AF
    PORT->MODER |= (GPIO_MODER_MODER10_1);
    //Сброс бит в режим OUT_PP
    PORT->OTYPER &= ~(GPIO_OTYPER_OT_10);
    //Сброс бит
    PORT->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR10);
    //Скорость 
    PORT->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR10);
    PORT->OSPEEDR |= (S_VH << GPIO_OSPEEDER_OSPEEDR10_Pos);
    //Подтяжки отключены
    PORT->PUPDR &= ~(GPIO_PUPDR_PUPDR10);
    //AF 14
    PORT->AFR[1] &= ~(GPIO_AFRH_AFRH10);
    PORT->AFR[1] |= (14 << GPIO_AFRH_AFRH10_Pos);


/****************************************** GPIOH **************************************/
    /* 8,9,10,11,12,13,14,15 */
    PORT=GPIOH;

    //Сброс бит
    PORT->MODER &= ~(GPIO_MODER_MODER8|\
                    GPIO_MODER_MODER9|\
                    GPIO_MODER_MODER10|\
                    GPIO_MODER_MODER11|\
                    GPIO_MODER_MODER12|\
                    GPIO_MODER_MODER13|\
                    GPIO_MODER_MODER14|\
                    GPIO_MODER_MODER15);
    //Режим AF
    PORT->MODER |= (GPIO_MODER_MODER8_1|\
                    GPIO_MODER_MODER9_1|\
                    GPIO_MODER_MODER10_1|\
                    GPIO_MODER_MODER11_1|\
                    GPIO_MODER_MODER12_1|\
                    GPIO_MODER_MODER13_1|\
                    GPIO_MODER_MODER14_1|\
                    GPIO_MODER_MODER15_1);
    
    //Сброс бит в режим OUT_PP
    PORT->OTYPER &= ~(GPIO_OTYPER_OT_8|\
                      GPIO_OTYPER_OT_9|\
                      GPIO_OTYPER_OT_10|\
                      GPIO_OTYPER_OT_11|\
                      GPIO_OTYPER_OT_12|\
                      GPIO_OTYPER_OT_13|\
                      GPIO_OTYPER_OT_14|\
                      GPIO_OTYPER_OT_15);
    //Сброс бит
    PORT->OSPEEDR &= ~( GPIO_OSPEEDER_OSPEEDR8|\
                        GPIO_OSPEEDER_OSPEEDR9|\
                        GPIO_OSPEEDER_OSPEEDR10|\
                        GPIO_OSPEEDER_OSPEEDR11|\
                        GPIO_OSPEEDER_OSPEEDR12|\
                        GPIO_OSPEEDER_OSPEEDR13|\
                        GPIO_OSPEEDER_OSPEEDR14|\
                        GPIO_OSPEEDER_OSPEEDR15);

    //Скорость 
    PORT->OSPEEDR &= ~( GPIO_OSPEEDER_OSPEEDR8|\
                        GPIO_OSPEEDER_OSPEEDR9|\
                        GPIO_OSPEEDER_OSPEEDR10|\
                        GPIO_OSPEEDER_OSPEEDR11|\
                        GPIO_OSPEEDER_OSPEEDR12|\
                        GPIO_OSPEEDER_OSPEEDR13|\
                        GPIO_OSPEEDER_OSPEEDR14|\
                        GPIO_OSPEEDER_OSPEEDR15);

    PORT->OSPEEDR |= (S_VH << GPIO_OSPEEDER_OSPEEDR8_Pos)|\
                     (S_VH << GPIO_OSPEEDER_OSPEEDR9_Pos)|\
                     (S_VH << GPIO_OSPEEDER_OSPEEDR10_Pos)|\
                     (S_VH << GPIO_OSPEEDER_OSPEEDR11_Pos)|\
                     (S_VH << GPIO_OSPEEDER_OSPEEDR12_Pos)|\
                     (S_VH << GPIO_OSPEEDER_OSPEEDR13_Pos)|\
                     (S_VH << GPIO_OSPEEDER_OSPEEDR14_Pos)|\
                     (S_VH << GPIO_OSPEEDER_OSPEEDR15_Pos);

    //Подтяжки отключены
    PORT->PUPDR &= ~(GPIO_PUPDR_PUPDR8|\
                     GPIO_PUPDR_PUPDR9|\
                     GPIO_PUPDR_PUPDR10|\
                     GPIO_PUPDR_PUPDR11|\
                     GPIO_PUPDR_PUPDR12|\
                     GPIO_PUPDR_PUPDR13|\
                     GPIO_PUPDR_PUPDR14|\
                     GPIO_PUPDR_PUPDR15);

    //AF 14
    PORT->AFR[1] &= ~(GPIO_AFRH_AFRH8|GPIO_AFRH_AFRH9|GPIO_AFRH_AFRH10|GPIO_AFRH_AFRH11|GPIO_AFRH_AFRH12|GPIO_AFRH_AFRH13|GPIO_AFRH_AFRH14|GPIO_AFRH_AFRH15);
    
    PORT->AFR[1] |= (14 << GPIO_AFRH_AFRH8_Pos)|\
                    (14 << GPIO_AFRH_AFRH9_Pos)|\
                    (14 << GPIO_AFRH_AFRH10_Pos)|\
                    (14 << GPIO_AFRH_AFRH11_Pos)|\
                    (14 << GPIO_AFRH_AFRH12_Pos)|\
                    (14 << GPIO_AFRH_AFRH13_Pos)|\
                    (14 << GPIO_AFRH_AFRH14_Pos)|\
                    (14 << GPIO_AFRH_AFRH15_Pos);

/****************************************** GPIOG **************************************/
    /* 6,7,10,11,12,13 */
    PORT=GPIOG;

    //Сброс бит
    PORT->MODER &= ~(GPIO_MODER_MODER6|\
                    GPIO_MODER_MODER7|\
                    GPIO_MODER_MODER10|\
                    GPIO_MODER_MODER11|\
                    GPIO_MODER_MODER12|\
                    GPIO_MODER_MODER13);
    //Режим AF
    PORT->MODER |= (GPIO_MODER_MODER6_1|\
                    GPIO_MODER_MODER7_1|\
                    GPIO_MODER_MODER10_1|\
                    GPIO_MODER_MODER11_1|\
                    GPIO_MODER_MODER12_1|\
                    GPIO_MODER_MODER13_1);
    
    //Сброс бит в режим OUT_PP
    PORT->OTYPER &= ~(GPIO_OTYPER_OT_6|\
                      GPIO_OTYPER_OT_7|\
                      GPIO_OTYPER_OT_10|\
                      GPIO_OTYPER_OT_11|\
                      GPIO_OTYPER_OT_12|\
                      GPIO_OTYPER_OT_13);
    //Сброс бит
    PORT->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR6|\
                        GPIO_OSPEEDER_OSPEEDR7|\
                        GPIO_OSPEEDER_OSPEEDR10|\
                        GPIO_OSPEEDER_OSPEEDR11|\
                        GPIO_OSPEEDER_OSPEEDR12|\
                        GPIO_OSPEEDER_OSPEEDR13);

    //Скорость 
    PORT->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR6|\
                        GPIO_OSPEEDER_OSPEEDR7|\
                        GPIO_OSPEEDER_OSPEEDR10|\
                        GPIO_OSPEEDER_OSPEEDR11|\
                        GPIO_OSPEEDER_OSPEEDR12|\
                        GPIO_OSPEEDER_OSPEEDR13);

    PORT->OSPEEDR |= (S_VH << GPIO_OSPEEDER_OSPEEDR6_Pos)|\
                     (S_VH << GPIO_OSPEEDER_OSPEEDR7_Pos)|\
                     (S_VH << GPIO_OSPEEDER_OSPEEDR10_Pos)|\
                     (S_VH << GPIO_OSPEEDER_OSPEEDR11_Pos)|\
                     (S_VH << GPIO_OSPEEDER_OSPEEDR12_Pos)|\
                     (S_VH << GPIO_OSPEEDER_OSPEEDR13_Pos);

    //Подтяжки отключены
    PORT->PUPDR &= ~(GPIO_PUPDR_PUPDR6|\
                     GPIO_PUPDR_PUPDR7|\
                     GPIO_PUPDR_PUPDR10|\
                     GPIO_PUPDR_PUPDR11|\
                     GPIO_PUPDR_PUPDR12|\
                     GPIO_PUPDR_PUPDR13);

    //AF 14
    PORT->AFR[0] &= ~(GPIO_AFRL_AFRL6|GPIO_AFRL_AFRL7);
    PORT->AFR[1] &= ~(GPIO_AFRH_AFRH10|GPIO_AFRH_AFRH11|GPIO_AFRH_AFRH12|GPIO_AFRH_AFRH13);

    PORT->AFR[0] |= (14 << GPIO_AFRL_AFRL6_Pos)|\
                    (14 << GPIO_AFRL_AFRL7_Pos);

    PORT->AFR[1] |= (14 << GPIO_AFRH_AFRH10_Pos)|\
                    (14 << GPIO_AFRH_AFRH11_Pos)|\
                    (14 << GPIO_AFRH_AFRH12_Pos)|\
                    (14 << GPIO_AFRH_AFRH13_Pos);

/****************************************** GPIOI **************************************/
    /* 0,1,2,4,5,6,7,9,10 */
    PORT=GPIOI;

    //Сброс бит
    PORT->MODER &= ~(GPIO_MODER_MODER0|\
                    GPIO_MODER_MODER1|\
                    GPIO_MODER_MODER2|\
                    GPIO_MODER_MODER4|\
                    GPIO_MODER_MODER5|\
                    GPIO_MODER_MODER6|\
                    GPIO_MODER_MODER7|\
                    GPIO_MODER_MODER9|\
                    GPIO_MODER_MODER10);
    //Режим AF
    PORT->MODER |= (GPIO_MODER_MODER0_1|\
                    GPIO_MODER_MODER1_1|\
                    GPIO_MODER_MODER2_1|\
                    GPIO_MODER_MODER4_1|\
                    GPIO_MODER_MODER5_1|\
                    GPIO_MODER_MODER6_1|\
                    GPIO_MODER_MODER7_1|\
                    GPIO_MODER_MODER9_1|\
                    GPIO_MODER_MODER10_1);
    
    //Сброс бит в режим OUT_PP
    PORT->OTYPER &= ~(GPIO_OTYPER_OT_0|\
                      GPIO_OTYPER_OT_1|\
                      GPIO_OTYPER_OT_2|\
                      GPIO_OTYPER_OT_4|\
                      GPIO_OTYPER_OT_5|\
                      GPIO_OTYPER_OT_6|\
                      GPIO_OTYPER_OT_7|\
                      GPIO_OTYPER_OT_9|\
                      GPIO_OTYPER_OT_10);
    //Сброс бит
    PORT->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR0|\
                        GPIO_OSPEEDER_OSPEEDR1|\
                        GPIO_OSPEEDER_OSPEEDR2|\
                        GPIO_OSPEEDER_OSPEEDR4|\
                        GPIO_OSPEEDER_OSPEEDR5|\
                        GPIO_OSPEEDER_OSPEEDR6|\
                        GPIO_OSPEEDER_OSPEEDR7|\
                        GPIO_OSPEEDER_OSPEEDR9|\
                        GPIO_OSPEEDER_OSPEEDR10);

    //Скорость 
    PORT->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR0|\
                        GPIO_OSPEEDER_OSPEEDR1|\
                        GPIO_OSPEEDER_OSPEEDR2|\
                        GPIO_OSPEEDER_OSPEEDR4|\
                        GPIO_OSPEEDER_OSPEEDR5|\
                        GPIO_OSPEEDER_OSPEEDR6|\
                        GPIO_OSPEEDER_OSPEEDR7|\
                        GPIO_OSPEEDER_OSPEEDR9|\
                        GPIO_OSPEEDER_OSPEEDR10);

    PORT->OSPEEDR |= (S_VH << GPIO_OSPEEDER_OSPEEDR0_Pos)|\
                     (S_VH << GPIO_OSPEEDER_OSPEEDR1_Pos)|\
                     (S_VH << GPIO_OSPEEDER_OSPEEDR2_Pos)|\
                     (S_VH << GPIO_OSPEEDER_OSPEEDR4_Pos)|\
                     (S_VH << GPIO_OSPEEDER_OSPEEDR5_Pos)|\
                     (S_VH << GPIO_OSPEEDER_OSPEEDR6_Pos)|\
                     (S_VH << GPIO_OSPEEDER_OSPEEDR7_Pos)|\
                     (S_VH << GPIO_OSPEEDER_OSPEEDR9_Pos)|\
                     (S_VH << GPIO_OSPEEDER_OSPEEDR10_Pos);

    //Подтяжки отключены
    PORT->PUPDR &= ~(GPIO_PUPDR_PUPDR0|\
                     GPIO_PUPDR_PUPDR1|\
                     GPIO_PUPDR_PUPDR2|\
                     GPIO_PUPDR_PUPDR4|\
                     GPIO_PUPDR_PUPDR5|\
                     GPIO_PUPDR_PUPDR6|\
                     GPIO_PUPDR_PUPDR7|\
                     GPIO_PUPDR_PUPDR9|\
                     GPIO_PUPDR_PUPDR10);

    //AF 14
    PORT->AFR[0] &= ~(GPIO_AFRL_AFRL0|GPIO_AFRL_AFRL1|GPIO_AFRL_AFRL2|GPIO_AFRL_AFRL4|GPIO_AFRL_AFRL5|GPIO_AFRL_AFRL6|GPIO_AFRL_AFRL7);
    PORT->AFR[1] &= ~(GPIO_AFRH_AFRH9|GPIO_AFRH_AFRH10);

    PORT->AFR[0] |= (14 << GPIO_AFRL_AFRL0_Pos)|\
                    (14 << GPIO_AFRL_AFRL1_Pos)|\
                    (14 << GPIO_AFRL_AFRL2_Pos)|\
                    (14 << GPIO_AFRL_AFRL4_Pos)|\
                    (14 << GPIO_AFRL_AFRL5_Pos)|\
                    (14 << GPIO_AFRL_AFRL6_Pos)|\
                    (14 << GPIO_AFRL_AFRL7_Pos);

    PORT->AFR[1] |= (14 << GPIO_AFRH_AFRH9_Pos)|\
                    (14 << GPIO_AFRH_AFRH10_Pos);



};


