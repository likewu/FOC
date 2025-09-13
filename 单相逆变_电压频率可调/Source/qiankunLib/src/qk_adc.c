/******************** (C) COPYRIGHT 2014 qiankunPower ********************

File Name		qk_adc.c
Author			qiankunTeam
Version			V0.0.1
Date			2014-4-10
Description		--

**************************************************************************/
#include "qk_adc.h"


u16 ADC_Results[4];

/****************************************************************

	�������ܣ�	ADC�ĳ�ʼ��
	���������	��
	���أ�		��
	��������:	2014-4-2
	��ע��		��

*****************************************************************/
void AdcInit(void)
{
	ADC_InitTypeDef ADC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	DMA_InitTypeDef DMA_InitStructure; 

	//ʹ��ADC1,ADC2
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_ADC2,ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);
	//ʹ��GPIOC
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	//ʹ��DMA
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);

	//����PC0(channel 10),PC1(channel 11),PC2(channel 12),PC3(channel 13),PC4(channel 14)����ΪADC������ͨ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_4; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	//DMA����
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;							//�����ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_Results;							//�ڴ��ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;									//dma���䷽����
	DMA_InitStructure.DMA_BufferSize = 4;												//����DMA�ڴ���ʱ�������ĳ���
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;					//����DMA���������ģʽ��һ������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;								//����DMA���ڴ����ģʽ��
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;			//���������ֳ�
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;					//�ڴ������ֳ�
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;										//����DMA�Ĵ���ģʽ���������ϵ�ѭ��ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;									//����DMA�����ȼ���
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;										//����DMA��2��memory�еı����������
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	
	DMA_Cmd(DMA1_Channel1, ENABLE);														//ʹ��DMA

	//ADC1����
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;									//��������ģʽ
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;										//ɨ�跽ʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;									//����ת��
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;					//�ⲿ������ֹ
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;								//�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 4;												//����ת����ͨ����
	ADC_Init(ADC1, &ADC_InitStructure);
	
	//����ģʽͨ������
	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_7Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 2, ADC_SampleTime_7Cycles5);  
	ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 3, ADC_SampleTime_7Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 4, ADC_SampleTime_7Cycles5);
	
	/* Enable ADC1 DMA [ʹ��ADC1 DMA]*/
  	ADC_DMACmd(ADC1, ENABLE);
  																		
	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);//

	/* Enable ADC1 reset calibaration register */   
	ADC_ResetCalibration(ADC1);
	/* Check the end of ADC1 reset calibration register */
	while(ADC_GetResetCalibrationStatus(ADC1));
	
	/* Start ADC1 calibaration */
	ADC_StartCalibration(ADC1);
	/* Check the end of ADC1 calibration */
	while(ADC_GetCalibrationStatus(ADC1));
	
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);

}

