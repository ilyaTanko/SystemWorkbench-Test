/**
  ******************************************************************************
  * @file    DCMI/OVM7690_Camera/dcmi_ov9655.h
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    18-April-2011
  * @brief   Header for dcmi_ov9655.c module
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DCMI_OVM7690_H
#define __DCMI_OVM7690_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

/* Exported types ------------------------------------------------------------*/
typedef struct
{
  uint8_t Manufacturer_ID1;
  uint8_t Manufacturer_ID2;
  uint8_t Version;
  uint8_t PID; 
} OVM7690_IDTypeDef;

typedef struct reginfo {
	u8 reg;
	u8 val;
} reginfo_t;

/* Exported constants --------------------------------------------------------*/

/* Use this define to set the maximum delay timeout for the I2C DCMI_SingleRandomWrite()
   and DCMI_SingleRandomRead() operations. Exeeding this timeout delay, the read/write 
   functions will be aborted and return error code (0xFF).
   The period of the delay will depend on the system operating frequency. The following
   value has been set for system running at 120MHz. */
#define DCMI_TIMEOUT_MAX  10000

#define OVM7690_DEVICE_WRITE_ADDRESS    0x42
#define OVM7690_DEVICE_READ_ADDRESS     0x43

///* OVM7690 Registers definition */
//#define OVM7690_GAIN       0x00
//#define OVM7690_BLUE       0x01
//#define OVM7690_RED        0x02
//#define OVM7690_VREF       0x03
//#define OVM7690_COM1       0x04
//#define OVM7690_BAVE       0x05
//#define OVM7690_GbAVE      0x06
//#define OVM7690_GrAVE      0x07
//#define OVM7690_RAVE       0x08
//#define OVM7690_COM2       0x09
//#define OVM7690_PID        0x0A
//#define OVM7690_VER        0x0B
//#define OVM7690_COM3       0x0C
//#define OVM7690_COM4       0x0D
//#define OVM7690_COM5       0x0E
//#define OVM7690_COM6       0x0F
//#define OVM7690_AEC        0x10
//#define OVM7690_CLKRC      0x11
//#define OVM7690_COM7       0x12
//#define OVM7690_COM8       0x13
//#define OVM7690_COM9       0x14
//#define OVM7690_COM10      0x15
//#define OVM7690_REG16      0x16
//#define OVM7690_HSTART     0x17
//#define OVM7690_HSTOP      0x18
//#define OVM7690_VSTART     0x19
//#define OVM7690_VSTOP      0x1A
//#define OVM7690_PSHFT      0x1B
//#define OVM7690_MIDH       0x1C
//#define OVM7690_MIDL       0x1D
//#define OVM7690_MVFP       0x1E
//#define OVM7690_BOS        0x20
//#define OVM7690_GBOS       0x21
//#define OVM7690_GROS       0x22
//#define OVM7690_ROS        0x23
//#define OVM7690_AEW        0x24
//#define OVM7690_AEB        0x25
//#define OVM7690_VPT        0x26
//#define OVM7690_BBIAS      0x27
//#define OVM7690_GbBIAS     0x28
//#define OVM7690_PREGAIN    0x29
//#define OVM7690_EXHCH      0x2A
//#define OVM7690_EXHCL      0x2B
//#define OVM7690_RBIAS      0x2C
//#define OVM7690_ADVFL      0x2D
//#define OVM7690_ADVFH      0x2E
//#define OVM7690_YAVE       0x2F
//#define OVM7690_HSYST      0x30
//#define OVM7690_HSYEN      0x31
//#define OVM7690_HREF       0x32
//#define OVM7690_CHLF       0x33
//#define OVM7690_AREF1      0x34
//#define OVM7690_AREF2      0x35
//#define OVM7690_AREF3      0x36
//#define OVM7690_ADC1       0x37
//#define OVM7690_ADC2       0x38
//#define OVM7690_AREF4      0x39
//#define OVM7690_TSLB       0x3A
//#define OVM7690_COM11      0x3B
//#define OVM7690_COM12      0x3C
//#define OVM7690_COM13      0x3D
//#define OVM7690_COM14      0x3E
//#define OVM7690_EDGE       0x3F
//#define OVM7690_COM15      0x40
//#define OVM7690_COM16      0x41
//#define OVM7690_COM17      0x42
//#define OVM7690_MTX1       0x4F
//#define OVM7690_MTX2       0x50
//#define OVM7690_MTX3       0x51
//#define OVM7690_MTX4       0x52
//#define OVM7690_MTX5       0x53
//#define OVM7690_MTX6       0x54
//#define OVM7690_BRTN       0x55
//#define OVM7690_CNST1      0x56
//#define OVM7690_CNST2      0x57
//#define OVM7690_MTXS       0x58
//#define OVM7690_AWBOP1     0x59
//#define OVM7690_AWBOP2     0x5A
//#define OVM7690_AWBOP3     0x5B
//#define OVM7690_AWBOP4     0x5C
//#define OVM7690_AWBOP5     0x5D
//#define OVM7690_AWBOP6     0x5E
//#define OVM7690_BLMT       0x5F
//#define OVM7690_RLMT       0x60
//#define OVM7690_GLMT       0x61
//#define OVM7690_LCC1       0x62
//#define OVM7690_LCC2       0x63
//#define OVM7690_LCC3       0x64
//#define OVM7690_LCC4       0x65
//#define OVM7690_MANU       0x66
//#define OVM7690_MANV       0x67
//#define OVM7690_MANY       0x68
//#define OVM7690_VARO       0x69
//#define OVM7690_BD50MAX    0x6A
//#define OVM7690_DBLV       0x6B
//#define OVM7690_DNSTH      0x70
//#define OVM7690_POIDX      0x72
//#define OVM7690_PCKDV      0x73
//#define OVM7690_XINDX      0x74
//#define OVM7690_YINDX      0x75
//#define OVM7690_SLOP       0x7A
//#define OVM7690_GAM1       0x7B
//#define OVM7690_GAM2       0x7C
//#define OVM7690_GAM3       0x7D
//#define OVM7690_GAM4       0x7E
//#define OVM7690_GAM5       0x7F
//#define OVM7690_GAM6       0x80
//#define OVM7690_GAM7       0x81
//#define OVM7690_GAM8       0x82
//#define OVM7690_GAM9       0x83
//#define OVM7690_GAM10      0x84
//#define OVM7690_GAM11      0x85
//#define OVM7690_GAM12      0x86
//#define OVM7690_GAM13      0x87
//#define OVM7690_GAM14      0x88
//#define OVM7690_GAM15      0x89
//#define OVM7690_COM18      0x8B
//#define OVM7690_COM19      0x8C
//#define OVM7690_COM20      0x8D
//#define OVM7690_DMLNL      0x92
//#define OVM7690_DMLNH      0x93
//#define OVM7690_LCC6       0x9D
//#define OVM7690_LCC7       0x9E
//#define OVM7690_AECH       0xA1
//#define OVM7690_BD50       0xA2
//#define OVM7690_BD60       0xA3
//#define OVM7690_COM21      0xA4
//#define OVM7690_GREEN      0xA6
//#define OVM7690_VZST       0xA7
//#define OVM7690_REFA8      0xA8
//#define OVM7690_REFA9      0xA9
//#define OVM7690_BLC1       0xAC
//#define OVM7690_BLC2       0xAD
//#define OVM7690_BLC3       0xAE
//#define OVM7690_BLC4       0xAF
//#define OVM7690_BLC5       0xB0
//#define OVM7690_BLC6       0xB1
//#define OVM7690_BLC7       0xB2
//#define OVM7690_BLC8       0xB3
//#define OVM7690_CTRLB4     0xB4
//#define OVM7690_FRSTL      0xB7
//#define OVM7690_FRSTH      0xB8
//#define OVM7690_ADBOFF     0xBC
//#define OVM7690_ADROFF     0xBD
//#define OVM7690_ADGbOFF    0xBE
//#define OVM7690_ADGrOFF    0xBF
//#define OVM7690_COM23      0xC4
//#define OVM7690_BD60MAX    0xC5
//#define OVM7690_COM24      0xC7
//
///* Registers bit definition */
///* COM1 Register */
//#define CCIR656_FORMAT  0x40
//#define HREF_SKIP_0     0x00
//#define HREF_SKIP_1     0x04
//#define HREF_SKIP_3     0x08
//
///* COM2 Register */
//#define SOFT_SLEEP_MODE  0x10
//#define ODCAP_1x         0x00
//#define ODCAP_2x         0x01
//#define ODCAP_3x         0x02
//#define ODCAP_4x         0x03
//
///* COM3 Register */
//#define COLOR_BAR_OUTPUT         0x80
//#define OUTPUT_MSB_LAS_SWAP      0x40
//#define PIN_REMAP_RESETB_EXPST   0x08
//#define RGB565_FORMAT            0x00
//#define RGB_OUTPUT_AVERAGE       0x04
//#define SINGLE_FRAME             0x01
//
///* COM5 Register */
//#define SLAM_MODE_ENABLE      0x40
//#define EXPOSURE_NORMAL_MODE  0x01
//
///* COM7 Register */
//#define SCCB_REG_RESET                       0x80
//#define FORMAT_CTRL_15fpsVGA                 0x00
//#define FORMAT_CTRL_30fpsVGA_NoVArioPixel    0x50
//#define FORMAT_CTRL_30fpsVGA_VArioPixel      0x60
//#define OUTPUT_FORMAT_RAWRGB                 0x00
//#define OUTPUT_FORMAT_RAWRGB_DATA            0x00
//#define OUTPUT_FORMAT_RAWRGB_INTERP          0x01
//#define OUTPUT_FORMAT_YUV                    0x02
//#define OUTPUT_FORMAT_RGB                    0x03
//
///* COM9 Register */
//#define GAIN_2x         0x00
//#define GAIN_4x         0x10
//#define GAIN_8x         0x20
//#define GAIN_16x        0x30
//#define GAIN_32x        0x40
//#define GAIN_64x        0x50
//#define GAIN_128x       0x60
//#define DROP_VSYNC      0x04
//#define DROP_HREF       0x02
//
///* COM10 Register */
//#define RESETb_REMAP_SLHS    0x80
//#define HREF_CHANGE_HSYNC    0x40
//#define PCLK_ON              0x00
//#define PCLK_OFF             0x20
//#define PCLK_POLARITY_REV    0x10
//#define HREF_POLARITY_REV    0x08
//#define RESET_ENDPOINT       0x04
//#define VSYNC_NEG            0x02
//#define HSYNC_NEG            0x01
//
///* TSLB Register */
//#define PCLK_DELAY_0         0x00
//#define PCLK_DELAY_2         0x40
//#define PCLK_DELAY_4         0x80
//#define PCLK_DELAY_6         0xC0
//#define OUTPUT_BITWISE_REV   0x20
//#define UV_NORMAL            0x00
//#define UV_FIXED             0x10
//#define YUV_SEQ_YUYV         0x00
//#define YUV_SEQ_YVYU         0x02
//#define YUV_SEQ_VYUY         0x04
//#define YUV_SEQ_UYVY         0x06
//#define BANDING_FREQ_50      0x02
//
///* COM15 Register */
//#define OUTPUT_RANGE__10_F0	0x00
//#define OUTPUT_RANGE__01_FE	0x80
//#define OUTPUT_RANGE__00_FF	0xC0
//#define RGB_NORMAL   0x00
//#define RGB_565      0x10
//#define RGB_555      0x30


/* OVM7690 Registers definition when DSP bank selected (0xFF = 0x00) */

/* OVM7690 Registers definition when sensor bank selected (0xFF = 0x01) */
	/* Gain lower 8 bits (rest in vref) */
#define OVM7690_GAIN			0x00
#define OVM7690_BLUE			0x01    /* blue gain */
#define OVM7690_RED			0x02    /* red gain */
#define OVM7690_VREF			0x03    /* Pieces of GAIN, VSTART, VSTOP */
#define OVM7690_COM1			0x04    /* Control 1 */
#define  COM1_CCIR656		 0x40    /* CCIR656 enable */
#define  COM1_HREF_SKIP_0	 0x00
#define  COM1_HREF_SKIP_1	 0x04
#define  COM1_HREF_SKIP_3	 0x08
#define OVM7690_BAVE			0x05    /* U/B Average level */
#define OVM7690_GbAVE		0x06    /* Y/Gb Average level */
#define OVM7690_AECHH		0x07    /* AEC MS 5 bits */
#define OVM7690_RAVE			0x08    /* V/R Average level */
#define OVM7690_COM2			0x09    /* Control 2 */
#define  COM2_SSLEEP		 0x10    /* Soft sleep mode */
#define  COM2_ODCAP_1x		 0x00
#define  COM2_ODCAP_2x		 0x01
#define  COM2_ODCAP_3x		 0x02
#define  COM2_ODCAP_4x		 0x03
#define OVM7690_PID			0x0A    /* Product ID MSB */
#define OVM7690_VER			0x0B    /* Product ID LSB */
#define OVM7690_REG0C			0x0C    /* REG0C */
#define  REG0C_VERT_FLIP		 0x80	/* Vertical flip */
#define  REG0C_HORIZ_MIRROR		 0x40	/* Horizontal mirror */
#define  REG0C_BR_SWAP_IN_RGB	 0x20	/* BR swap in RGB format */
#define  REG0C_YUYV_SWAP_IN_YUV	 0x10	/* YU/YV swap in YUV format */
#define	 REG0C_CLK_TRI_STATE	 0x04	/* Clock output pin status: 1 - tri-state, 0 - output state */
#define	 REG0C_DATA_TRI_STATE	 0x02	/* Data output pin status: 1 - tri-state, 0 - output state */
#define	 REG0C_COLOR_BAR		 0x01	/* Color bar output (overlay) */
#define OVM7690_COM4			0x0D    /* Control 4 */
#define OVM7690_COM5			0x0E    /* All "reserved" */
#define	 COM5_SLAM_MODE_ENABLE     0x40
#define	 COM5_EXPOSURE_NORMAL_MODE 0x01
#define OVM7690_COM6			0x0F    /* Control 6 */
#define OVM7690_AECH			0x10    /* More bits of AEC value */
#define OVM7690_REG11		0x11    /* Clock control */
#define  REG11_CLK_EXT_MASK	 0x40    /* Use external clock directly */
#define  REG11_CLK_SCALE_MASK 0x3F    /* Mask for internal clock scale */
#define OVM7690_REG12			0x12    /* REG12 */
#define  REG12_RESET		 0x80    /* Resets system */
#define  REG12_SKIP			 0x40	 /* Skip mode enable */
#define  REG12_ITU656		 0x20    /* ITU656 protocol ON/OFF selection */
#define  REG12_ORIG			 0x10    /* Select sensor original raw data output */
#define  REG12_RGB565		  0x04    /* RGB565 output format */
#define  REG12_RGB444		  0x0C    /* RGB444 output format */
#define  REG12_YUV			   0x00    /* YUV output format */
#define  REG12_BAYER_1		   0x01    /* RGB output format */
#define  REG12_RGB			   0x02    /* Bayer format */
#define  REG12_BAYER_2		   0x03    /* Bayer format */
#define OVM7690_COM8			0x13    /* Control 8 */
#define  COM8_FASTAEC		 0x80    /* Enable fast AGC/AEC */
#define  COM8_AECSTEP		 0x40    /* Unlimited AEC step size */
#define  COM8_BFILT			 0x20    /* Band filter enable */
#define  COM8_AGC			 0x04    /* Auto gain enable */
#define  COM8_AWB			 0x02    /* White balance enable */
#define  COM8_AEC			 0x01    /* Auto exposure enable */
#define OVM7690_COM9			0x14    /* Control 9  - gain ceiling */
#define	 COM9_AGC_2X		 0x00
#define	 COM9_AGC_4X		 0x10	/* Automatic Gain Ceiling 4X */
#define	 COM9_AGC_8X		 0x20
#define	 COM9_AGC_16X		 0x30
#define	 COM9_AGC_32X		 0x40
#define	 COM9_AGC_64X		 0x50
#define	 COM9_AGC_128X		 0x60
#define	 COM9_DROP_VSYNC	 0x04
#define	 COM9_DROP_HREF		 0x02
#define OVM7690_REG28		0x28    /* REG28 */
#define	 REG28_NEG_DATA		 0x80	 /* Output negative data */
#define  REG28_HSYNC		 0x40    /* HSYNC instead of HREF */
#define  REG28_PCLK_HB		 0x20    /* Suppress PCLK on horiz blank */
#define  REG28_PCLK_REV		 0x10
#define  REG28_HREF_REV		 0x08    /* Reverse HREF */
#define  REG28_VS_LEAD		 0x04    /* VSYNC on clock leading edge */
#define  REG28_VS_NEG		 0x02    /* VSYNC negative */
#define  REG28_HS_NEG		 0x01    /* HSYNC negative */
#define OVM7690_REG16		0x16
#define OVM7690_HSTART		0x17    /* Horiz start high bits */
#define OVM7690_HSIZE		0x18    /* REG18 (Horiz stop high bits) */
#define OVM7690_VSTART		0x19    /* Vert start high bits */
#define OVM7690_VSIZE		0x1A    /* REG1A (Vert stop high bits) */
#define OVM7690_PSHFT		0x1B    /* Pixel delay after HREF */
#define OVM7690_MIDH			0x1C    /* Manuf. ID high */
#define OVM7690_MIDL			0x1D    /* Manuf. ID low */
#define OVM7690_MVFP			0x1E    /* Mirror / vflip */
#define  MVFP_MIRROR		0x20    /* Mirror image */
#define  MVFP_FLIP			0x10    /* Vertical flip */
#define OVM7690_LAEC			0x1F
#define OVM7690_ADCCTR0		0x20
#define OVM7690_ADCCTR1		0x21
#define OVM7690_ADCCTR2		0x22
#define OVM7690_ADCCTR3		0x23
#define OVM7690_AEW			0x24    /* AGC upper limit */
#define OVM7690_AEB			0x25    /* AGC lower limit */
#define OVM7690_VPT			0x26    /* AGC/AEC fast mode op region */
#define OVM7690_BBIAS		0x27
#define OVM7690_GbBIAS		0x28
#define OVM7690_REG29		0x29
#define OVM7690_EXHCH		0x2A
#define OVM7690_EXHCL		0x2B
#define OVM7690_RBIAS		0x2C
#define OVM7690_ADVFL		0x2D
#define OVM7690_ADVFH		0x2E
#define OVM7690_YAVE			0x2F
#define OVM7690_HSYST		0x30    /* HSYNC rising edge delay */
#define OVM7690_HSYEN		0x31    /* HSYNC falling edge delay */
#define OVM7690_HREF			0x32    /* HREF pieces */
#define OVM7690_CHLF			0x33
#define OVM7690_ARBLM		0x34
#define OVM7690_REG35		0x35
#define OVM7690_REG36		0x36
#define OVM7690_ADC			0x37
#define OVM7690_ACOM			0x38
#define OVM7690_OFON			0x39
#define OVM7690_TSLB			0x3A    /* lots of stuff */
#define	 PCLK_DELAY_0		 0x00
#define	 PCLK_DELAY_2		 0x40
#define	 PCLK_DELAY_4		 0x80
#define	 PCLK_DELAY_6		 0xC0
#define	 OUTPUT_BITWISE_REV	 0x20
#define	 UV_NORMAL			 0x00
#define	 UV_FIXED			 0x10
#define	 YUV_SEQ_YUYV		 0x00
#define	 YUV_SEQ_YVYU		 0x02
#define	 YUV_SEQ_VYUY		 0x04	/* UYVY or VYUY - see com13 */
#define	 YUV_SEQ_UYVY		 0x06
#define	 BANDING_FREQ_50	 0x02
#define OVM7690_COM11		0x3B    /* Control 11 */
#define  COM11_NIGHT		 0x80    /* NIght mode enable */
#define  COM11_NMFR			 0x60    /* Two bit NM frame rate */
#define  COM11_HZAUTO		 0x10    /* Auto detect 50/60 Hz */
#define  COM11_50HZ			 0x08    /* Manual 50Hz select */
#define  COM11_EXP			 0x02
#define OVM7690_COM12		0x3C    /* Control 12 */
#define  COM12_HREF			 0x80    /* HREF always */
#define OVM7690_COM13		0x3D    /* Control 13 */
#define  COM13_GAMMA		 0x80    /* Gamma enable */
#define  COM13_UVSAT		 0x40    /* UV saturation auto adjustment */
#define  COM13_UVSWAP		 0x01    /* V before U - w/TSLB */
#define OVM7690_COM14		0x3E    /* Control 14 */
#define  COM14_DCWEN		 0x10    /* DCW/PCLK-scale enable */
#define OVM7690_EDGE			0x3F    /* Edge enhancement factor */
#define OVM7690_COM15		0x40    /* Control 15 */
#define  COM15_R10F0		 0x00    /* Data range 10 to F0 */
#define  COM15_R01FE		 0x80    /*            01 to FE */
#define  COM15_R00FF		 0xC0    /*            00 to FF */
#define  COM15_RGB565		 0x10    /* RGB565 output */
#define  COM15_RGB555		 0x30    /* RGB555 output */
#define OVM7690_COM16		0x41    /* Control 16 */
#define  COM16_AWBGAIN		 0x08    /* AWB gain enable */
#define OVM7690_COM17		0x42    /* Control 17 */
#define  COM17_AECWIN		 0xC0    /* AEC window - must match COM4 */
#define  COM17_CBAR			 0x08    /* DSP Color bar */
#define OVM7690_AWBC1		0x43
#define OVM7690_AWBC2		0x44
#define OVM7690_AWBC3		0x45
#define OVM7690_AWBC4		0x46
#define OVM7690_AWBC5		0x47
#define OVM7690_AWBC6		0x48
#define OVM7690_REG49		0x49
#define OVM7690_REG4A		0x4A
#define OVM7690_REG4B		0x4B
#define OVM7690_DNSTH		0x4C
#define OVM7690_REG4D		0x4D
#define OVM7690_REG4E		0x4E
#define OVM7690_MTX1			0x4F
#define OVM7690_MTX2			0x50
#define OVM7690_MTX3			0x51
#define OVM7690_MTX4			0x52
#define OVM7690_MTX5			0x53
#define OVM7690_MTX6			0x54
#define  CMATRIX_LEN		 6
#define OVM7690_BRIGHT		0x55    /* Brightness */
#define OVM7690_CONTRAST		0x56    /* Contrast control */
#define OVM7690_CONTRASCENTER 0x57
#define OVM7690_CMATRIX_SIGN	0x58
#define OVM7690_REG59		0x59
#define OVM7690_REG5A		0x5A
#define OVM7690_REG5B		0x5B
#define OVM7690_REG5C		0x5C
#define OVM7690_REG5D		0x5D
#define OVM7690_REG5E		0x5E
#define OVM7690_REG5F		0x5F
#define OVM7690_REG60		0x60
#define OVM7690_REG61		0x61
#define OVM7690_LCC1			0x62
#define OVM7690_LCC2			0x63
#define OVM7690_LCC3			0x64
#define OVM7690_LCC4			0x65
#define OVM7690_LCC5			0x66
#define OVM7690_MANU			0x67
#define OVM7690_MANV			0x68
#define OVM7690_GFIX			0x69    /* Fix gain control */
#define OVM7690_GGAIN		0x6A
#define OVM7690_DBLV			0x6B
#define OVM7690_AWBCTR3		0x6C
#define OVM7690_AWBCTR2		0x6D
#define OVM7690_AWBCTR1		0x6E
#define OVM7690_AWBCTR0		0x6F
#define OVM7690_SCL_XSC		0x70    /* Horizontal scale factor */
#define OVM7690_SCL_YSC		0x71    /* Vertical scale factor */
#define OVM7690_SCALING_DCWCTR 0x72
#define OVM7690_SCALING_PC	0x73
#define OVM7690_REG74		0x74
#define OVM7690_REG75		0x75
#define OVM7690_REG76		0x76    /* OV's name */
#define  R76_BLKPCOR		 0x80    /* Black pixel correction enable */
#define  R76_WHTPCOR		 0x40    /* White pixel correction enable */
#define OVM7690_REG77		0x77
#define OVM7690_REG78		0x78
#define OVM7690_REG79		0x79
#define OVM7690_SLOP			0x7A
#define OVM7690_GAM1			0x7B
#define OVM7690_GAM2			0x7C
#define OVM7690_GAM3			0x7D
#define OVM7690_GAM4			0x7E
#define OVM7690_GAM5			0x7F
#define OVM7690_GAM6			0x80
#define OVM7690_GAM7			0x81
#define OVM7690_GAM8			0x82
#define OVM7690_GAM9			0x83
#define OVM7690_GAM10		0x84
#define OVM7690_GAM11		0x85
#define OVM7690_GAM12		0x86
#define OVM7690_GAM13		0x87
#define OVM7690_GAM14		0x88
#define OVM7690_GAM15		0x89
#define OVM7690_REG8A		0x8A
#define OVM7690_REG8B		0x8B
#define OVM7690_RGB444		0x8C    /* RGB 444 control */
#define  R444_ENABLE		 0x02    /* Turn on RGB444, overrides 5x5 */
#define  R444_RGBX			 0x01    /* Empty nibble at end */
#define OVM7690_REG8D		0x8D
#define OVM7690_REG8E		0x8E
#define OVM7690_REG8F		0x8F
#define OVM7690_REG90		0x90
#define OVM7690_REG91		0x91
#define OVM7690_DM_LNL		0x92
#define OVM7690_DM_LNH		0x93
#define OVM7690_LCC6			0x94
#define OVM7690_LCC7			0x95
#define OVM7690_REG96		0x96
#define OVM7690_REG97		0x97
#define OVM7690_REG98		0x98
#define OVM7690_REG99		0x99
#define OVM7690_REG9A		0x9A
#define OVM7690_REG9B		0x9B
#define OVM7690_REG9C		0x9C
#define OVM7690_BD50ST		0x9D
#define OVM7690_BD60ST		0x9E
#define OVM7690_HAECC1		0x9F    /* Hist AEC/AGC control 1 */
#define OVM7690_HAECC2		0xA0    /* Hist AEC/AGC control 2 */
#define OVM7690_REGA1		0xA1
#define OVM7690_SCALING_PCLK_DELAY 0xA2
#define OVM7690_REGA3		0xA3
#define OVM7690_NT_CTRL		0xA4
#define OVM7690_BD50MAX		0xA5    /* 50hz banding step limit */
#define OVM7690_HAECC3		0xA6    /* Hist AEC/AGC control 3 */
#define OVM7690_HAECC4		0xA7    /* Hist AEC/AGC control 4 */
#define OVM7690_HAECC5		0xA8    /* Hist AEC/AGC control 5 */
#define OVM7690_HAECC6		0xA9    /* Hist AEC/AGC control 6 */
#define OVM7690_HAECC7		0xAA   /* Hist AEC/AGC control 7 */
#define OVM7690_BD60MAX		0xAB    /* 60hz banding step limit */
#define OVM7690_STR_OPT		0xAC
#define OVM7690_STR_R		0xAD
#define OVM7690_STR_G		0xAE
#define OVM7690_STR_B		0xAF
#define OVM7690_REGB0		0xB0
#define OVM7690_ABLC1		0xB1
#define OVM7690_REGB2		0xB2
#define OVM7690_THL_DLT		0xB3
#define OVM7690_REGB4		0xB4
#define OVM7690_THL_DLT_AREA	0xB5
#define OVM7690_REGB6		0xB6
#define OVM7690_REGB7		0xB7
#define OVM7690_REGB8		0xB8
#define OVM7690_REGBC		0xBC
#define OVM7690_REGBD		0xBD
#define OVM7690_AD_CHB		0xBE
#define OVM7690_AD_CHR		0xBF
#define OVM7690_AD_CHGb		0xC0
#define OVM7690_AD_CHGr		0xC1
#define OVM7690_REGC2		0xC2
#define OVM7690_REGC3		0xC3
#define OVM7690_REGC4		0xC4
#define OVM7690_REGC5		0xC5
#define OVM7690_REGC6		0xC6
#define OVM7690_REGC7		0xC7
#define OVM7690_REGC8		0xC8
#define OVM7690_SATCTR		0xC9

#define CAMERA_INIT_DELAY				10

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void DCMI_Control_IO_Init(void);
void DCMI_OVM7690_Reset(void);
void DCMI_OVM7690_QVGASizeSetup(void);
void DCMI_OVM7690_ReadID(OVM7690_IDTypeDef* OV9655ID);
void DCMI_OVM7690_SetPrescaler(uint8_t OVM7690_Prescaler);
void DCMI_OVM7690_SelectOutputFormat(uint8_t OVM7690_OuputFormat);
void DCMI_OVM7690_SelectFormatResolution(uint8_t OVM7690_FormatResolution);
void DCMI_OVM7690_SetRegister(uint8_t OVM7690_Register, uint8_t Register_Val);
void DCMI_OVM7690_HREFControl(uint8_t OV9665_HREFControl);
void DCMI_OVM7690_SelectRGBOption(uint8_t OV9665_RGBOption);
uint8_t DCMI_SingleRandomWrite(uint8_t Device, uint16_t Addr, uint8_t Data);
uint8_t DCMI_SingleRandomRead(uint8_t Device, uint16_t Addr);

#endif /* __DCMI_OVM7690_H */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/

