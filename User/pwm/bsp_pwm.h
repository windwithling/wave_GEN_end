#ifndef __PWM_H
#define	__PWM_H

#include "stm32f10x.h"


void TIM3_PWM_Init(u16 arr,u16 psc);
void set_pulse_period(int period);


#endif 


