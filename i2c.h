#ifndef _I2C_H
#define _I2C_H

#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_i2c.h"
#include "stm32f7xx_nucleo_144.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* User can use this section to tailor I2Cx/I2Cx instance used and associated
   resources */
#define I2C_ADDRESS       0x30F
#define I2C_TIMING        0x00D00E28  /* (Rise time = 120ns, Fall time = 25ns) */
/* Definition for I2Cx clock resources */
#define I2Cx                            I2C1
#define I2Cx_CLK_ENABLE()               __HAL_RCC_I2C1_CLK_ENABLE()
#define I2Cx_SDA_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()
#define I2Cx_SCL_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE() 

#define I2Cx_FORCE_RESET()              __HAL_RCC_I2C1_FORCE_RESET()
#define I2Cx_RELEASE_RESET()            __HAL_RCC_I2C1_RELEASE_RESET()

/* Definition for I2Cx Pins */
#define I2Cx_SCL_PIN                    GPIO_PIN_8
#define I2Cx_SCL_GPIO_PORT              GPIOB
#define I2Cx_SDA_PIN                    GPIO_PIN_9
#define I2Cx_SDA_GPIO_PORT              GPIOB
#define I2Cx_SCL_SDA_AF                 GPIO_AF4_I2C1

/* Definition for I2Cx's NVIC */
#define I2Cx_EV_IRQn                    I2C1_EV_IRQn
#define I2Cx_ER_IRQn                    I2C1_ER_IRQn
#define I2Cx_EV_IRQHandler              I2C1_EV_IRQHandler
#define I2Cx_ER_IRQHandler              I2C1_ER_IRQHandler

/* Simulated I2C */
#define I2C_SDA(n) (n?HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET))
#define I2C_SCL(n) (n?HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET))
#define SDA_OUT()   {GPIOB->MODER &= ~(0x3 << (10));GPIOB->MODER |= 0x0 << 10;}
#define SDA_IN()    {GPIOB->MODER &= ~(0x3 << 10); GPIOB->MODER |= 0x0 << 10;}
#define READ_SDA    HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9) 

void I2C_Init(void);
void I2C_Start(void);
void I2C_Stop(void);
uint8_t I2C_Wait_Ack(void);
void I2C_Ack(void);
void I2C_NoAck(void);

void I2C_Send_Byte(uint8_t txd);
uint8_t I2C_Read_Byte(uint8_t ack);

#endif

