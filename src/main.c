#include "main.h"

volatile uint32_t RAM_D1 ALGN4 letter[32*1024/4];

int main(void){
	
	RCC_init();
	ltdc_init();

	ltdc_show(LTDC_Layer2,100,100,200,200,(uint32_t *)letter,255,LTDC_PIXEL_RGB565);
	gr_test((uint16_t *)letter);

	while(1){};
}

void gr_test(uint16_t *mem){
     uint16_t color;

	 for(uint8_t h=0;h<100;h++){
        if((h % 2) == 0 ){
            color=RED;
        }else{
            color=GREEN;
        }
        for(uint8_t i=0; i < 100;i++){
               *mem++=color;
        };
    };
};

