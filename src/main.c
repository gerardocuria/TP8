#include "bsp.h"
#include "digital.h"
#include <stdbool.h>
#include <stddef.h>

#include "reloj.h"

void ActivarAlarma(clock_t2 reloj);

static board_t board;

static clock_t2 reloj;

uint8_t hora[4];

void ActivarAlarma(clock_t2 reloj){

}

int main(void){

    //uint8_t hora[6];
    reloj = ClockCreate(4, ActivarAlarma);

    SisTick_Init(1000);

    board = BoardCreate();

    while(true){

        //DisplayWriteBCD(board->display, (uint8_t[]){4,4,4,4},4);
        //DisplayRefresh(board->display);



        if(DigitalInputHasActivated(board->set_time) == true){
            DisplayWriteBCD(board->display, (uint8_t[]){1,1,1,1},4);
            DisplayRefresh(board->display);
        }

        if (DigitalInputHasActivated(board->cancel) == true){
            DisplayWriteBCD(board->display, NULL, 0);
        }

        if(DigitalInputHasActivated(board->set_time)){
    
        }

        if(DigitalInputHasActivated(board->set_alarm)== true){
            DisplayWriteBCD(board->display, (uint8_t[]){0,9,1,2},4);
            DisplayRefresh(board->display);
        }

        if(DigitalInputHasActivated(board->decrement)== true){
            DisplayWriteBCD(board->display, (uint8_t[]){1,2,3,4},4);
            DisplayRefresh(board->display);
        }

        if(DigitalInputHasActivated(board->increment)){
            DisplayWriteBCD(board->display, (uint8_t[]){9,9,9,8},4);
            DisplayRefresh(board->display);
        }

       DisplayRefresh(board->display);

        for (int index = 0; index < 100; index++) {
            for (int delay = 0; delay < 25000; delay++) {
                __asm("NOP");
            }
        }


        ClockGetTime(reloj, hora, sizeof(hora));
        __asm volatile("cpsid i");
        DisplayWriteBCD(board->display, hora, sizeof(hora));
        __asm volatile ("cpsie i");

    }
}

void SysTick_Handler(void){
    DisplayRefresh(board->display);
    ClockTick(reloj);
    //ClockGetTime(reloj, hora, sizeof(hora));
    //DisplayWriteBCD(board->display, hora, sizeof(hora));
}