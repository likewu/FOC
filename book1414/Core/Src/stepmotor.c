#include "stepmotor.h"
#include "bldc.h"

//stepmotor_t mymotor;



// 经过测试，示例使用四拍方式刚好是400步走完一圈，故步距角是 360/400 = 0.9 度
// 又因步距角 = 180/(相数*转子极对数),前面已实测出该步进电机相数为2 ，可知，转子极对数为100
// 八拍驱动方式为四拍驱动方式的1/2细分驱动，在该驱动方式下，步时电机每拍转动 1/2 步距角即0.45度
// 故需要800个节拍才能起完一圈


// 
void setpwm(uint16_t pwm)
{
	 if( pwm > 1199 ) pwm = 1199;
	 TIM1->CCR1 = pwm; 
	 TIM1->CCR2 = pwm; 
	 TIM1->CCR3 = pwm; 
	 TIM1->CCR4 = pwm; 
}


#if 1

	
// （1）四拍：A 1000，A- 0010,   B 0100,  B- 0001.
void step4(uint8_t step)
{
	TIM1->CCER &= 0xeeee;
	switch(step)
	{
		case 0: TIM1->CCER |= 0x0001; break;
		case 1: TIM1->CCER |= 0x0100; break;
		case 2: TIM1->CCER |= 0x0010; break;
		case 3: TIM1->CCER |= 0x1000; break;
		
		default:break;
	}
}

// （2）八拍：A 1000，AA- 1010,A- 0010,A-B 0110,B 0100,BB- 0101,B- 0001,B-A 1001.


void step8(uint8_t step)
{
	TIM1->CCER &= 0xeeee;
	switch(step)
	{
		case 0: TIM1->CCER |= 0x0001; break;
		case 1: TIM1->CCER |= 0x0101; break;
		case 2: TIM1->CCER |= 0x0100; break;
		case 3: TIM1->CCER |= 0x0110; break;
		case 4: TIM1->CCER |= 0x0010; break;
		case 5: TIM1->CCER |= 0x1010; break;
		case 6: TIM1->CCER |= 0x1000; break;
		case 7: TIM1->CCER |= 0x1001; break;	
		default:break;
		}
}


#else


// （1）四拍：A 1000，A- 0010,   B 0100,  B- 0001.
void step4(uint8_t step)
{
	
//	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_RESET);
//	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_RESET);
//	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_RESET);
//	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET);
	
	
	switch(step)
	{
		case 0:
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET);
			break;
		
		case 1:
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET);
			break;
		
		case 2:
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET);
			break;
		
		case 3:
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET);
			break;
		
		default:break;
	}
}

// （2）八拍：A 1000，AA- 1010,A- 0010,A-B 0110,B 0100,BB- 0101,B- 0001,B-A 1001.


void step8(uint8_t step)
{
	
//		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_RESET);
//		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_RESET);
//		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_RESET);
//		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET);
	
		switch(step)
		{
		case 0:
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET);
			break;
		
		case 1:
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET);
			break;
		
		case 2:
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET);
			break;
		
		case 3:
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET);
			break;
		
		case 4:
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET);
			break;
		
		case 5:
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET);
			break;
		
		case 6:
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET);
			break;
		
		case 7:
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET);
			break;
		
		default:break;
		}
}


#endif 
