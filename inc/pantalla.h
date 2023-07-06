#include <stdint.h>



#define SEGMENT_A (1<<0)
#define SEGMENT_B (1<<1)
#define SEGMENT_C (1<<2)
#define SEGMENT_D (1<<3)
#define SEGMENT_E (1<<4)
#define SEGMENT_F (1<<5)
#define SEGMENT_G (1<<6)
#define SEGMENT_P (1<<7)




typedef struct display_s * display_t;

typedef void(*display_screen_off_t)(void);

typedef void(*display_segments_on_t)(uint8_t segments);

typedef void(*display_digit_on_t)(uint8_t digit);

typedef struct display_driver_s {

    display_screen_off_t ScreenTurnOff;
    display_segments_on_t SegmentsTurnOn;
    display_digit_on_t DigitTurnOn;

} const * const display_driver_t;

display_t DisplayCreate(uint8_t digits, display_driver_t driver);

void DisplayWriteBCD(display_t display, uint8_t * number, uint8_t size);

void DisplayRefresh(display_t display);


void DisplayFlashDigits(display_t display,uint8_t from, uint8_t to, uint16_t frecuency); 

void DisplayToggleDot(display_t display, uint8_t position);

void DisplayDotOn (display_t display, uint8_t position);

void DisplayDotOff (display_t display, uint8_t position);
