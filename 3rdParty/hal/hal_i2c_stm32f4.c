/*
 * hal_i2c_stm32f4.c
 *
 *  Created on: May 5, 2023
 *      Author: mmitr
 */


#include "hal_i2c_stm32f4.h"

extern I2C_HandleTypeDef hi2c2;

#define ATECC608A_RESET_CMD		0x00U
#define ATECC608A_SLEEP_CMD		0x01U
#define ATECC608A_IDLE_CMD		0x02U
#define ATECC608A_SEND_DATA_CMD 0x03U

static bool g_IsDeviceAwake = false;

extern volatile uint8_t RxBufferLen;

ATCA_STATUS hal_i2c_init(ATCAIface iface, ATCAIfaceCfg *cfg)
{
	ATCA_STATUS returnValue;
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
		  returnValue = ATCA_NOT_INITIALIZED;
	  }
	  else
	  {
		  returnValue = ATCA_SUCCESS;
	  }

	return returnValue;

}
ATCA_STATUS hal_i2c_post_init(ATCAIface iface)
{
    return ATCA_SUCCESS;
}
ATCA_STATUS hal_i2c_send(ATCAIface iface, uint8_t address, uint8_t *txdata, int txlength)
{

    ATCAIfaceCfg* cfg = atgetifacecfg(iface);
    uint8_t l_debugVal = 0;
	uint8_t l_result = HAL_OK;

	if (!cfg)
	{
		l_result = ATCA_BAD_PARAM;
	}

	txdata[0] = ATECC608A_SEND_DATA_CMD;
	txlength++;

	if(l_result != ATCA_BAD_PARAM)
	{

		if( HAL_OK != (l_debugVal = HAL_I2C_Master_Transmit(&hi2c2, address, txdata, txlength, I2C_TIMEOUT_VALUE)) )
		{
			l_result = ATCA_BAD_PARAM;
		}
		else
		{
			l_result = ATCA_SUCCESS;
		}

	}

	return l_result;
}
ATCA_STATUS hal_i2c_receive(ATCAIface iface, uint8_t word_address, uint8_t *rxdata, uint16_t *rxlength)
{
#define RX_BUFFER_LENGHT 300U

    ATCAIfaceCfg* cfg = atgetifacecfg(iface);
	uint8_t  l_RxBuffer[RX_BUFFER_LENGHT] = {0};
	uint8_t  l_result = HAL_OK;
    uint8_t l_debugVal = 0;
	uint8_t l_NumberOfRetries = cfg->rx_retries;
	uint8_t l_SlaveAddress = cfg->atcai2c.address;
	uint8_t l_MsgLen = 0xC0;


    if ((NULL == cfg) || (NULL == rxlength) || (NULL == rxdata))
	{
		l_result = ATCA_TRACE(ATCA_BAD_PARAM, "NULL pointer encountered");
		l_result = ATCA_BAD_PARAM;
	}


	if(l_result != ATCA_BAD_PARAM)
	{


		if(l_result == ATCA_SUCCESS )
		{


			while (l_NumberOfRetries--)
			{
				if(HAL_OK != (l_debugVal = HAL_I2C_Master_Receive(&hi2c2, (l_SlaveAddress), &l_RxBuffer[0], l_MsgLen, I2C_TIMEOUT_VALUE)) )

				{
					l_result = ATCA_RX_FAIL;
				}
			}


		}

	}
	RxBufferLen = l_RxBuffer[0];
	if(RxBufferLen > 35)
	{
		RxBufferLen = 0;
	}

	memcpy(rxdata, &l_RxBuffer[0], l_RxBuffer[0]);


	return l_result;

#undef RX_BUFFER_LENGHT
}
/*TODO to be correctly implemented */
ATCA_STATUS hal_i2c_wake(ATCAIface iface)
{

	uint16_t l_DeviceAddress = iface->mIfaceCFG->atcai2c.address;
	uint8_t l_ReturnStatus = ATCA_SUCCESS;
	uint8_t l_DataBuffer[4] = {0x00};
	ATCAIfaceCfg *cfg = atgetifacecfg(iface);
	uint8_t l_NumberOfRetries  = cfg->rx_retries;

	if(false == g_IsDeviceAwake)
	{



		HAL_I2C_IsDeviceReady(&hi2c2, 0x00, 1, 1); // sets SDA=0

		do
		{
			atca_delay_us(cfg->wake_delay);

			if (HAL_OK == HAL_I2C_Master_Receive(&hi2c2, l_DeviceAddress, &l_DataBuffer[0], sizeof(l_DataBuffer), 100)) // 100 is overkill
			    break;
			//HAL_Delay(100);

		} while (l_NumberOfRetries--);

		if (ATCA_SUCCESS != hal_check_wake(l_DataBuffer, sizeof(l_DataBuffer)) )
		{
			l_ReturnStatus = ATCA_COMM_FAIL;
		}
		else
		{
			g_IsDeviceAwake = true;
		}

	}

	return l_ReturnStatus ;
}
ATCA_STATUS hal_i2c_idle(ATCAIface iface)
{
	 uint8_t l_DeviceAddress = iface->mIfaceCFG->atcai2c.address;
	 uint8_t l_Command = ATECC608A_IDLE_CMD;
	 uint8_t l_ReturnStatus = ATCA_COMM_FAIL;

	 if(HAL_OK != HAL_I2C_Master_Transmit(&hi2c2, l_DeviceAddress, &l_Command, sizeof(l_Command), 100))
	 {
		 l_ReturnStatus = ATCA_COMM_FAIL;
	 }
	 else
	 {
		 g_IsDeviceAwake = false;
		 l_ReturnStatus = ATCA_SUCCESS;
	 }


	 return l_ReturnStatus;
}
ATCA_STATUS hal_i2c_sleep(ATCAIface iface)
{
	 uint8_t l_DeviceAddress = iface->mIfaceCFG->atcai2c.address;
	 uint8_t l_Command = ATECC608A_SLEEP_CMD;
	 uint8_t l_ReturnStatus = ATCA_COMM_FAIL;

	 if(HAL_OK != HAL_I2C_Master_Transmit(&hi2c2, l_DeviceAddress, &l_Command, sizeof(l_Command), 100))
	 {
		 l_ReturnStatus = ATCA_COMM_FAIL;
	 }
	 else
	 {
		 l_ReturnStatus = ATCA_SUCCESS;
	 }

	 return l_ReturnStatus;
}
ATCA_STATUS hal_i2c_release(void *hal_data )
{
	   return ATCA_SUCCESS;
}
