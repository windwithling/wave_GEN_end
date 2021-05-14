#ifndef __DAC_H
#define	__DAC_H


#include "stm32f10x.h"

//DAC DHR12RD�Ĵ�����12λ���Ҷ��롢˫ͨ��
#define DAC_DHR12RD_ADDRESS      (DAC_BASE+0x20) //0x40007420
#define DAC_DHR8R1_Address      0x40007410
#define DAC_DHR12R1    0x40007408   //DACͨ��1�����ַ
#define DAC_DHR12R2    0x40007414   //DACͨ��2�����ַ

//�������ݽṹ��
typedef struct
{
	float vpp;
	int period;
	int freq;
}WAVE;

//���ò���
void change_to_sine(void);
void change_to_Triangle(void);
void change_to_Pulse(void);
//���÷��ֵ
void set_sine_vpp(float vpp);
void set_Triangle_vpp(float vpp);
void set_Pulse_vpp(float vpp);
//��������
void set_peroid(int period);


void DAC_GPIO_config(void);
void DAC1_TIM_Config(void);
void DAC1_DMA_Config(void);
void DAC_Mode_Init(void);


#endif /* __DAC_H */

