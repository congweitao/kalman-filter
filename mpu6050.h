#ifndef _MPU6050_H
#define _MPU6050_H

#include "stdint.h"

// MPU6050 related register address defination
#define MPU6050_ADDRESS 				0x68  // I2C address, 0x68 default(readonly)
#define MPU6050_PWR_MGMT_1			0x6B 	//power management,0x00 default
#define MPU6050_PWR_MGMT_2			0x6C  // power management 2, control wakeup freq in low power state
#define MPU6050_FIFO_EN					0x23	// 0x00 disable, 0x01 enable
#define MPU6050_INTBP_CFG				0x37   // 
#define MPU6050_INT_EN					0x38  // interrupt enable 0x01, 0x00 disable
#define MPU6050_SMPLRT_DIV			0x19	//sample frequency, 0x07 (125Hz)
#define MPU6050_LPF_CONFIG			0x1A	//low pass filter frequency, 0x06(5Hz)
#define MPU6050_GYRO_CONFIG			0x1B	//gyro 自检及测量范围，典型值：0x18(不自检，2000deg/s)
#define MPU6050_ACCEL_CONFIG		0x1C	//accelerator 自检及测量范围及高通滤波频率，0x01,2G,5Hz
#define	MPU6050_ACCEL_XOUT_H		0x3B
#define	MPU6050_ACCEL_XOUT_L		0x3C
#define	MPU6050_ACCEL_YOUT_H		0x3D
#define	MPU6050_ACCEL_YOUT_L		0x3E
#define	MPU6050_ACCEL_ZOUT_H		0x3F
#define	MPU6050_ACCEL_ZOUT_L		0x40
#define	MPU6050_TEMP_OUT_H			0x41
#define	MPU6050_TEMP_OUT_L			0x42
#define	MPU6050_GYRO_XOUT_H			0x43
#define	MPU6050_GYRO_XOUT_L			0x44
#define	MPU6050_GYRO_YOUT_H			0x45
#define	MPU6050_GYRO_YOUT_L			0x46
#define	MPU6050_GYRO_ZOUT_H			0x47
#define	MPU6050_GYRO_ZOUT_L			0x48
#define MPU6050_USER_CTRL				0x6A  //user control 
#define MPU6050_DEVICE_ID				0x75   // device ID

/* I/O operation */
void MPU6050_I2C_Write_Byte(uint8_t data, uint8_t reg);
uint8_t MPU6050_I2C_Read_Byte(uint8_t reg);
uint8_t MPU6050_I2C_Write_Len(uint8_t len, uint8_t *buffer,uint8_t reg);
uint8_t MPU6050_I2C_Read_Len(uint8_t len, uint8_t *buffer,uint8_t reg);

/* setup mpu6050 clock */
void MPU6050_Set_PowerMgmt(uint8_t pwr);
/* setup gyro patameter
*	fsr: 0, +-250dps;1, +-500dps; 2, +-1000dps; 3, +-2000dps
*/
void MPU6050_Set_Gyro_Fsr(uint8_t fsr);

/* setup accelerator parameter
* fsr: 0, +-2g; 1, +-4g; 2, +-8g; 3, +-16g
*/
void MPU_Set_Accel_Fsr(uint8_t fsr);

/* setup low pass filter frequency */
void MPU6050_Set_LPF(uint16_t lpf);

/* setup sampling frequency */
void MPU6050_Set_SampleRate(uint16_t rate);

/* Initializing MPU6050 */
void MPU6050_Init(void);

/* Sensor data */
short MPU6050_Get_Temperature(void);
uint8_t MPU6050_Get_Gyroscope(short *gx, short *gy, short *gz);
uint8_t MPU6050_Get_Accelerator(short *ax, short *ay, short *az);

#endif

