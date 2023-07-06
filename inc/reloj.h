#include <stdbool.h>
#include <stdint.h>

typedef struct clock_s * clock_t2;
typedef struct alarm_s * alarm_t;
typedef void (*funcion_alarma)(clock_t2 reloj);

clock_t2 ClockCreate(int tics_por_segundo,funcion_alarma alarma);     //esta igual

bool ClockGetTime(clock_t2 reloj,uint8_t * hora, int size);           //este esta igual

bool ClockSetTime(clock_t2 reloj,const uint8_t * hora, int size);     //este esta igual const esta en otra posicion

uint32_t ClockTick(clock_t2 reloj);                                       //esta igual

bool ClockGetAlarm(clock_t2 reloj,uint8_t * alarma, int size);        //esta igual


bool ClockSetAlarm(clock_t2 reloj,const uint8_t * alarma, int size);  //este esta igual const esta en otra posicion

bool Alarmon(clock_t2 reloj);

bool Alarmoff(clock_t2 reloj);

// bool clockToggleAlarm(clock_t2 reloj); 

bool consultaralarma(clock_t2 reloj);


