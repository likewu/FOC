#include "main.h"
#include "tle5012b.h"

uint16_t offset = 0;

void SPI5012B_Init(void)
{
	
// ���԰�MOSI��� ��©���ģʽ����
//  ��Щ������� HAL_SPI_MspInit ����	
//	GPIO_InitStruct.Pin = GPIO_PIN_7;
//	GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
//	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
//	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	SPI1->CR1 |= SPI_CR1_SPE;
	
}


void CalibTle5012b( void )
{
	SPI5012B_Init();
	HAL_GPIO_WritePin(VCC_ON_GPIO_Port,VCC_ON_Pin,GPIO_PIN_SET);  // ��������Դ
	
	
	TIM1->CCR1 = 0;
	TIM1->CCR2 = 100;
	TIM1->CCR3 = 0;
	TIM1->CCER = 0x0555;
	TIM1->BDTR |= TIM_BDTR_MOE;
	TIM1->CR1  |= TIM_CR1_CEN;
	
	HAL_Delay(300);
	
	SPI_CS_GPIO_Port->ODR &= ~SPI_CS_Pin;
	SPI_ReadWriteData(READ_ANGLE_VALUE);
	offset = SPI_ReadWriteData(0xffff)<<1;
	SPI_CS_GPIO_Port->ODR |= SPI_CS_Pin;
	
	TIM1->CCR1 = 0;
	TIM1->CCR2 = 0;
	TIM1->CCR3 = 0;

	
//	TIM1->BDTR &= ~TIM_BDTR_MOE;
//	TIM1->CR1  &= ~TIM_CR1_CEN;

}




uint16_t SPI_ReadWriteData(uint16_t byte)
{
	uint16_t retry = 0;
	
	while( (SPI1->SR&1<<1) == 0 ) // ���ͻ������ǿ�
	{
		if( ++retry > 200 )
		return 0;                   // �ӳ�һ��ʱ��󷵻�
	}
	
	SPI1->DR = byte;              // ��������
	retry = 0;
	while( (SPI1->SR&1<<0) == 0 ) // ���ջ�����Ϊ��
	{
		if( ++retry > 200 )
		return 0;										// �ӳ�һ��ʱ��󷵻�
	}
	return SPI1->DR;              // ��һ�»����������־
	
}


// �õ� 0~359 ��
uint16_t ReadAngle(void)
{
	// return ( ReadValue(READ_ANGLE_VALUE) * 360 / 0x10000 );
	return ReadValue(READ_ANGLE_VALUE);
}
 
// �õ����ٶ�
uint16_t ReadSpeed(void)
{
	return ReadValue(READ_SPEED_VALUE);
}
 
 

uint16_t ReadValue(uint16_t u16RegValue)
{
	uint16_t u16Data;
	
//	uint16_t txbuf[2];
//	uint16_t rxbuf[2];
	
//	txbuf[0] = u16RegValue;
//	txbuf[1] = 0xffff;
 
	SPI_CS_GPIO_Port->ODR &= ~SPI_CS_Pin;

	// HAL_SPI_TransmitReceive(&hspi1,(uint8_t *)&txbuf,(uint8_t *)&rxbuf,2,2);
	// u16Data = rxbuf[1] << 1;   // ���16λ���
	
	SPI_ReadWriteData(u16RegValue);
	u16Data = SPI_ReadWriteData(0xffff)<<1;
	
	SPI_CS_GPIO_Port->ODR |= SPI_CS_Pin;
	
	u16Data = (65536 + u16Data - offset);
	
	u16Data = 65536 - u16Data;

	return u16Data;
}

