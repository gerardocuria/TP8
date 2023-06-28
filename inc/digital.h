#ifndef DIGITAL_H
#define DIGITAL_H
#endif

#include <stdint.h>
#include <stdbool.h>

typedef struct digital_output_s * digital_output_t;
typedef struct digital_input_s * digital_input_t;

digital_input_t DigitalInputCreate(uint8_t port,uint8_t pin, bool logic);


bool DigitalInputGetState(digital_input_t input);

bool DigitalInputHasChanged(digital_input_t input);

bool DigitalInputHasActivated(digital_input_t input);

bool DigitalInputHasDesactivated(digital_input_t input);




digital_output_t DigitalOutputCreate(uint8_t port,uint8_t pin);


void DigitalOutputActivate(digital_output_t output);


void DigitalOutputDesactivate(digital_output_t output);


void DigitalOutputToggle(digital_output_t output);
