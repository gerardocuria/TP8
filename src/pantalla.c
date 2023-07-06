#include "pantalla.h"
#include <string.h>


#ifndef DISPLAY_MAX_DIGITS
#define DISPLAY_MAX_DIGITS 8
#endif


struct display_s {
    uint8_t digits;
    uint8_t active_digit;

    uint8_t flashing_from;
    uint8_t flashing_to;
    uint16_t flashing_count;
    uint16_t flashing_factor;

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

        display->flashing_from = 0;
        display->flashing_to= 0;
        display->flashing_count = 0;
        display->flashing_factor = 0;

        memcpy(display->driver, driver, sizeof(display->driver));
        memset(display->memory, 0, sizeof(display->memory));
        display->driver->ScreenTurnOff();
    }
    return display;
}

void DisplayWriteBCD(display_t display, uint8_t * number, uint8_t size){
    //memset(display->memory, 0, sizeof(display->memory));
    for(int index = 0; index < size; index++){
        if(index >= display -> digits) break;
        display->memory[index]  &= (1<<7);
        display->memory[index] |= IMAGES[number[index]];
        //display->memory[index] =  (1<<7); 
    }
}

void DisplayRefresh(display_t display){

    uint8_t segments;

    display->driver->ScreenTurnOff();
    display->active_digit = (display->active_digit + 1) % display->digits;

    segments = display->memory[display->active_digit];
    if(display->flashing_factor != 0){
        if(display->active_digit == 0){
            display->flashing_count = (display->flashing_count + display->digits) % display->flashing_factor;
        }
        if((display->active_digit >= display->flashing_from) && (display->active_digit <= display->flashing_to)){
            if(display->flashing_count > (display->flashing_factor / 2 )){
                segments=0;
            }
        }
    }
    display->driver->SegmentsTurnOn(segments);
    display->driver->DigitTurnOn(display->active_digit);
}

void DisplayFlashDigits(display_t display, uint8_t from, uint8_t to, uint16_t frecuency){
    display->flashing_count = 0;
    display->flashing_factor = frecuency;
    display->flashing_from = from;
    display->flashing_to = to;
}

void DisplayToggleDot (display_t display, uint8_t position){
    display->memory[position] ^= (1<<7);
}

void DisplayDotOn (display_t display, uint8_t position){
    display->memory[position] |= (1<<7);
}

void DisplayDotOff (display_t display, uint8_t position){
    display->memory[position] &= !(1<<7);
}



 



