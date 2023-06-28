#include "digital.h"
#include "pantalla.h"
#include <stdint.h>
#include <stdbool.h>


typedef struct board_s {
    digital_output_t buzzer;
    display_t display;
    digital_input_t set_time;
    digital_input_t set_alarm;
    digital_input_t decrement;

    digital_input_t increment;
    digital_input_t accept;
    digital_input_t cancel;
    
    
} const * board_t;

board_t BoardCreate(void);

void SisTick_Init(uint16_t ticks);