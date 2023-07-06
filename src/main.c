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

static const uint8_t LIMITES_MINUTOS[] = {5,9};

static const uint8_t LIMITES_HORAS[] = {2,3};

void ActivarAlarma(clock_t2 reloj){

}


void IncrementarBCD(uint8_t numero[2], const uint8_t limite[2]) {
    numero[1]++;
    if (numero[1] > 9) {
        numero[1] = 0;
        numero[0]++;
    }
    if ((numero[0] >= limite[0]) && numero[1] > limite[1]) {
        numero[1] = 0;
        numero[0] = 0;
    }
}

void DecrementarBCD(uint8_t numero[2], const uint8_t limite[2]) {
    numero[1]--;
    if (numero[1] > 9) {
        numero[1] = 9;
        numero[0]--;
    }
    if ((numero[0] >= limite[0]) && numero[1] >= limite[1]) {
        numero[0] = 0;
        numero[1] = 0;
    }
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
        DisplayFlashDigits(board->display, 0, 1, PERIODO_PARPADEO);
        /*DisplayToggleDot(board->display, 1);
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
            if (modo == MOSTRANDO_HORA){
                if(!ClockGetTime(reloj,entrada,sizeof(entrada))){
                    //ClockToggleAlarm(reloj);
                }
            }else if(modo == AJUSTANDO_MINUTOS_ACTUAL){
                CambiarModo(AJUSTANDO_HORAS_ACTUAL);
            }else if (modo == AJUSTANDO_HORAS_ACTUAL){
                ClockSetTime(reloj,entrada, sizeof(entrada));
                CambiarModo(MOSTRANDO_HORA);
            }else if (modo == AJUSTANDO_MINUTOS_ALARMA){
                CambiarModo(AJUSTANDO_HORAS_ALARMA);
            }else if (modo == AJUSTANDO_HORAS_ALARMA){
                    ClockSetAlarm(reloj,entrada, sizeof(entrada));
                    CambiarModo(MOSTRANDO_HORA);
            }
        }


        if(DigitalInputHasActivated(board->cancel)){
            if (modo == MOSTRANDO_HORA){
                if(ClockGetAlarm(reloj,entrada,sizeof(entrada))){
                    //ClockToggleAlarm(reloj);
                }
            } else {
                if(ClockGetTime(reloj,entrada,sizeof(entrada))){
                    CambiarModo(MOSTRANDO_HORA);
                } else {
                    CambiarModo(SIN_CONFIGURAR);
                };
            }
        }

        if(DigitalInputHasActivated(board->set_time)== true){
            CambiarModo(AJUSTANDO_MINUTOS_ACTUAL);
            //ClockSetTime(reloj,entrada, sizeof(entrada));
            //DisplayWriteBCD(board->display, entrada, sizeof(entrada));

        }

        if(DigitalInputHasActivated(board->set_alarm)== true){
            CambiarModo(modo == AJUSTANDO_MINUTOS_ALARMA);
            ClockGetAlarm(reloj,entrada,sizeof(entrada));
            DisplayWriteBCD(board->display, entrada, sizeof(entrada));


        }

        if(DigitalInputHasActivated(board->decrement) == true){
            if (modo == AJUSTANDO_MINUTOS_ACTUAL || modo == AJUSTANDO_MINUTOS_ALARMA){
                DecrementarBCD(&entrada[2], LIMITES_MINUTOS);
            }else if(modo== AJUSTANDO_HORAS_ACTUAL || modo == AJUSTANDO_HORAS_ALARMA){
                DecrementarBCD(entrada, LIMITES_HORAS);
            }
            DisplayWriteBCD(board->display, entrada, sizeof(entrada));
        }

        if(DigitalInputHasActivated(board->increment)== true){
            if (modo == AJUSTANDO_MINUTOS_ACTUAL || modo == AJUSTANDO_MINUTOS_ALARMA){
                IncrementarBCD(&entrada[2], LIMITES_MINUTOS);
            }else if(modo== AJUSTANDO_HORAS_ACTUAL || modo == AJUSTANDO_HORAS_ALARMA){
                IncrementarBCD(entrada, LIMITES_HORAS);
            }
            DisplayWriteBCD(board->display, entrada, sizeof(entrada));
        }

// min 1hrs

        for (int index = 0; index < 10; index++) {
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

        //DisplayToggleDot(board->display,1);
        //last_value = current_value;

        if(modo <= MOSTRANDO_HORA){
            ClockGetTime(reloj,hora,sizeof(hora));
            DisplayWriteBCD(board->display, hora, sizeof(hora));
            if(current_value == 500){
            DisplayToggleDot(board->display,1);
            };

        };


    //ClockTick(reloj);
    //ClockGetTime(reloj, hora, sizeof(hora));
    //DisplayWriteBCD(board->display, hora, sizeof(hora));
}