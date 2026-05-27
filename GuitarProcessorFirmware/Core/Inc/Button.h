#ifndef BUTTON_H
#define BUTTON_H
#include "main.h"

typedef struct {
    GPIO_TypeDef* gpioPort;
    uint16_t mask;
    uint16_t pinNum;
    uint16_t state;
} button_t;

// this initializes a button object
void button_init(button_t* btn, GPIO_TypeDef* port, uint16_t pin, bool activeLow);

// call this on the timer callback. It checks the input and returns true when a valid click happens.
bool button_debounce(button_t* btn);
#endif