#include "stm32f10x.h"
#include "bsp_usart.h"
#include "bsp_dac.h"
#include "string.h"
#include "bsp_pwm.h"
#include "stm32f10x_tim.h"

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
  
  
uint8_t UART1_RX_BUF[100];
uint8_t rx_cnt = 0;

WAVE Sine;
WAVE Triangle;
WAVE Pulse;

/*temp var definition*/
uint8_t ucTemp;
int freq;
float vpp;

int wave_flag;
/*
1---sine
2---Triangle
3---pulse
*/


int main(void)
{		
	DAC_Mode_Init();
	USART_Config();	
    while(1);	
}



void DEBUG_USART_IRQHandler(void)
{	
	if(USART_GetITStatus(DEBUG_USARTx,USART_IT_RXNE)!=RESET)
	{		
		ucTemp =USART_ReceiveData(USART1);	
		UART1_RX_BUF[rx_cnt++]=ucTemp;		
		if(ucTemp==0x0a)
		{
			sscanf((char *)UART1_RX_BUF,"%d,%d,%f",&wave_flag,&freq,&vpp);
			switch(wave_flag)
			{
				case 1:
				{
					printf("SET sine_f = %d,sine_VPP = %f\r\n",freq,vpp);
					Sine.vpp = vpp;
					Sine.freq = freq;
					Sine.period = (int)(562500/Sine.freq + 1);
					set_sine_vpp(Sine.vpp);
					change_to_sine();
					set_peroid(Sine.period);								
					break;
				}
				case 2:
				{
					printf("SET Triangle_f = %d,Triangle_VPP = %f\r\n",freq,vpp);
					Triangle.vpp = vpp;
					Triangle.freq = freq;
					Triangle.period = (int)(562500/Triangle.freq + 1);	
					set_Triangle_vpp(Triangle.vpp);
					change_to_Triangle();						
					set_peroid(Triangle.period);					
					break;					
				}
				case 3:
				{
					printf("SET Pulse_f = %d,Pulse_VPP = %f\r\n",freq,vpp);
					Pulse.vpp = vpp;
					Pulse.freq = freq;
					Pulse.period = (int)(562500/Pulse.freq + 1);	
					set_Pulse_vpp(Pulse.vpp);	
					change_to_Pulse();
					set_peroid(Pulse.period);				
					break;					
				}
				default:break;
			}
			rx_cnt = 0;
	    }	 
     }	 
}
/*********************************************END OF FILE**********************/
