#include "i2c.h"

/* I2C handler declaration */
I2C_HandleTypeDef I2cHandle;

static void Delay_Us(uint32_t ticks)
{
	uint32_t n = ticks*216; // 216MHz, useconds
	while(n--);
}

static void Error_Handler()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	__HAL_RCC_GPIOB_CLK_ENABLE();
	GPIO_InitStructure.Pin = GPIO_PIN_14;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
	
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_SET);
	while(1)
	{
		HAL_GPIO_TogglePin(LED3_GPIO_PORT,GPIO_PIN_14);
		HAL_Delay(500);
	}
}


void I2C_Init() 
{
	/* GPIO & Simulated I2C Initializing Structures */
	GPIO_InitTypeDef GPIO_InitStructure;
	__HAL_RCC_GPIOB_CLK_ENABLE();
	GPIO_InitStructure.Pin =  GPIO_PIN_8 | GPIO_PIN_9;	
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;			
	GPIO_InitStructure.Mode = GPIO_MODE_AF_OD;				
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	I2C_SDA(1);
	I2C_SCL(1);
	
	/* I2C Initializing Using ST HAL Driver */
	/* I2cHandle.Instance             = I2Cx;
  I2cHandle.Init.Timing          = I2C_TIMING;
  I2cHandle.Init.OwnAddress1     = I2C_ADDRESS;
  I2cHandle.Init.AddressingMode  = I2C_ADDRESSINGMODE_10BIT;
  I2cHandle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  I2cHandle.Init.OwnAddress2     = 0xFF;
  I2cHandle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  I2cHandle.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE; */
	
	/* Peripheral clock enable */
	//__I2C1_CLK_ENABLE();
	/* Peripheral interrupt init*/
  /*HAL_NVIC_SetPriority(I2Cx_EV_IRQn, 0, 1);
  HAL_NVIC_EnableIRQ(I2Cx_EV_IRQn);
  HAL_NVIC_SetPriority(I2Cx_ER_IRQn, 0, 2);
  HAL_NVIC_EnableIRQ(I2Cx_ER_IRQn);
	
	if(HAL_I2C_Init(&I2cHandle) != HAL_OK)
  {
    Error_Handler();
  }*/
}

void I2C_Start()
{
	SDA_OUT();
	I2C_SDA(1);
	I2C_SCL(1);
	Delay_Us(4);
	I2C_SDA(0);
	Delay_Us(4);
	I2C_SCL(0);
}

void I2C_Stop()
{
	SDA_OUT();
	I2C_SCL(0);
	I2C_SDA(0);
	Delay_Us(4);
	I2C_SCL(1);
	I2C_SDA(1);
}

uint8_t I2C_Wait_Ack()
{
	uint8_t ucErrTime  = 0;
	
	SDA_IN();  // input
  I2C_SDA(1); Delay_Us(1);
  I2C_SCL(1); Delay_Us(1);
	while(READ_SDA) {
      ucErrTime++;
      if (ucErrTime > 250) {
          I2C_Stop();
          return 1;  //failed
      }
	}
  I2C_SCL(0);

	return 0;  // ack sucess
}

void I2C_Ack()
{
	I2C_SCL(0);
  SDA_OUT();
  I2C_SDA(0);
  Delay_Us(2);
  I2C_SCL(1);
  Delay_Us(2);
  I2C_SCL(0);
}

void I2C_NoAck()
{
	I2C_SCL(0);
  SDA_OUT();
  I2C_SDA(1);
  Delay_Us(2);
  I2C_SCL(1);
  Delay_Us(2);
  I2C_SCL(0);
}

void I2C_Send_Byte(uint8_t txd)
{
	uint8_t t;
  SDA_OUT();
  I2C_SCL(0);
  for(t = 0; t < 8; t++) {
      I2C_SDA((txd & 0x80) >> 7);
      txd <<= 1;
      Delay_Us(2);
      I2C_SCL(1);
      Delay_Us(2);
      I2C_SCL(0);
      Delay_Us(2); 
    }
}

uint8_t I2C_Read_Byte(uint8_t ack)
{
	uint8_t i,receive = 0;

  SDA_IN();           //SDA receive data
  for(i = 0; i < 8; i++) {
      I2C_SCL(0);
      Delay_Us(2);
      I2C_SCL(1);
      receive <<= 1;
      if (READ_SDA) receive++;
      Delay_Us(1);
  }

  if (!ack) {
      I2C_NoAck();     // no need ack
  } else {
      I2C_Ack();      // need ack
  }
  return receive;
}


