/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "CryptoWrapper.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define BUFFER_SIZE 0x1FFFC

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
CRC_HandleTypeDef hcrc;

I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c2;

RNG_HandleTypeDef hrng;

SPI_HandleTypeDef hspi1;

/* USER CODE BEGIN PV */
extern I2C_HandleTypeDef hi2c2;
uint8_t random_number[32];
uint8_t AddressValue = 0;
uint8_t crc_len = 27;
//uint8_t crc_TestVal[5] = { 0x07, 0x02,	0x80,	0x00,	0x00};
				/*  1	 2	   3	4	  5		 6	  7		 8	   9	10	  11	12	  13    14	  15    16	  17    18    19    20    21 */
/*
uint8_t crc_TestVal[31] ={ 0x23, 0xD0, 0x6C, 0x00, 0x00, 0x60, 0x02, 0x60, 0x85, 0xC5, 0x5C, 0xEE, 0x01, 0x39, 0x00, 0xC0, 0x00, 0x00, 0x00,
				  0x83, 0x20, 0x87, 0x20, 0x8F, 0x20, 0xC4, 0x8F, 0x8F, 0x8F, 0x8F, 0x8F };*/
uint8_t crc_TestVal[27] = {0x6C	,0x00,	0x00,	0x60,	0x85,	0xC5	,0x5C,	0xEE,	0x01,	0x39,	0x00,	0xC0,	0x00,	0x00,	0x00,	0x83,	0x20,	0x87, 0x20,
0x8F,	0x20,	0xC4,	0x8F,	0x8F,	0x8F,	0x8F,	0x8F};

const uint8_t SignaturedataNew[70] = {
    0x30, 0x44, 0x02, 0x20, 0x21, 0xA3, 0x57, 0x0E, 0xF2, 0x3C, 0x55, 0xEB, 0xD5, 0xDF, 0x0C, 0x55,
    0xF6, 0xE2, 0xCB, 0x41, 0x07, 0xD3, 0x4F, 0x51, 0x14, 0xE2, 0x3D, 0xB9, 0x1C, 0x9E, 0x5E, 0x4C,
    0x89, 0xB1, 0x01, 0x0E, 0x02, 0x20, 0x3B, 0x54, 0x65, 0x24, 0x44, 0x44, 0xD0, 0x58, 0xFB, 0x95,
    0x61, 0x97, 0xDE, 0x07, 0xA8, 0x17, 0x4F, 0xD0, 0xBD, 0x70, 0x4D, 0xC2, 0xE7, 0x51, 0xEE, 0xB1,
    0x9C, 0x0B, 0xA6, 0x37, 0xBC, 0x5B
};

const uint8_t SignaturedataRSNew [64] =
{		0x44, 0x9D, 0x07, 0x7F, 0xDD, 0xC4, 0xB6, 0xA7,
		0xA1, 0x94, 0x98, 0x86, 0xCD, 0x44, 0xAB, 0xF0,
		0x6B, 0x98, 0x64, 0x12, 0xF1, 0xDF, 0xEC, 0xD8,
		0x6A, 0x88, 0x28, 0x36, 0xBF, 0x63, 0xA3, 0x19,
		0x6D, 0x69, 0xFD, 0x59, 0xB9, 0x3C, 0xDF, 0xA3,
		0x58, 0x48, 0xAF, 0xD7, 0x5D, 0x20, 0x1F, 0x7E,
		0xF6, 0xA2, 0xA5, 0xFD, 0xF8, 0x3D, 0xC6, 0xC5,
		0xE4, 0x65, 0x47, 0xC2, 0xE3, 0x50, 0x2F, 0x80};


const uint8_t SignaturedataRSOld [64] = {0x21, 0xA3, 0x57, 0x0E, 0xF2, 0x3, 0x55,0xEB,
0xD5, 0xDF, 0x0C, 0x55, 0xF6, 0xE2, 0xCB, 0x41,
0x07, 0xD3, 0x4F, 0x51, 0x14, 0xE2, 0x3D, 0xB9,
0x1C, 0x9E, 0x5E, 0x4C, 0x89, 0xB1, 0x01, 0x0E,
0x3B, 0x54, 0x65, 0x24, 0x44, 0x44, 0xD0, 0x58,
0xFB, 0x95, 0x61, 0x97, 0xDE, 0x07, 0xA8, 0x17,
0x4F, 0xD0, 0xBD, 0x70, 0x4D, 0xC2, 0xE7, 0x51,
0xEE, 0xB1, 0x9C, 0x0B, 0xA6, 0x37, 0xBC, 0x5B};


const uint8_t SignaturedataRSOldOutOfBin[64] = {
	/* R */
	0xFA, 0xA6, 0x8B, 0x91, 0x48, 0xC7, 0xB8, 0xC4, 0x61, 0x45, 0x99, 0xEF, 0xC8, 0x61, 0x87, 0xD8,
    0x82, 0x5F, 0xCE, 0x48, 0x1A, 0x9E, 0xB6, 0xDA, 0x4A, 0x60, 0x2E, 0xE4, 0xE8, 0x35, 0x23, 0xD4,
	/* S */
    0x23, 0xAA, 0xB7, 0x80, 0x29, 0x0C, 0x03, 0x02, 0x0C, 0xE8, 0x59, 0xDC, 0xEF, 0x27, 0xC1, 0x51,
    0xF0, 0xD1, 0xAA, 0x6E, 0x42, 0xB0, 0xD3, 0xF8, 0xB4, 0xC3, 0x45, 0xBF, 0xED, 0x7E, 0x0A, 0x5C
};

const uint8_t PublicKeyExtern[64] = {
						0x69,0x47,0x7b,0xfa,0x5a,0x5f,0x08,0x84,
						0x25,0x34,0xbd,0x24,0x79,0x42,0xb2,0x9a,
						0xb6,0xe4,0xdb,0x7a,0x37,0xca,0xc7,0xa0,
						0xb6,0xb7,0x49,0x0d,0x6c,0x01,0x32,0x1a,

						0xf1,0xab,0xc6,0xdb,0x80,0x90,0x70,0x52,
						0x40,0x55,0x6f,0xc1,0x62,0x9a,0xb3,0xd5,
					    0x4c,0x4e,0xac,0x78,0x5f,0x62,0x74,0xe1,
						0x8d,0x64,0x2c,0xd4,0x40,0xc0,0xbf,0x77 };
const uint8_t hash[32] = {
    0x5D, 0x24, 0x33, 0x6A, 0x68, 0x31, 0xEF, 0x93, 0x45, 0xF4, 0x5A, 0x2B, 0x5D, 0x48, 0x4F, 0x3B,
    0x6E, 0x49, 0x4F, 0x76, 0x8C, 0xC1, 0xB7, 0xF4, 0x77, 0xD0, 0x41, 0xC0, 0xA3, 0x1F, 0x11, 0x9F
};

uint8_t crc_TestVal2 = 1;
uint8_t crc_Val_LE[2] = {0};
volatile uint8_t RxBufferLen = 0;
volatile uint8_t idleStatus = 0;
volatile uint8_t DeviceStatus = 0;
uint8_t bufferOut[128] = {0};
uint8_t bufferIn[128] = {0};
uint8_t SerialNumber[9] = {0};
uint8_t TestSecret[32] = {0};
const uint8_t TestSecretEntry[32] ={
0x86, 0x4A, 0x4A ,0x08, 0x1B, 0x10, 0xEF, 0x7D, 0xFC, 0x96, 0x6C, 0xD7, 0xB3, 0x1F, 0x2D, 0x14,
 0x0F, 0xAE, 0xCB, 0x5F, 0xF6, 0xE2, 0x18, 0x27, 0xDF, 0x03, 0x1C, 0x13, 0xE3, 0xA5, 0x97, 0xFA
};
const uint8_t enc_key [32] = {0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
				0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
				0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
				0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA};

const uint8_t cmac_test_data [4] = { 0x30, 0x30, 0x30, 0x30};

uint8_t cmac_test_data_out [16] = {0};
bool isVerified =  false;
atecc608_config_t ReadConfig;
uint8_t *p_ReadConfig = &ReadConfig;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_SPI1_Init(void);
static void MX_I2C2_Init(void);
static void MX_RNG_Init(void);
static void MX_CRC_Init(void);
/* USER CODE BEGIN PFP */
int find_I2C_deviceAddress(void);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_SPI1_Init();
  MX_I2C2_Init();
  MX_RNG_Init();
  MX_CRC_Init();
  /* USER CODE BEGIN 2 */
 // AddressValue = find_I2C_deviceAddress();
  // initialize CryptoAuthLib for an ECC default I2C interface
  uint32_t BufferCount = BUFFER_SIZE /4;
  uint32_t BufferCount2 = 1;
  uint32_t myDataBuffer =(uint32_t*) 0x08040000;
  uint8_t testByte = 0;
  uint32_t R_ChecksumValue = 0;
  const char *p_StartAddress = 0x08040000;
  uint32_t u32_ChecksumValue = 0;
  uint8_t _36thByte = 0;
  uint8_t byteNumber = 36;
  uint8_t ATCA_ReturnStatus = 0;
  memset (bufferOut, 0x00, 128);
      bufferIn [0] = 'a';
      bufferIn [1] = 'b';
      bufferIn [2] = 'c';
      bufferIn [3] = 'd';


//
//// atca_trace_config( fopen("log/file1.txt", "w"));
// //atCRC(crc_len, &crc_TestVal[0], crc_Val_LE);
//u32_ChecksumValue = HAL_CRC_Accumulate(&hcrc, myDataBuffer, BufferCount);
//R_ChecksumValue = BE_to_SE_convert(u32_ChecksumValue);
// sha256( (uint8_t*)0x08000000, 0xFFD0u, bufferOut);
// for(int i = 0; i < 8;i++)
// {
// HAL_RNG_GenerateRandomNumber(&hrng, &random_number[i*4]);
// }


      DeviceStatus = Crypto_u_Init();

//      DeviceStatus = Crypto_u_ReadSerialNumber(SerialNumber);


//      DeviceStatus = Crypto_u_ReadConfigZone(bufferOut);
//
//      for(int i = 0; i < sizeof(ReadConfig); i++)
//      {
//    	  *(p_ReadConfig + i) = bufferOut[i];
//      }
      //DeviceStatus = atcab_write_pubkey(PUBLIC_KEY_SLOT, &PublicKeyExtern[0]);

  //    DeviceStatus = atcab_write_zone(ATCA_ZONE_DATA, (uint16_t)8U, 0, 0, &enc_key[0], 32);
   //   DeviceStatus = atcab_write_enc(14U, 0, TestSecretEntry, enc_key, 8U);
  //     DeviceStatus = atcab_write_zone(ATCA_ZONE_DATA, (uint16_t)12U, 0, 0, &TestSecretEntry[0], 32);
      DeviceStatus  = Crypto_u_Verify_stored(10U, hash, SignaturedataRSOldOutOfBin, &isVerified);
      DeviceStatus = Crypto_u_ReadSecret(TestSecret);
      DeviceStatus = Crypto_u_CalculateCMAC(AES_KEY2_SLOT, 0, cmac_test_data, 4U, cmac_test_data_out, 16U);
     // DeviceStatus = Crypto_u_Verify(hash, SignaturedataRSOldOutOfBin, PublicKeyExtern, &isVerified);
      HAL_Delay(1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  idleStatus = atcab_idle();
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief CRC Initialization Function
  * @param None
  * @retval None
  */
static void MX_CRC_Init(void)
{

  /* USER CODE BEGIN CRC_Init 0 */

  /* USER CODE END CRC_Init 0 */

  /* USER CODE BEGIN CRC_Init 1 */

  /* USER CODE END CRC_Init 1 */
  hcrc.Instance = CRC;
  if (HAL_CRC_Init(&hcrc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CRC_Init 2 */

  /* USER CODE END CRC_Init 2 */

}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.ClockSpeed = 100000;
  hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

/**
  * @brief RNG Initialization Function
  * @param None
  * @retval None
  */
static void MX_RNG_Init(void)
{

  /* USER CODE BEGIN RNG_Init 0 */

  /* USER CODE END RNG_Init 0 */

  /* USER CODE BEGIN RNG_Init 1 */

  /* USER CODE END RNG_Init 1 */
  hrng.Instance = RNG;
  if (HAL_RNG_Init(&hrng) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RNG_Init 2 */

  /* USER CODE END RNG_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(CS_I2C_SPI_GPIO_Port, CS_I2C_SPI_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(OTG_FS_PowerSwitchOn_GPIO_Port, OTG_FS_PowerSwitchOn_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, LD4_Pin|LD3_Pin|LD5_Pin|LD6_Pin
                          |Audio_RST_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : CS_I2C_SPI_Pin */
  GPIO_InitStruct.Pin = CS_I2C_SPI_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(CS_I2C_SPI_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : OTG_FS_PowerSwitchOn_Pin */
  GPIO_InitStruct.Pin = OTG_FS_PowerSwitchOn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(OTG_FS_PowerSwitchOn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : BOOT1_Pin */
  GPIO_InitStruct.Pin = BOOT1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BOOT1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LD4_Pin LD3_Pin LD5_Pin LD6_Pin
                           Audio_RST_Pin */
  GPIO_InitStruct.Pin = LD4_Pin|LD3_Pin|LD5_Pin|LD6_Pin
                          |Audio_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : OTG_FS_OverCurrent_Pin */
  GPIO_InitStruct.Pin = OTG_FS_OverCurrent_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(OTG_FS_OverCurrent_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : MEMS_INT2_Pin */
  GPIO_InitStruct.Pin = MEMS_INT2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(MEMS_INT2_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

int find_I2C_deviceAddress(void)
{
	uint8_t i=0;
	for(i=0;i<255;i++)
	{
		if(HAL_I2C_IsDeviceReady(&hi2c2,i,1,10) == HAL_OK)
		{
			HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
			return i;
			break;
		}
	}
	return -1;//greska
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
