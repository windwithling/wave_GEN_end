#ifndef __DAC_H
#define	__DAC_H


#include "stm32f10x.h"

//DAC DHR12RD寄存器，12位、右对齐、双通道
#define DAC_DHR12RD_ADDRESS      (DAC_BASE+0x20) //0x40007420
#define DAC_DHR8R1_Address      0x40007410
#define DAC_DHR12R1    0x40007408   //DAC通道1输出地址
#define DAC_DHR12R2    0x40007414   //DAC通道2输出地址

//波形数据结构体
typedef struct
{
	float vpp;
	int period;
	int freq;
}WAVE;

//设置波形
void change_to_sine(void);
void change_to_Triangle(void);
void change_to_Pulse(void);
//设置峰峰值
void set_sine_vpp(float vpp);
void set_Triangle_vpp(float vpp);
void set_Pulse_vpp(float vpp);
//设置周期
void set_peroid(int period);


void DAC_GPIO_config(void);
void DAC1_TIM_Config(void);
void DAC1_DMA_Config(void);
void DAC_Mode_Init(void);


#endif /* __DAC_H */

