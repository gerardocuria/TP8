#include "bsp.h"
#include "digital.h"
#include <stdbool.h>
#include <stddef.h>

#include "reloj.h"

#define tics_por_segundo 1000
#define PERIODO_PARPADEO 200

typedef enum {
    SIN_CONFIGURAR,
    MOSTRANDO_HORA,
    AJUSTANDO_MINUTOS_ACTUAL,
    AJUSTANDO_HORAS_ACTUAL,
    AJUSTANDO_MINUTOS_ALARMA,
    AJUSTANDO_HORAS_ALARMA,
} modo_t;

void ActivarAlarma(clock_t2 reloj);

static board_t board;

static clock_t2 reloj;

uint8_t hora[4];

static modo_t modo;

void ActivarAlarma(clock_t2 reloj){

}

void CambiarModo(modo_t valor) {
    modo = valor;

    switch (modo) {
    case SIN_CONFIGURAR:
        DisplayFlashDigits(board->display, 0, 3, PERIODO_PARPADEO);
        /*DisplayTurnOnDot(board->display, 1);
        DisplayTurnOffDot(board->display, 0);
        DisplayToggleDot(board->display, 2);
        DisplayToggleDot(board->display, 3);*/
        break;
    case MOSTRANDO_HORA:
        DisplayFlashDigits(board->display, 0, 3, 0);
        /*DisplayTurnOffDot(board->display, 0);
        DisplayTurnOffDot(board->display, 1);
        DisplayTurnOffDot(board->display, 2);
        DisplayTurnOffDot(board->display, 3);*/
        break;
    case AJUSTANDO_MINUTOS_ACTUAL:
        DisplayFlashDigits(board->display, 2, 3, PERIODO_PARPADEO);
       /* DisplayToggleDot(board->display, 1);
        DisplayToggleDot(board->display, 0);
        DisplayToggleDot(board->display, 2);
        DisplayToggleDot(board->display, 3);*/
        break;
    case AJUSTANDO_HORAS_ACTUAL:
       /* DisplayFlashDigits(board->display, 0, 1, PERIODO_PARPADEO);
        DisplayToggleDot(board->display, 1);
        DisplayToggleDot(board->display, 0);
        DisplayToggleDot(board->display, 2);
        DisplayToggleDot(board->display, 3);*/
        break;
    case AJUSTANDO_MINUTOS_ALARMA:
        DisplayFlashDigits(board->display, 2, 3, PERIODO_PARPADEO);
        DisplayToggleDot(board->display, 0);
        DisplayToggleDot(board->display, 1);
        DisplayToggleDot(board->display, 2);
        DisplayToggleDot(board->display, 3);
        break;
    case AJUSTANDO_HORAS_ALARMA:
        DisplayFlashDigits(board->display, 0, 1, PERIODO_PARPADEO);
        DisplayToggleDot(board->display, 0);
        DisplayToggleDot(board->display, 1);
        DisplayToggleDot(board->display, 2);
        DisplayToggleDot(board->display, 3);
        break;
    default:
        break;
    }
}


static uint32_t current_value;

int main(void){

    //uint8_t hora[6];
    static uint8_t entrada[4];
    reloj = ClockCreate(tics_por_segundo, ActivarAlarma);

    SisTick_Init(tics_por_segundo);

    board = BoardCreate();

    modo = SIN_CONFIGURAR;
    //CambiarModo(SIN_CONFIGURAR);

    DisplayFlashDigits(board->display,0, 3, PERIODO_PARPADEO); 


    //DisplayToggleDot(board->display, 1);

    while(true){

        //DisplayWriteBCD(board->display, (uint8_t[]){4,4,4,4},4);
        //DisplayRefresh(board->display);



        if(DigitalInputHasActivated(board->accept) == true){
            if(modo == AJUSTANDO_MINUTOS_ACTUAL){
                CambiarModo(AJUSTANDO_HORAS_ACTUAL);
            }else if (modo == AJUSTANDO_HORAS_ACTUAL){
                ClockSetTime(reloj,entrada, sizeof(entrada));
                CambiarModo(MOSTRANDO_HORA);
            }
        }

        if(DigitalInputHasActivated(board->cancel)){
            if(ClockGetTime(reloj,entrada,sizeof(entrada))){
                CambiarModo(MOSTRANDO_HORA);
            }else{
                CambiarModo(SIN_CONFIGURAR);
            };
    
        }

        if(DigitalInputHasActivated(board->set_time)== true){
            CambiarModo(AJUSTANDO_MINUTOS_ACTUAL);
            //ClockSetTime(reloj,entrada, sizeof(entrada));
            //DisplayWriteBCD(board->display, entrada, sizeof(entrada));

        }

        if(DigitalInputHasActivated(board->set_alarm)== true){
            
        }

        if(DigitalInputHasActivated(board->decrement) == true){
            if (modo == AJUSTANDO_MINUTOS_ACTUAL){
                entrada[3] = entrada[3]-1;
            }else if(modo== AJUSTANDO_HORAS_ACTUAL){
                entrada[1] = entrada[1]-1;
            }
            DisplayWriteBCD(board->display, entrada, sizeof(entrada));
        }

        if(DigitalInputHasActivated(board->increment)== true){
            if (modo == AJUSTANDO_MINUTOS_ACTUAL){
                entrada[3] = entrada[3]+1;
            }else if(modo== AJUSTANDO_HORAS_ACTUAL){
                entrada[1] = entrada[1]+1;
            }
            DisplayWriteBCD(board->display, entrada, sizeof(entrada));
        }

        for (int index = 0; index < 50; index++) {
            for (int delay = 0; delay < 25000; delay++) {
                __asm("NOP");
            }
        }


        // ClockGetTime(reloj, hora, sizeof(hora));
        // __asm volatile("cpsid i");
        // DisplayWriteBCD(board->display, hora, sizeof(hora));
        // __asm volatile ("cpsie i");

    }
}

void SysTick_Handler(void){

    //static bool last_value = false;

    DisplayRefresh(board->display);
    uint8_t hora[6];
    //current_value = 
    current_value = ClockTick(reloj);

    if(current_value == 500){
        DisplayToggleDot(board->display,1);
        //last_value = current_value;

        if(modo <= MOSTRANDO_HORA){
            ClockGetTime(reloj,hora,sizeof(hora));
            DisplayWriteBCD(board->display, hora, sizeof(hora));
            DisplayToggleDot(board->display,1);
        }
        
    };


    //ClockTick(reloj);
    //ClockGetTime(reloj, hora, sizeof(hora));
    //DisplayWriteBCD(board->display, hora, sizeof(hora));
}