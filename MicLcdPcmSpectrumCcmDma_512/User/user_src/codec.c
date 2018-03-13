#include "../user_inc/codec.h"


// Writes a byte at a specific Codec register
// Device: device address
// Addr: register address
// Data: data to be written to the specific register
// Return: 0x00 if write operation is OK
//         0xFF if timeout condition occured (device not connected or bus error).  
uint8_t CODEC_I2C_SingleRandomWrite(I2C_TypeDef* I2Cx, uint8_t Device, uint16_t Addr, uint8_t Data) 
{
	uint32_t timeout = TIMEOUT_MAX;

	I2C_GenerateSTART(I2Cx, ENABLE); // Generate the Start Condition

	// Test on I2C1 EV5 and clear it 
	timeout = TIMEOUT_MAX;
	while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
		if ((timeout--) == 0) // If the timeout delay is exceeded, exit with error code
			return 0xFF;

	// Send Codec selected device slave Address for write
	I2C_Send7bitAddress ( I2Cx, Device, I2C_Direction_Transmitter );

	// Test on I2Cx EV6 and clear it
	timeout = TIMEOUT_MAX; 
	while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTING)) 
		if ((timeout--) == 0) // If the timeout delay is exceeded, exit with error code
			return 0xFF;

	I2C_SendData(I2Cx,(uint8_t)(Addr)); // Send I2Cx location address LSB

	// Test on I2Cx EV8 and clear it 
	timeout = TIMEOUT_MAX; 
	while (!I2C_CheckEvent ( I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED )) 
		if ((timeout--) == 0) // If the timeout delay is exceeded, exit with error code 
			return 0xFF;
    
	I2C_SendData(I2Cx, Data); // Send Data

	// Test on I2Cx EV8 and clear it 
	timeout = TIMEOUT_MAX; 
	while (!I2C_CheckEvent ( I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) 
		if ((timeout--) == 0) // If the timeout delay is exceeded, exit with error code 
			return 0xFF;

	I2C_GenerateSTOP(I2Cx, ENABLE); // Send I2Cx STOP Condition
        
	return 0;
}


// Reads a byte from a specific Codec register
// Device: device address
// Addr: register address
// Return: data read from the specific register or 0xFF if timeout condition occured
uint8_t CODEC_I2C_SingleRandomRead(I2C_TypeDef* I2Cx, uint8_t Device, uint16_t Addr) 
{
	uint32_t timeout = TIMEOUT_MAX;
	uint8_t Data = 0;
    
	I2C_GenerateSTART(I2Cx, ENABLE); // Generate the Start Condition 

	// Test on I2Cx EV5 and clear it
	timeout = TIMEOUT_MAX; 
	while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
		if ((timeout--) == 0) // If the timeout delay is exceeded, exit with error code
			return 0xFF;

	// Send Codec selected device slave Address for write 
	I2C_Send7bitAddress(I2Cx, Device, I2C_Direction_Transmitter);

	// Test on I2Cx EV6 and clear it
	timeout = TIMEOUT_MAX; 
	while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) 
		if ((timeout--) == 0) // If the timeout delay is exceeded, exit with error code
			return 0xFF;

	// Send I2Cx location address LSB 
	I2C_SendData(I2Cx, (uint8_t)(Addr));

	// Test on I2Cx EV8 and clear it 
	timeout = TIMEOUT_MAX; 
	while (!I2C_CheckEvent (I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) 
		if ((timeout--) == 0) // If the timeout delay is exceeded, exit with error code
			return 0xFF;

	I2Cx->SR1 |= (uint16_t) 0x0400; // Clear AF flag if arised
	I2C_GenerateSTOP(I2Cx, ENABLE); // Generate the Stop Condition
	I2C_GenerateSTART(I2Cx, ENABLE); // Generate the Start Condition

	// Test on I2Cx EV6 and clear it
	timeout = TIMEOUT_MAX; 
	while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
		if ((timeout--) == 0) // If the timeout delay is exceeded, exit with error code
			return 0xFF;

	// Send Codec selected device slave Address for write
	I2C_Send7bitAddress(I2Cx, Device, I2C_Direction_Receiver);

	// Test on I2Cx EV6 and clear it
	timeout = TIMEOUT_MAX;
	while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)) 
		if ((timeout--) == 0) // If the timeout delay is exceeded, exit with error code
			return 0xFF;

	// Prepare an NACK for the next data received
	I2C_AcknowledgeConfig(I2Cx, DISABLE);

	// Test on I2Cx EV7 and clear it
	timeout = TIMEOUT_MAX;
	while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED)) 
		if ((timeout--) == 0) // If the timeout delay is exceeded, exit with error code
			return 0xFF;

	I2C_GenerateSTOP(I2Cx, ENABLE); // Prepare Stop after receiving data
	Data = I2C_ReceiveData(I2Cx); // Receive the Data
        
	return Data;
}

// Sets higher or lower the codec volume level.
// Volume: a byte value from 0 to 255 (refer to codec registers description for more details).
// Return: 0 if correct communication, else wrong communication
uint32_t CODEC_VolumeCtrl(uint8_t Volume) 
{
	uint32_t counter = 0;
	if (Volume > 0xE6) 
    {
		// Set the Master volume
		counter += CODEC_I2C_SingleRandomWrite(I2C1, CODEC_ADDRESS, 0x20, Volume - 0xE7);
		counter += CODEC_I2C_SingleRandomWrite(I2C1, CODEC_ADDRESS, 0x21, Volume - 0xE7);
	} else 
    {
		// Set the Master volume
		counter += CODEC_I2C_SingleRandomWrite(I2C1, CODEC_ADDRESS, 0x20, Volume + 0x19);
		counter += CODEC_I2C_SingleRandomWrite(I2C1, CODEC_ADDRESS, 0x21, Volume + 0x19);
	}
	return counter;
}

void CODEC_CtrlLinesConfig() 
{
	// PA4, PB6, PB9, PC7, PC10, PC12, PD4 clock enable
	RCC_AHB1PeriphClockCmd(CODEC_I2C_SCL_RCC |
						   CODEC_I2C_SDA_RCC |
						   CODEC_I2S_MCK_RCC |
						   CODEC_I2S_SCK_RCC |
						   CODEC_I2S_SD_RCC  |
						   CODEC_I2S_WS_RCC  |
						   CODEC_RESET_RCC, ENABLE );
    
    GPIO_InitTypeDef GPIO_InitStruct;

	// I2C1_SCL (PB6)
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = CODEC_I2C_SCL_PIN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(CODEC_I2C_SCL_PORT, &GPIO_InitStruct);
	GPIO_PinAFConfig(CODEC_I2C_SCL_PORT, CODEC_I2C_SCL_PINSOURCE, CODEC_I2C_SCL_AF);

	// I2C1_SDA (PB9)
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStruct.GPIO_Pin = CODEC_I2C_SDA_PIN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(CODEC_I2C_SDA_PORT, &GPIO_InitStruct);
	GPIO_PinAFConfig(CODEC_I2C_SDA_PORT, CODEC_I2C_SDA_PINSOURCE, CODEC_I2C_SDA_AF);

	// Codec I2S3_MCK (PC7)
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = CODEC_I2S_MCK_PIN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_Init(CODEC_I2S_MCK_PORT, &GPIO_InitStruct);
	GPIO_PinAFConfig(CODEC_I2S_MCK_PORT, CODEC_I2S_MCK_PINSOURCE, CODEC_I2S_MCK_AF);

	// Codec I2S3_SCK (PC10)
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = CODEC_I2S_SCK_PIN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_Init(CODEC_I2S_SCK_PORT, &GPIO_InitStruct);
	GPIO_PinAFConfig(CODEC_I2S_SCK_PORT, CODEC_I2S_SCK_PINSOURCE, CODEC_I2S_SCK_AF);

	// Codec I2S3_SD (PC12)
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = CODEC_I2S_SD_PIN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_Init(CODEC_I2S_SD_PORT, &GPIO_InitStruct);
	GPIO_PinAFConfig(CODEC_I2S_SD_PORT, CODEC_I2S_SD_PINSOURCE, CODEC_I2S_SD_AF);

	// I2S3_WS (PA4) 
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = CODEC_I2S_WS_PIN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_Init(CODEC_I2S_WS_PORT, &GPIO_InitStruct);
	GPIO_PinAFConfig(CODEC_I2S_WS_PORT, CODEC_I2S_WS_PINSOURCE, CODEC_I2S_WS_AF);

	// Codec reset pin (PD4)
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = CODEC_RESET_PIN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(CODEC_RESET_PORT, &GPIO_InitStruct);
}

// I2C1 (codec control interface) initialization
void CODEC_I2CConfig() 
{
	RCC_APB1PeriphClockCmd(CODEC_I2C_RCC, ENABLE);

    I2C_InitTypeDef	I2C_InitStruct;
	I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStruct.I2C_ClockSpeed = 30000;
	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStruct.I2C_OwnAddress1 = 1;
	I2C_Init(CODEC_I2C_PORT, &I2C_InitStruct);

	I2C_Cmd(CODEC_I2C_PORT, ENABLE);
}

// I2S3 (SPI3) initialization (Codec)
void CODEC_I2SConfig() 
{
    RCC_APB1PeriphClockCmd(CODEC_I2S_RCC, ENABLE);
    
	I2S_InitTypeDef	I2S_InitStruct;
	I2S_InitStruct.I2S_AudioFreq = Filter.Fs / 2;
	I2S_InitStruct.I2S_CPOL = I2S_CPOL_Low;
	I2S_InitStruct.I2S_DataFormat = I2S_DataFormat_16b;
	I2S_InitStruct.I2S_MCLKOutput = I2S_MCLKOutput_Enable;
	I2S_InitStruct.I2S_Mode = I2S_Mode_MasterTx;
	I2S_InitStruct.I2S_Standard = I2S_Standard_Phillips;
	I2S_Init(CODEC_I2S_PORT, &I2S_InitStruct);
	I2S_Cmd(CODEC_I2S_PORT, ENABLE);
}

void CODEC_Config() 
{
	// Keep Codec powered OFF
	CODEC_I2C_SingleRandomWrite(I2C1, CODEC_ADDRESS, 0x02, 0x01);
	CODEC_I2C_SingleRandomWrite(I2C1, CODEC_ADDRESS, 0x04, 0xAF);	// SPK always OFF & HP always ON
    
	// Clock configuration: Auto detection
	CODEC_I2C_SingleRandomWrite(I2C1, CODEC_ADDRESS, 0x05, 0x81);
    
	// Set the Slave Mode and the audio Standard
	CODEC_I2C_SingleRandomWrite(I2C1, CODEC_ADDRESS, 0x06, 0x04);
    
	// Set the Master volume
	CODEC_VolumeCtrl(VOLUME_CONVERT(90));
    
	// Power on the Codec
	CODEC_I2C_SingleRandomWrite(I2C1, CODEC_ADDRESS, 0x02, 0x9E);
    
	// Additional configuration for the CODEC. These configurations are done to reduce
	// the time needed for the Codec to power off. If these configurations are removed,
	// then a long delay should be added between powering off the Codec and switching
	// off the I2S peripheral MCLK clock (which is the operating clock for Codec).
	// If this delay is not inserted, then the codec will not shut down properly and
	// it results in high noise after shut down.

	// Disable the analog soft ramp
	CODEC_I2C_SingleRandomWrite(I2C1, CODEC_ADDRESS, 0x0A, 0x00);
    
	// Disable the digital soft ramp
	CODEC_I2C_SingleRandomWrite(I2C1, CODEC_ADDRESS, 0x0E, 0x04);
    
	// Disable the limiter attack level
	CODEC_I2C_SingleRandomWrite(I2C1, CODEC_ADDRESS, 0x27, 0x00);
    
	// Adjust Bass and Treble levels
	CODEC_I2C_SingleRandomWrite(I2C1, CODEC_ADDRESS, 0x1F, 0x0F);
    
	// Adjust PCM volume level
	CODEC_I2C_SingleRandomWrite(I2C1, CODEC_ADDRESS, 0x1A, 0x0A);
	CODEC_I2C_SingleRandomWrite(I2C1, CODEC_ADDRESS, 0x1B, 0x0A);
}


void CODEC_DMAConfig() 
{
	RCC_AHB1PeriphClockCmd(CODEC_DMA_RCC, ENABLE);
    
	DMA_InitTypeDef	DMA_InitStruct;
	DMA_DeInit(CODEC_DMA_STREAM);
	DMA_InitStruct.DMA_Channel = CODEC_DMA_CHANNEL;
	DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&CODEC_I2S_PORT->DR;
	DMA_InitStruct.DMA_Memory0BaseAddr = (uint32_t) AudioBuffer;
	DMA_InitStruct.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	DMA_InitStruct.DMA_BufferSize = 2 * PCM_OUT_SIZE;
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStruct.DMA_Priority = DMA_Priority_Low;
	DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStruct.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
	DMA_InitStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(CODEC_DMA_STREAM, &DMA_InitStruct);
	DMA_Cmd(CODEC_DMA_STREAM, ENABLE);

	SPI_I2S_DMACmd(CODEC_I2S_PORT, SPI_I2S_DMAReq_Tx, ENABLE);
}

void CODEC_DMA_NVIC_Config() 
{
	DMA_ITConfig(CODEC_DMA_STREAM, DMA_IT_TC, ENABLE);
	NVIC_PriorityGroupConfig(NVIC_GROUP);

    NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = CODEC_IRQ_CHANNEL;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStruct);
}

void CODEC_Init() 
{
	CODEC_CtrlLinesConfig();

	// Reset codec
	GPIO_ResetBits(CODEC_RESET_PORT, CODEC_RESET_PIN);
	Delay(100);
	GPIO_SetBits(CODEC_RESET_PORT, CODEC_RESET_PIN);

	CODEC_I2SConfig();
	CODEC_I2CConfig();
	CODEC_Config();
	CODEC_DMAConfig();
	CODEC_DMA_NVIC_Config();
}
