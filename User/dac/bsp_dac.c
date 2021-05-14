#include "./dac/bsp_dac.h"
#include "math.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_dac.h"
//正弦波单个周期的点数
#define POINT_NUM 32
#define angle 3.1415/64


/* 初始波形数据 ---------------------------------------------------------*/
uint16_t Sine12bit[128] = {
2048,
2148,2248,2348,2447,2545,2642,2737,2831,
2923,3013,3100,3185,3267,3347,3423,3496,
3565,3631,3692,3750,3804,3854,3899,3940,
3976,4007,4034,4056,4073,4086,4093,4095,
4093,4086,4073,4056,4034,4007,3976,3940,
3899,3854,3804,3750,3692,3631,3565,3496,
3423,3347,3267,3185,3100,3013,2923,2831,
2737,2642,2545,2447,2348,2248,2148,2048,
1948,1848,1748,1649,1551,1454,1359,1265,
1173,1083,996,911,829,749,673,600,
531,465,404,346,292,242,197,156,
120,89,62,40,23,10,3,1,
3,10,23,40,62,89,120,156,
197,242,292,346,404,465,531,600,
673,749,829,911,996,1083,1173,1265,
1359,1454,1551,1649,1748,1848,1948
};

uint16_t TriangleWave12bit[128] = {
2048,
2148,2248,2348,2447,2545,2642,2737,2831,
2923,3013,3100,3185,3267,3347,3423,3496,
3565,3631,3692,3750,3804,3854,3899,3940,
3976,4007,4034,4056,4073,4086,4093,4095,
4093,4086,4073,4056,4034,4007,3976,3940,
3899,3854,3804,3750,3692,3631,3565,3496,
3423,3347,3267,3185,3100,3013,2923,2831,
2737,2642,2545,2447,2348,2248,2148,2048,
1948,1848,1748,1649,1551,1454,1359,1265,
1173,1083,996,911,829,749,673,600,
531,465,404,346,292,242,197,156,
120,89,62,40,23,10,3,1,
3,10,23,40,62,89,120,156,
197,242,292,346,404,465,531,600,
673,749,829,911,996,1083,1173,1265,
1359,1454,1551,1649,1748,1848,1948
};

uint16_t PulseWave12bit[128] = {
2048,
2148,2248,2348,2447,2545,2642,2737,2831,
2923,3013,3100,3185,3267,3347,3423,3496,
3565,3631,3692,3750,3804,3854,3899,3940,
3976,4007,4034,4056,4073,4086,4093,4095,
4093,4086,4073,4056,4034,4007,3976,3940,
3899,3854,3804,3750,3692,3631,3565,3496,
3423,3347,3267,3185,3100,3013,2923,2831,
2737,2642,2545,2447,2348,2248,2148,2048,
1948,1848,1748,1649,1551,1454,1359,1265,
1173,1083,996,911,829,749,673,600,
531,465,404,346,292,242,197,156,
120,89,62,40,23,10,3,1,
3,10,23,40,62,89,120,156,
197,242,292,346,404,465,531,600,
673,749,829,911,996,1083,1173,1265,
1359,1454,1551,1649,1748,1848,1948
};

void change_to_sine(void)
{
	DMA_InitTypeDef  DMA_InitStructure;
		
	DMA_InitStructure.DMA_PeripheralBaseAddr = DAC_DHR12R1;					
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&Sine12bit;				
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;											
    DMA_InitStructure.DMA_BufferSize = 128;																	
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;				
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;									
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;					
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;													
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;											
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

	DMA_Init(DMA2_Channel3, &DMA_InitStructure);	
}
	
void change_to_Triangle(void)
{
	DMA_InitTypeDef  DMA_InitStructure;
		
	DMA_InitStructure.DMA_PeripheralBaseAddr = DAC_DHR12R1;					//外设数据地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&TriangleWave12bit;				//内存数据地址 Sine12bit
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;											//数据传输方向内存至外设
    DMA_InitStructure.DMA_BufferSize = 128;									//缓存大小为POINT_NUM字节									
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;				//外设数据地址固定	
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;									//内存数据地址自增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//外设数据以字为单位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;					//内存数据以字为单位	
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;													//循环模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;											//高DMA通道优先级
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

	DMA_Init(DMA2_Channel3, &DMA_InitStructure);	
}

void change_to_Pulse(void)
{
	DMA_InitTypeDef  DMA_InitStructure;
		
	DMA_InitStructure.DMA_PeripheralBaseAddr = DAC_DHR12R1;					
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&PulseWave12bit;				
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;											
    DMA_InitStructure.DMA_BufferSize = 128;																	
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;				
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;									
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;					
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;													
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;											
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

	DMA_Init(DMA2_Channel3, &DMA_InitStructure);	
}
void set_sine_vpp(float vpp)
{
	int i;	
	for(i=0;i<128;i++)
	{
		Sine12bit[i]= (uint16_t)((sin(i*angle)+1)/2*vpp*4096/3.3);		
	}
	
}

void set_Triangle_vpp(float vpp)
{
	int i;	
	for(i=0;i<128;i++)
	{
		if(i<=63)
			TriangleWave12bit[i]= (uint16_t)vpp/3.3*4096/64*i;
		else
			TriangleWave12bit[i]= (uint16_t)(vpp/3.3*4096-vpp/3.3*4096/64*(i-64));	
	}			
}

void set_Pulse_vpp(float vpp)
{
	int i;	
	for(i=0;i<128;i++)
	{
		if(i<=63)
			PulseWave12bit[i]= 0;
		else
			PulseWave12bit[i]= (uint16_t)(vpp/3.3*4096);	
	}			
}

void set_peroid(int period)
{
	TIM_Cmd(TIM6, DISABLE);
	TIM6->ARR=period;
	TIM_Cmd(TIM6, ENABLE);	
}




void DAC_GPIO_config(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;

  /* 使能GPIOA时钟 */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	
	
	
  /* DAC的GPIO配置，模拟输入 */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

}


/**
  * @brief  配置DAC通道1的TIM
  * @param  无
  * @retval 无
  */
void DAC1_TIM_Config(void)
{
	
  TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure;
	
  /* 使能TIM6时钟，TIM2CLK 为72M */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	
  /*定时器初始周期*/
  TIM_TimeBaseStructure.TIM_Period = 0XF;        									
  TIM_TimeBaseStructure.TIM_Prescaler = 0x0;       							//预分频，不分频 72M / (0+1) = 72M
  TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;    						//时钟分频系数
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  	//向上计数模式
  TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);

  /* 配置TIM6触发源 */
  TIM_SelectOutputTrigger(TIM6, TIM_TRGOSource_Update);

	/* 使能TIM6 */
  TIM_Cmd(TIM6, ENABLE);
}


/**
  * @brief  配置DMA
  * @param  无
  * @retval 无
  */
void DAC1_DMA_Config(void)
{	
	DMA_InitTypeDef  DMA_InitStructure;
	
	DAC_InitTypeDef  DAC_InitStructure;

	/* 使能DMA2时钟 */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);
	
	/* 使能DAC时钟 */	
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
	
		
	  /* 配置DAC 通道1 */
	DAC_InitStructure.DAC_Trigger = DAC_Trigger_T6_TRGO;						//使用TIM6作为触发源
	DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;	//不使用波形发生器
	DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;	//不使用DAC输出缓冲
  
	DAC_Init(DAC_Channel_1, &DAC_InitStructure);
	
	/* 配置DMA2 */

	  DMA_InitStructure.DMA_PeripheralBaseAddr = DAC_DHR12R1;					//外设数据地址
	  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&Sine12bit;				//内存数据地址 Sine12bit
	  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;											//数据传输方向内存至外设
	  DMA_InitStructure.DMA_BufferSize = 128;									//缓存大小为POINT_NUM字节									
	  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;				//外设数据地址固定	
	  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;									//内存数据地址自增
	  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//外设数据以字为单位
	  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;					//内存数据以字为单位	
	  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;													//循环模式
	  DMA_InitStructure.DMA_Priority = DMA_Priority_High;											//高DMA通道优先级
	  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;														//非内存至内存模式	

	  DMA_Init(DMA2_Channel3, &DMA_InitStructure);
	
  /* 使能DMA2-14通道 */
  DMA_Cmd(DMA2_Channel3, ENABLE);
  /* 使能通道1 由PA4输出 */
  DAC_Cmd(DAC_Channel_1, ENABLE);
  /* 使能DAC的DMA请求 */
  DAC_DMACmd(DAC_Channel_1, ENABLE);
}




void DAC_Mode_Init(void)
{
	DAC_GPIO_config();
	DAC1_TIM_Config();	
	DAC1_DMA_Config();
}





























