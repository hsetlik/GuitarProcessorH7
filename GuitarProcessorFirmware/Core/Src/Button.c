#include "Button.h"
#include "stm32h7xx_hal_gpio.h"


void button_init(button_t* btn, GPIO_TypeDef* port, uint16_t pin, bool activeLow){
    btn->gpioPort = port;
    btn->pinNum = pin;
    if(activeLow){
        btn->state = 0xFFFF;
        btn->mask = 0xFFF0;
    } else {
        btn->state = 0;
        btn->mask = 0x000F;
    }
}


bool button_debounce(button_t* btn){
    btn->state = (btn->state << 1) | HAL_GPIO_ReadPin(btn->gpioPort, btn->pinNum);
    return btn->state == btn->mask; 
}