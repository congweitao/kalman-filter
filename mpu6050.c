#include "mpu6050.h"
#include "i2c.h"

static void Error_Handler()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	__HAL_RCC_GPIOB_CLK_ENABLE();
	GPIO_InitStructure.Pin = GPIO_PIN_7;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
	
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_SET);
	while(1)
	{
		HAL_GPIO_TogglePin(LED3_GPIO_PORT,GPIO_PIN_7);
		HAL_Delay(500);
	}
}


void MPU6050_I2C_Write_Byte(uint8_t data, uint8_t reg)
{
	I2C_Start();
	I2C_Send_Byte(0xd0);  // send device address + write signal 
	if(I2C_Wait_Ack())   // wait ack
	{
		I2C_Stop();
	}
	I2C_Send_Byte(reg);  //send reg/write address
	I2C_Wait_Ack();
	I2C_Send_Byte(data);
	if(I2C_Wait_Ack())
	{
		I2C_Stop();
	}
	I2C_Stop();
	HAL_Delay(10);
}

uint8_t MPU6050_I2C_Read_Byte(uint8_t reg)
{
	 uint8_t res;
   I2C_Start();
   I2C_Send_Byte(0xd0); 
   I2C_Wait_Ack();
   I2C_Send_Byte(reg);
   I2C_Wait_Ack();
   I2C_Start();
   I2C_Send_Byte(0xd0+1);
   I2C_Wait_Ack();
   res = I2C_Read_Byte(0);
   I2C_Stop();
   return res;
}

uint8_t MPU6050_I2C_Write_Len(uint8_t len, uint8_t *buffer,uint8_t reg)
{
	uint8_t i;
	I2C_Start();
  I2C_Send_Byte(0xd0);
  if(I2C_Wait_Ack())
  {
    I2C_Stop();
    return 1;
  }
  I2C_Send_Byte(reg);
  I2C_Wait_Ack();
  for(i = 0; i < len; i++)
  {
    I2C_Send_Byte(buffer[i]);
    if(I2C_Wait_Ack())
    {
      I2C_Stop();
      return 1;
    }
  }
  I2C_Stop();
  return 0;
}

uint8_t MPU6050_I2C_Read_Len(uint8_t len, uint8_t *buffer,uint8_t reg)
{
	 I2C_Start();
   I2C_Send_Byte(0xd0);
   if(I2C_Wait_Ack())
   {
      I2C_Stop();
      return 1;
   }
   I2C_Send_Byte(reg);
   I2C_Wait_Ack();
   I2C_Start();
   I2C_Send_Byte(0xd0+1);
   I2C_Wait_Ack();
   while(len)
   {
     if(len==1) *buffer = I2C_Read_Byte(0);
     else *buffer = I2C_Read_Byte(1);
     len--;
     buffer++;
   }
   I2C_Stop();
   return 0;
}


void MPU6050_Set_PowerMgmt(uint8_t pwr)
{
		MPU6050_I2C_Write_Byte(pwr,MPU6050_PWR_MGMT_1);
}

void MPU6050_Set_Gyro_Fsr(uint8_t fsr)
{
	MPU6050_I2C_Write_Byte(fsr,MPU6050_GYRO_CONFIG);
}

void MPU_Set_Accel_Fsr(uint8_t fsr)
{
	MPU6050_I2C_Write_Byte(fsr,MPU6050_ACCEL_CONFIG);
}

void MPU6050_Set_LPF(uint16_t lpf)
{
	MPU6050_I2C_Write_Byte(lpf,MPU6050_LPF_CONFIG);
}

void MPU6050_Set_SampleRate(uint16_t rate)
{
	MPU6050_I2C_Write_Byte(rate,MPU6050_SMPLRT_DIV);
}

void MPU6050_Init()
{
	uint8_t res;
	I2C_Init();
	MPU6050_Set_PowerMgmt(0x80); // reset 
	HAL_Delay(100);
	MPU6050_Set_PowerMgmt(0x00);
	MPU6050_Set_SampleRate(0x07);
	MPU6050_Set_LPF(0x06);
	MPU6050_Set_Gyro_Fsr(0x18);
	MPU_Set_Accel_Fsr(0x01);
	MPU6050_I2C_Write_Byte(MPU6050_INT_EN,0x00);
	MPU6050_I2C_Write_Byte(MPU6050_FIFO_EN,0x00);
	MPU6050_I2C_Write_Byte(MPU6050_INTBP_CFG,0x80);
	MPU6050_I2C_Write_Byte(MPU6050_USER_CTRL,0x00);
	res = MPU6050_I2C_Read_Byte(MPU6050_DEVICE_ID);
	if (res != MPU6050_ADDRESS)
	{
		Error_Handler();
	}
}

short MPU6050_Get_Temperature()
{
	 uint8_t buf[2]; 
   short raw;
   float temp;
   MPU6050_I2C_Read_Len(2,buf,MPU6050_TEMP_OUT_H); 
   raw = ((uint16_t)buf[0]<<8)|buf[1];  
   temp = 36.53+((double)raw)/340;  
   return temp*100;
}

uint8_t MPU6050_Get_Gyroscope(short *gx, short *gy, short *gz)
{
	uint8_t buf[6],res;
  res = MPU6050_I2C_Read_Len(6,buf,MPU6050_GYRO_XOUT_H);
  if(res == 0)
  {
    *gx = ((uint16_t)buf[0]<<8)|buf[1];  
    *gy = ((uint16_t)buf[2]<<8)|buf[3];  
    *gz = ((uint16_t)buf[4]<<8)|buf[5];
  }   
  return res;
}

uint8_t MPU6050_Get_Accelerator(short *ax, short *ay, short *az)
{
	 uint8_t buf[6],res;  
   res = MPU6050_I2C_Read_Len(6,buf,MPU6050_ACCEL_XOUT_H);
   if(res == 0)
   {
      *ax = ((uint16_t)buf[0]<<8)|buf[1];  
      *ay = ((uint16_t)buf[2]<<8)|buf[3];  
      *az = ((uint16_t)buf[4]<<8)|buf[5];
   }   
   return res;
}

