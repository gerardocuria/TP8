#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "reloj.h"

bool compara(clock_t2 reloj);


struct alarm_s {
    uint8_t alarma_actual[6];
    bool valida;
    bool habilitada;
    funcion_alarma funcion;
    bool sonar;
    uint8_t posponer;
};

struct clock_s {
    uint8_t hora_actual[6];
    bool valida;
    uint32_t tics_por_segundo;
    uint32_t conteo_tics;
    struct alarm_s alarm[1]; 
    //uint8_t hora_alarma[6];
    //bool alarma_habilitada;
};


clock_t2 ClockCreate(int tics_por_segundo,funcion_alarma alarma) {
    static struct clock_s self[1];
    memset(self, 0, sizeof(self));
    self->tics_por_segundo = tics_por_segundo;
    self->alarm->funcion=alarma;
    return self;
}

bool ClockGetTime(clock_t2 reloj,uint8_t * hora, int size){
    memcpy(hora, reloj->hora_actual, size);
    return reloj->valida;
}

bool ClockSetTime(clock_t2 reloj,const uint8_t * hora, int size){
    memcpy(reloj->hora_actual,hora, size);
    reloj->valida = true;
    return true;
}

uint32_t ClockTick(clock_t2 reloj){
    reloj->conteo_tics++;

    compara(reloj);


    if(reloj->conteo_tics==reloj->tics_por_segundo){
        reloj->hora_actual[5]++;
        reloj->conteo_tics=0;
        DecrementarPosponerAlarma (reloj);
  
    }

    if(reloj->hora_actual[5]==10){
        reloj->hora_actual[5]=0;    
        reloj->hora_actual[4]++;
    }

    if(reloj->hora_actual[4]==6){
        reloj->hora_actual[4]=0;    
        reloj->hora_actual[3]++;
    }

    if(reloj->hora_actual[3]==10){
        reloj->hora_actual[3]=0;    
        reloj->hora_actual[2]++;
    }

    if(reloj->hora_actual[2]==6){
        reloj->hora_actual[2]=0;    
        reloj->hora_actual[1]++;
    }

    if(reloj->hora_actual[1]==10){
        reloj->hora_actual[1]=0;    
        reloj->hora_actual[0]++;
    }
    
    if(reloj->hora_actual[1]==4 && reloj->hora_actual[0]==2){
        reloj->hora_actual[0]=0;  
        reloj->hora_actual[1]=0;    
    }
    //reloj->alarm->funcion(reloj);
    return reloj->conteo_tics;
}

//tengo que hacer un get time y un set time par la alarmavalida



bool ClockGetAlarm(clock_t2 reloj,uint8_t * alarma, int size){
    memcpy(alarma, reloj->alarm->alarma_actual, size);
    return reloj->alarm->valida;
}

bool ClockSetAlarm(clock_t2 reloj,const uint8_t * alarma, int size){
    memcpy(reloj->alarm->alarma_actual,alarma, size);
    reloj->alarm->valida = true;
    return true;
}


bool Alarmon(clock_t2 reloj){
    reloj->alarm->valida=true;
    return reloj->alarm->valida;
}

bool Alarmoff(clock_t2 reloj){
    reloj->alarm->valida=false;
    return reloj->alarm->valida;
}


bool consultaralarma(clock_t2 reloj){
    bool habilitada = reloj->alarm->habilitada;
    return habilitada;
}


bool compara(clock_t2 reloj){
    if(memcmp(reloj->alarm->alarma_actual,reloj->hora_actual, sizeof(reloj->hora_actual)) == 0 && reloj->alarm->valida==true){
        reloj->alarm->funcion(reloj);
    }
    return false;
}

void PosponerAlarma (clock_t2 reloj){
    reloj->alarm->posponer = 60;
}


void DecrementarPosponerAlarma (clock_t2 reloj){
        reloj->alarm->posponer--;
        if (reloj->alarm->posponer == 0 && reloj->alarm->valida==true) {
            reloj->alarm->funcion(reloj);
        }
    }
