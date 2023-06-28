#include "pantalla.h"
#include <string.h>


#ifndef DISPLAY_MAX_DIGITS
#define DISPLAY_MAX_DIGITS 8
#endif


struct display_s {
    uint8_t digits;
    uint8_t active_digit;
    uint8_t memory[DISPLAY_MAX_DIGITS];   //necesito saber cual esta pintado en este momento para saber cual tengo que inter despues
    struct display_driver_s driver[1];    //puntero a las tres funciones para borrar pintar etc
};


static const uint8_t IMAGES[] = {
    SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_D | SEGMENT_E | SEGMENT_F,
    SEGMENT_B | SEGMENT_C,
    SEGMENT_A | SEGMENT_B | SEGMENT_D | SEGMENT_E | SEGMENT_G,
    SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_D | SEGMENT_G,
    SEGMENT_B | SEGMENT_C | SEGMENT_F | SEGMENT_G,
    SEGMENT_A | SEGMENT_C | SEGMENT_D | SEGMENT_F | SEGMENT_G,
    SEGMENT_A | SEGMENT_C | SEGMENT_D | SEGMENT_E | SEGMENT_F | SEGMENT_G,
    SEGMENT_A | SEGMENT_B | SEGMENT_C,
    SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_D | SEGMENT_E | SEGMENT_F | SEGMENT_G,
    SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_F | SEGMENT_G,
};

static display_t DisplayAllocate(void) {

    static struct display_s instances[1] = {0};

    return &instances[0];
}

display_t DisplayCreate(uint8_t digits, display_driver_t driver) {

    display_t display = DisplayAllocate();
    

    if (display) {
        display->digits = digits;
        display->active_digit = digits - 1;
        memcpy(display->driver, driver, sizeof(display->driver));
        memset(display->memory, 0, sizeof(display->memory));
        display->driver->ScreenTurnOff();
    }
    return display;
}

void DisplayWriteBCD(display_t display, uint8_t * number, uint8_t size){
    memset(display->memory, 0, sizeof(display->memory));
    for(int index = 0; index < size; index++){
        if(index >= display -> digits) break;
        display->memory[index] = IMAGES[number[index]];
    }
}

void DisplayRefresh(display_t display){
    display->driver->ScreenTurnOff();
    display->active_digit = (display->active_digit + 1) % display->digits;
    display->driver->SegmentsTurnOn(display->memory[display->active_digit]);
    display->driver->DigitTurnOn(display->active_digit);
}



 



