/**
  ******************************************************************************
  * @file    DCMI/OVM7690_Camera/dcmi_OVM7690.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    18-April-2011
  * @brief   This file includes the driver for OVM7690 Camera module mounted on
  *          STM322xG-EVAL board RevA and RevB.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include <dcmi_ovm7690.h>

#include <stm32f4xx_rcc.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_i2c.h>

/** @addtogroup DCMI_OVM7690_Camera
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define  TIMEOUT  1

#define  CAMERA_RST_PIN			  GPIO_Pin_12	
#define  CAMERA_RST_PORT		  GPIOD

#define  CAMERA_PWR_EN_PIN		GPIO_Pin_6
#define  CAMERA_PWR_EN_PORT		GPIOD

/* Bits definitions ----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/* init 640X480 VGA */
static reginfo_t sensor_init_data[] =
{
	/* https://github.com/omegamoon/rockchip-rk30xx-mk808/blob/master/drivers/media/video/ov7690.c */
	{0x0c, 0x06},	// Data pin D[7:0] and HS, VS, PCLK output state
//	{0x81, 0xff},
//	{0x21, 0x44},
//	{0x16, 0x03},
// formate
	{OVM7690_REG12, REG12_RGB | REG12_RGB565},
//	{0x82, 0x03},
//	{0xd0, 0x48},
//	{0x80, 0x7F},
//	{0x3e, 0x30},
//	{0x22, 0x00},
//// resolution
//	{0x17, 0x69},
//	{0x18, 0xa4},
//	{0x19, 0x0c},
//	{0x1a, 0xf6},
//
//	{0xc8, 0x02},
//	{0xc9, 0x80},
//	{0xca, 0x01},
//	{0xcb, 0xe0},
//
//	{0xcc, 0x02},
//	{0xcd, 0x80},
//	{0xce, 0x01},
//	{0xcf, 0xe0},
// Lens Correction
	{0x85, 0x90},
	{0x86, 0x00},
	{0x87, 0x00},
	{0x88, 0x10},
	{0x89, 0x30},
	{0x8a, 0x29},
	{0x8b, 0x26},
//// Color Matrix
//	{0xbb, 0x80},
//	{0xbc, 0x62},
//	{0xbd, 0x1e},
//	{0xbe, 0x26},
//	{0xbf, 0x7b},
//	{0xc0, 0xac},
//	{0xc1, 0x1e},
//// Edge + Denoise
//	{0xb7, 0x05c},
//	{0xb8, 0x09},
//	{0xb9, 0x00},       //0x07//0x27
//	{0xba, 0x18},
//// 	UVAdjust
//	{0x5a, 0x4a},
//	{0x5b, 0x9f},
//	{0x5c, 0x48},
//	{0x5d, 0x32},
//// AEC/AGC target
//	{0x24, 0x78},
//	{0x25, 0x68},
//	{0x26, 0xb3},
// Gamma
	{0xa3, 0x0b},
	{0xa4, 0x15},
	{0xa5, 0x2a},
	{0xa6, 0x51},
	{0xa7, 0x63},
	{0xa8, 0x74},
	{0xa9, 0x83},
	{0xaa, 0x91},
	{0xab, 0x9e},
	{0xac, 0xaa},
	{0xad, 0xbe},
	{0xae, 0xce},
	{0xaf, 0xe5},
	{0xb0, 0xf3},
	{0xb1, 0xfb},
	{0xb2, 0x06},
//simple				// AWB control registers
	{0x8e, 0x92},
	{0x96, 0xf8},
	{0x97, 0x08},
//// Advance
//	{0x8c, 0x5d},
//	{0x8d, 0x11},
//	{0x8e, 0x12},
//	{0x8f, 0x11},
//	{0x90, 0x50},
//	{0x91, 0x22},
//	{0x92, 0xd1},
//	{0x93, 0xa7},
//	{0x94, 0x23},
//	{0x95, 0x3b},
//	{0x96, 0xff},
//	{0x97, 0x00},
//	{0x98, 0x4a},
//	{0x99, 0x46},
//	{0x9a, 0x3d},
//	{0x9b, 0x3a},
//	{0x9c, 0xf0},
//	{0x9d, 0xf0},
//	{0x9e, 0xf0},
//	{0x9f, 0xff},
//	{0xa0, 0x56},
//	{0xa1, 0x55},
//	{0xa2, 0x13},
// General Control
	{0x50, 0x4c},	// 50 Hz banding AEC 8 bits
//	{0x51, 0x3f},
//	{0x21, 0x57},
//	{0x20, 0x00},

	/* Automatic gain ceiling - maximum AGC value == 2x
	 * Manually set banding: 50 Hz
	 */
	{0x14, 0x01},
//	{0x13, 0xf7},
	{0x11, 0x01},	// Internal clock pre-scalar == 0x01 + 1
//	{0xd2, 0x06},

//	{0x2a, 0x30},
//	{0x2b, 0x0b},
//	{0x2c, 0x00},







//	/* https://github.com/manakeri/android_kernel_samsung_xcover/blob/master/common/drivers/media/video/pxa95x-ov7690.c
//	 */
//	{0x12, 0x80},
//	{0x0c, 0x16},
//	{0x48, 0x42},
//	{0x41, 0x43},
//	{0x4c, 0x73},
//	{0x81, 0xff},
//	{0xD8, 0x70},
//	{0xD9, 0x70},
//	{0xD2, 0x02},
//	{0x21, 0x44},
//	{0x16, 0x03},
//	{0x39, 0x80},
//	{0x1e, 0xb1},
//	{0x12, 0x00},
//	{0x82, 0x03},
//	{0xd0, 0x48},
//	{0x80, 0x7f},
//	{0x3e, 0x30},
//	{0x22, 0x00},
//	{0x17, 0x69},
//	{0x18, 0xa4},
//	{0x19, 0x0c},
//	{0x1a, 0xf6},
//	{0xc8, 0x02},
//	{0xc9, 0x80},
//	{0xca, 0x01},
//	{0xcb, 0xe0},
//	{0xcc, 0x02},
//	{0xcd, 0x80},
//	{0xce, 0x01},
//	{0xcf, 0xe0},
//	{0x85, 0x90},
//	{0x86, 0x00},
//	{0x87, 0x00},
//	{0x88, 0x10},
//	{0x89, 0x30},
//	{0x8a, 0x29},
//	{0x8b, 0x26},
//	{0xbb, 0x80},
//	{0xbc, 0x62},
//	{0xbd, 0x1e},
//	{0xbe, 0x26},
//	{0xbf, 0x7b},
//	{0xc0, 0xac},
//	{0xc1, 0x1e},
//	{0xb7, 0x05},
//	{0xb8, 0x09},
//	{0xb9, 0x00},
//	{0xba, 0x18},
//	{0x5A, 0x4A},
//	{0x5B, 0x9F},
//	{0x5C, 0x48},
//	{0x5d, 0x32},
//	{0x24, 0x80},
//	{0x25, 0x70},
//	{0x26, 0xa4},
//	{0xa3, 0x04},
//	{0xa4, 0x09},
//	{0xa5, 0x16},
//	{0xa6, 0x30},
//	{0xa7, 0x3E},
//	{0xa8, 0x4B},
//	{0xa9, 0x59},
//	{0xaa, 0x67},
//	{0xab, 0x72},
//	{0xac, 0x7C},
//	{0xad, 0x8E},
//	{0xae, 0x9E},
//	{0xaf, 0xB6},
//	{0xb0, 0xCC},
//	{0xb1, 0xE2},
//	{0xb2, 0x28},
//	{0x8c, 0x5d},
//	{0x8d, 0x11},
//	{0x8e, 0x12},
//	{0x8f, 0x11},
//	{0x90, 0x50},
//	{0x91, 0x22},
//	{0x92, 0xd1},
//	{0x93, 0xa7},
//	{0x94, 0x23},
//	{0x95, 0x3b},
//	{0x96, 0xff},
//	{0x97, 0x00},
//	{0x98, 0x4a},
//	{0x99, 0x46},
//	{0x9a, 0x3d},
//	{0x9b, 0x3a},
//	{0x9c, 0xf0},
//	{0x9d, 0xf0},
//	{0x9e, 0xf0},
//	{0x9f, 0xff},
//	{0xa0, 0x56},
//	{0xa1, 0x55},
//	{0xa2, 0x13},
//	{0x50, 0x4c},
//	{0x51, 0x3f},
//	{0x21, 0x57},
//	{0x20, 0x00},
//	{0x14, 0x29},
//	{0x13, 0xf7},
//	{0x11, 0x00},
//
//	{0xFF, 0xFF}
};

/* 320*240 QVGA */
static reginfo_t sensor_qvga[] =
{
	/* https://github.com/omegamoon/rockchip-rk30xx-mk808/blob/master/drivers/media/video/ov7690.c */
//	{0x16,0x00},
//	{0x17,0x69},
//	{0x18,0xa4},
//	{0x19,0x06},
//	{0x1a,0xf6},
//	{0x22,0x10},
//	{0xc8,0x02},
//	{0xc9,0x80},
	/* 0x00f0 == 240 vertical size */
	{0xca,0x00},
	{0xcb,0xf0},
	/* 0x0140 == 320 horizontal size */
	{0xcc,0x01},
	{0xcd,0x40},

//	{0xce,0x00},
//	{0xcf,0xf0},
//    {0x00,0x00},

//	/* https://github.com/manakeri/android_kernel_samsung_xcover/blob/master/common/drivers/media/video/pxa95x-ov7690.c */
//	/* 320 x 240 */
//	{0xc8, 0x02},
//	{0xc9, 0x80},
//	{0xca, 0x01},
//	{0xcb, 0xe0},
//	{0xcc, 0x01},
//	{0xcd, 0x40},
//	{0xce, 0x00},
//	{0xcf, 0xf0},
//
//	{0xFF, 0xFF}
};

/*
******************************************************************************
  Function:       DCMI_Control_IO_Init
  Description:    Init DCMI module power control
  Calls:          void
  Called By:      
  Input:          void
  Output:         void
  Return:         
  Others:         
******************************************************************************
*/
void DCMI_Control_IO_Init ( void ) {
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable GPIOD clocks */
	RCC_AHB1PeriphClockCmd ( RCC_AHB1Periph_GPIOD, ENABLE );

	/* camera RST pin configuration */
	GPIO_InitStructure.GPIO_Pin = CAMERA_RST_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init ( CAMERA_RST_PORT, &GPIO_InitStructure );
	/*Reset camera*/
	GPIO_ResetBits ( CAMERA_RST_PORT, CAMERA_RST_PIN );
#ifdef USE_FREERTOS
	vTaskDelay(10);
#else
	Delay(10);
#endif

	GPIO_SetBits ( CAMERA_RST_PORT, CAMERA_RST_PIN );

	/* camera PWR EN pin configuration */
	GPIO_InitStructure.GPIO_Pin = CAMERA_PWR_EN_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_Init ( CAMERA_PWR_EN_PORT, &GPIO_InitStructure );
	GPIO_ResetBits ( CAMERA_PWR_EN_PORT, CAMERA_PWR_EN_PIN );
}

 
/**
  * @brief  Reset the OVM7690 SCCB registers.
  * @param  None
  * @retval None
  */
void DCMI_OVM7690_Reset ( void ) {
	DCMI_SingleRandomWrite ( OVM7690_DEVICE_WRITE_ADDRESS, OVM7690_REG12, REG12_RESET );
}

#ifndef USE_FREERTOS
	void Delay ( uint32_t nTime );
#else
	#define Delay	vTaskDelay
#endif

/**
  * @brief  Set the QVGA size(240*320).
  * @param  None
  * @retval None
  */
void DCMI_OVM7690_QVGASizeSetup(void) {
	int iIndex;

	for ( iIndex = 0;
		  iIndex < sizeof(sensor_init_data) / sizeof(reginfo_t);
		  iIndex++) {
			DCMI_SingleRandomWrite ( OVM7690_DEVICE_WRITE_ADDRESS,
				sensor_init_data[iIndex].reg,
				sensor_init_data[iIndex].val );
	}

	for ( iIndex = 0;
		  iIndex < sizeof(sensor_qvga) / sizeof(reginfo_t);
		  iIndex++) {
			DCMI_SingleRandomWrite ( OVM7690_DEVICE_WRITE_ADDRESS,
					sensor_qvga[iIndex].reg,
					sensor_qvga[iIndex].val );
	}
}

/**
  * @brief  Read the OVM7690 Manufacturer identifier.
  * @param  OVM7690ID: pointer to the OVM7690 Manufacturer identifier.
  * @retval None
  */
void DCMI_OVM7690_ReadID ( OVM7690_IDTypeDef* OVM7690ID ) {
	OVM7690ID->Manufacturer_ID1 = DCMI_SingleRandomRead ( OVM7690_DEVICE_WRITE_ADDRESS, OVM7690_MIDH );
	OVM7690ID->Manufacturer_ID2 = DCMI_SingleRandomRead ( OVM7690_DEVICE_WRITE_ADDRESS, OVM7690_MIDL );
	OVM7690ID->Version = DCMI_SingleRandomRead ( OVM7690_DEVICE_WRITE_ADDRESS, OVM7690_VER );
	OVM7690ID->PID = DCMI_SingleRandomRead ( OVM7690_DEVICE_WRITE_ADDRESS, OVM7690_PID );
}

/**
  * @brief  Set the Internal Clock Prescaler.
  * @param  OVM7690_Prescaler: the new value of the prescaler.
  *         This parameter can be a value between 0x0 and 0x1F
  * @retval None
  */
void DCMI_OVM7690_SetPrescaler ( uint8_t OVM7690_Prescaler ) {
	DCMI_SingleRandomWrite ( OVM7690_DEVICE_WRITE_ADDRESS, OVM7690_REG11, OVM7690_Prescaler );
}

/**
  * @brief  Select the Output Format.
  * @param  OVM7690_OuputFormat: the Format of the ouput Data.
  *         This parameter can be one of the following values:
  *           @arg OUTPUT_FORMAT_RAWRGB_DATA 
  *           @arg OUTPUT_FORMAT_RAWRGB_INTERP    
  *           @arg OUTPUT_FORMAT_YUV              
  *           @arg OUTPUT_FORMAT_RGB    
  * @retval None
  */
void DCMI_OVM7690_SelectOutputFormat ( uint8_t OVM7690_OuputFormat ) {
	DCMI_SingleRandomWrite ( OVM7690_DEVICE_WRITE_ADDRESS, OVM7690_REG12, OVM7690_OuputFormat );
}

/**
  * @brief  Select the Output Format Resolution.
  * @param  OVM7690_FormatResolution: the Resolution of the ouput Data.
  *         This parameter can be one of the following values:
  *           @arg FORMAT_CTRL_15fpsVGA 
  *           @arg FORMAT_CTRL_30fpsVGA_NoVArioPixel    
  *           @arg FORMAT_CTRL_30fpsVGA_VArioPixel     
  * @retval None
  */
void DCMI_OVM7690_SelectFormatResolution(uint8_t OVM7690_FormatResolution)
{
  DCMI_SingleRandomWrite ( OVM7690_DEVICE_WRITE_ADDRESS, OVM7690_REG12, OVM7690_FormatResolution);
}

/**
  * @brief  Set the new value of OVM7690 registers
  * @param  OVM7690_Register: the OVM7690 Register to be configured.
  * @param  Register_Val: The new value to be set 
  * @retval None
  */
void DCMI_OVM7690_SetRegister(uint8_t OVM7690_Register, uint8_t Register_Val)
{
  DCMI_SingleRandomWrite ( OVM7690_DEVICE_WRITE_ADDRESS, OVM7690_Register, Register_Val);
}

/**
  * @brief  Select the HREF Control signal option
  * @param  OVM7690_HREFControl: the HREF Control signal option.
  *         This parameter can be one of the following value:
  *           @arg OVM7690_HREFControl_Opt1: HREF edge offset to data output.
  *           @arg OVM7690_HREFControl_Opt2: HREF end 3 LSB
  *           @arg OVM7690_HREFControl_Opt3: HREF start 3 LSB
  * @retval None
  */
void DCMI_OVM7690_HREFControl(uint8_t OVM7690_HREFControl)
{
  DCMI_SingleRandomWrite ( OVM7690_DEVICE_WRITE_ADDRESS, OVM7690_HREF, OVM7690_HREFControl);
}

/**
  * @brief  Select the RGB format option
  * @param  OVM7690_RGBOption: the RGB Format option.
  *         This parameter can be one of the following value:
  *           @arg RGB_NORMAL
  *           @arg RGB_565  
  *           @arg RGB_555    
  * @retval None
  */
void DCMI_OVM7690_SelectRGBOption ( uint8_t OVM7690_RGBOption ) {
  DCMI_SingleRandomWrite ( OVM7690_DEVICE_WRITE_ADDRESS, OVM7690_COM15, OVM7690_RGBOption );
}

/**
  * @brief  Writes a byte at a specific Camera register
  * @param  Device: OVM7690 write address.
  * @param  Addr: OVM7690 register address.
  * @param  Data: data to be written to the specific register 
  * @retval 0x00 if write operation is OK. 
  *         0xFF if timeout condition occured (device not connected or bus error).
  */
uint8_t DCMI_SingleRandomWrite ( uint8_t Device, uint16_t Addr, uint8_t Data ) {
	uint32_t timeout = DCMI_TIMEOUT_MAX;

	/* Generate the Start Condition */
	I2C_GenerateSTART ( I2C1, ENABLE );

	/* Test on I2C1 EV5 and clear it */
	timeout = DCMI_TIMEOUT_MAX; /* Initialize timeout value */
	while (!I2C_CheckEvent ( I2C1, I2C_EVENT_MASTER_MODE_SELECT )) {
		/* If the timeout delay is exeeded, exit with error code */
		if ((timeout--) == 0)
			return 0xFF;
	}

	/* Send DCMI selcted device slave Address for write */
	I2C_Send7bitAddress ( I2C1, Device, I2C_Direction_Transmitter );

	/* Test on I2C1 EV6 and clear it */
	timeout = DCMI_TIMEOUT_MAX; /* Initialize timeout value */
	while (!I2C_CheckEvent ( I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED )) {
		/* If the timeout delay is exeeded, exit with error code */
		if ((timeout--) == 0)
			return 0xFF;
	}

	/* Send I2C1 location address LSB */
	I2C_SendData ( I2C1, (uint8_t) (Addr) );

	/* Test on I2C1 EV8 and clear it */
	timeout = DCMI_TIMEOUT_MAX; /* Initialize timeout value */
	while (!I2C_CheckEvent ( I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED )) {
		/* If the timeout delay is exeeded, exit with error code */
		if ((timeout--) == 0)
			return 0xFF;
	}

	/* Send Data */
	I2C_SendData ( I2C1, Data );

	/* Test on I2C1 EV8 and clear it */
	timeout = DCMI_TIMEOUT_MAX; /* Initialize timeout value */
	while (!I2C_CheckEvent ( I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED )) {
		/* If the timeout delay is exeeded, exit with error code */
		if ((timeout--) == 0)
			return 0xFF;
	}

	/* Send I2C1 STOP Condition */
	I2C_GenerateSTOP ( I2C1, ENABLE );

	/* If operation is OK, return 0 */
	return 0;
}

/**
  * @brief  Reads a byte from a specific Camera register 
  * @param  Device: OVM7690 write address.
  * @param  Addr: OVM7690 register address.
  * @retval data read from the specific register or 0xFF if timeout condition 
  *         occured. 
  */
uint8_t DCMI_SingleRandomRead ( uint8_t Device, uint16_t Addr ) {
	uint32_t timeout = DCMI_TIMEOUT_MAX;
	uint8_t Data = 0;

	/* Generate the Start Condition */
	I2C_GenerateSTART ( I2C1, ENABLE );

	/* Test on I2C1 EV5 and clear it */
	timeout = DCMI_TIMEOUT_MAX; /* Initialize timeout value */
	while (!I2C_CheckEvent ( I2C1, I2C_EVENT_MASTER_MODE_SELECT )) {
		/* If the timeout delay is exeeded, exit with error code */
		if ((timeout--) == 0)
			return 0xFF;
	}

	/* Send DCMI selcted device slave Address for write */
	I2C_Send7bitAddress ( I2C1, Device, I2C_Direction_Transmitter );

	/* Test on I2C1 EV6 and clear it */
	timeout = DCMI_TIMEOUT_MAX; /* Initialize timeout value */
	while (!I2C_CheckEvent ( I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED )) {
		/* If the timeout delay is exeeded, exit with error code */
		if ((timeout--) == 0)
			return 0xFF;
	}

	/* Send I2C1 location address LSB */
	I2C_SendData ( I2C1, (uint8_t) (Addr) );

	/* Test on I2C1 EV8 and clear it */
	timeout = DCMI_TIMEOUT_MAX; /* Initialize timeout value */
	while (!I2C_CheckEvent ( I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED )) {
		/* If the timeout delay is exeeded, exit with error code */
		if ((timeout--) == 0)
			return 0xFF;
	}

	/* Clear AF flag if arised */
	I2C1->SR1 |= (uint16_t) 0x0400;

	/* Generate the Start Condition */
	I2C_GenerateSTART ( I2C1, ENABLE );

	/* Test on I2C1 EV6 and clear it */
	timeout = DCMI_TIMEOUT_MAX; /* Initialize timeout value */
	while (!I2C_CheckEvent ( I2C1, I2C_EVENT_MASTER_MODE_SELECT )) {
		/* If the timeout delay is exeeded, exit with error code */
		if ((timeout--) == 0)
			return 0xFF;
	}

	/* Send DCMI selected device slave Address for write */
	I2C_Send7bitAddress ( I2C1, Device, I2C_Direction_Receiver );

	/* Test on I2C1 EV6 and clear it */
	timeout = DCMI_TIMEOUT_MAX; /* Initialize timeout value */
	while (!I2C_CheckEvent ( I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED )) {
		/* If the timeout delay is exeeded, exit with error code */
		if ((timeout--) == 0)
			return 0xFF;
	}

	/* Prepare an NACK for the next data received */
	I2C_AcknowledgeConfig ( I2C1, DISABLE );

	/* Test on I2C1 EV7 and clear it */
	timeout = DCMI_TIMEOUT_MAX; /* Initialize timeout value */
	while (!I2C_CheckEvent ( I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED )) {
		/* If the timeout delay is exeeded, exit with error code */
		if ((timeout--) == 0)
			return 0xFF;
	}

	/* Prepare Stop after receiving data */
	I2C_GenerateSTOP ( I2C1, ENABLE );

	/* Receive the Data */
	Data = I2C_ReceiveData ( I2C1 );

	/* return the read data */
	return Data;
}

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
