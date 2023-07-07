/*
 * CryptoWrapper.c
 *
 *  Created on: Jul 4, 2023
 *      Author: mmitr
 */


#include "CryptoWrapper.h"



const uint8_t g_slot4_key[] = {
    0x37, 0x80, 0xe6, 0x3d, 0x49, 0x68, 0xad, 0xe5,
    0xd8, 0x22, 0xc0, 0x13, 0xfc, 0xc3, 0x23, 0x84,
    0x5d, 0x1b, 0x56, 0x9f, 0xe7, 0x05, 0xb6, 0x00,
    0x06, 0xfe, 0xec, 0x14, 0x5a, 0x0d, 0xb1, 0xe3};
const uint8_t PublicKey[]  = {
/* Pad bytes */			0x00,0x00,0x00,0x00,
/*X Coordinates */		0x69,0x47,0x7b,0xfa,0x5a,0x5f,0x08,0x84,
						0x25,0x34,0xbd,0x24,0x79,0x42,0xb2,0x9a,
						0xb6,0xe4,0xdb,0x7a,0x37,0xca,0xc7,0xa0,
						0xb6,0xb7,0x49,0x0d,0x6c,0x01,0x32,0x1a,
/* Pad bytes */			0x00,0x00,0x00,0x00,
/*Y Coordinates */		0xf1,0xab,0xc6,0xdb,0x80,0x90,0x70,0x52,
						0x40,0x55,0x6f,0xc1,0x62,0x9a,0xb3,0xd5,
						0x4c,0x4e,0xac,0x78,0x5f,0x62,0x74,0xe1,
						0x8d,0x64,0x2c,0xd4,0x40,0xc0,0xbf,0x77
};
const uint8_t PrivateKey[]  = {
/* Pad bytes */ 	0x00,0x00,0x00,0x00,
/* X Coordinates */	0xe7,0x32,0xbc,0xf2,0x9e,0xef,0x8d,0xf8,0x0b,0x69,0xeb,0x7c,0x50,0x41,0xcd,0x97,
/* Y Coordinates */ 0x77,0x49,0x5e,0xfa,0xd2,0xd1,0xc6,0x24,0x53,0xef,0xb1,0xaf,0xcc,0x0a,0xf0,0x37
};
const uint8_t AES_Key1[] = {
		0x04,	0x6C,	0x6C,	0x9E,	0x86,	0x21,	0xDE,	0xE4,	0xD6,	0x1F,	0x01,	0xAF,	0xA4,	0xEA,	0xF2,	0x04,
		0x04,	0x6C,	0x6C,	0x9E,	0x86,	0x21,	0xDE,	0xE4,	0xD6,	0x1F,	0x01,	0xAF,	0xA4,	0xEA,	0xF2,	0x04
};

const uint8_t AES_Key2[] = {
		0xE4,	0xA5,	0xE2,	0x74,	0x27,	0xC9,	0x2D,	0xAE,	0x9E,	0xD1,	0x66,	0x8F,	0x73,	0x69,	0xF1,	0xA9,
		0xE4,	0xA5,	0xE2,	0x74,	0x27,	0xC9,	0x2D,	0xAE,	0x9E,	0xD1,	0x66,	0x8F,	0x73,	0x69,	0xF1,	0xA9
};
const uint8_t Secret[] ={
0x86, 0x4A, 0x4A ,0x08, 0x1B, 0x10, 0xEF, 0x7D, 0xFC, 0x96, 0x6C, 0xD7, 0xB3, 0x1F, 0x2D, 0x14,
 0x0F, 0xAE, 0xCB, 0x5F, 0xF6, 0xE2, 0x18, 0x27, 0xDF, 0x03, 0x1C, 0x13, 0xE3, 0xA5, 0x97, 0xFA
};


atecc608_config_t configZoneRead;
atecc608_config_t configZoneWrite;
uint8_t *p_to_configZone = NULL;
uint8_t *p_to_configZoneWrite = NULL;



/*TODO
 * 		/* Write Public Key */
//ATCA_ReturnStatus = atcab_write_pubkey(PUBLIC_KEY_SLOT, &PublicKey[0]);
/* Write AES Keys */
//ATCA_ReturnStatus = atcab_write_zone(ATCA_ZONE_DATA, (uint16_t)AES_KEY1_SLOT, 0, 0, &AES_Key1[0], 32);
//ATCA_ReturnStatus = atcab_write_zone(ATCA_ZONE_DATA, (uint16_t)AES_KEY2_SLOT, 0, 0, &AES_Key2[0], 32);
///* Write Secrets */
//ATCA_ReturnStatus = atcab_write_zone(ATCA_ZONE_DATA, (uint16_t)SECRETS_SLOT, 0, 0, &Secret[0], 32);
///* Write ENC Secrets */
//atcab_write_enc(key_id, block, data, enc_key, enc_key_id)
/* Write Private Key */ /*
//ATCA_ReturnStatus = atcab_priv_write(PRIVATE_KEY_SLOT, &PrivateKey[0], 0, NULL);
 */


void atecc608_configure_config_zone(atecc608_config_t *configZoneWrite)
{
	/*Write must be performed after the byte #16*/
	configZoneWrite->I2C_Address = 0xC0;
	configZoneWrite->Reserved1 = 0;
	configZoneWrite->CountMatch = 0;
	configZoneWrite->ChipMode = 0;


	/* PublicKey Configuration */
	configZoneWrite->SlotConfig[10] =  0x0F8Fu;
	configZoneWrite->KeyConfig[10] = 0x0030u;
	/*AES Key1 Configuration */
	configZoneWrite->SlotConfig[9] =  0x0F8Fu;
	configZoneWrite->KeyConfig[9] = 0x001Au;
	/*AES Key2 Configuration#undef SERIALNUMBER_SIZE
	*/
	configZoneWrite->SlotConfig[11] =  0x0F8Fu;
	configZoneWrite->KeyConfig[11] = 0x001Au;
	/* Secret Configuration */
	configZoneWrite->SlotConfig[13] = 0x0F8Fu;
	configZoneWrite->KeyConfig[13] = 0x001Cu;
	/* ENC Secret Configuration */
	configZoneWrite->SlotConfig[14] = 0x89C9u;
	configZoneWrite->KeyConfig[14] = 0x001Cu;

	/* Additional config Values */
	configZoneWrite->UseLock = 0x00;
	configZoneWrite->VolatileKeyPermission = 0x00;
	configZoneWrite->SecureBoot = 0x00;
	configZoneWrite->KdflvLoc = 0x00;
	configZoneWrite->KdflvStr = 0x00;
	configZoneWrite->LockValue = 0x00;
	configZoneWrite->LockConfig = 0x00;



/*	0x83, 0x20, //  Slot Config Slot 1
	        0x85, 0x20, //  Slot Config Slot 2 		15-12 1 0 X X Never Writes are never permitted on this slot using the Write command.
													Slots set to never can still be used as key storage
	        0x8F, 0x20, //  Slot Config Slot 3
	        0xC4, 0x8F, //  Slot Config Slot 4
	        0x8F, 0x8F, //  Slot Config Slot 5
	        0x8F, 0x8F, //  Slot Config Slot 6
	        0x9F, 0x8F, //  Slot Config Slot 7
	        0x0F, 0x0F, //  Slot Config Slot 8
	        0x8F, 0x0F, //  Slot Config Slot 9
	        0x8F, 0x0F, //  Slot Config Slot 10
	        0x8F, 0x0F, //  Slot Config Slot 11
	        0x8F, 0x0F, //  Slot Config Slot 12
	        0x8F, 0x0F, //  Slot Config Slot 13
	        0x00, 0x00, //  Slot Config Slot 14
	        0x00, 0x00, //  Slot Config Slot 15
	        0xAF, 0x8F, //  Slot Config Slot 16
*/
}
/** \brief Lock the CONFIG_ZONE or the DATA_ZONE
 *  \param[in] cfg  Logical interface configuration. Some predefined
 *                  configurations can be found in atca_cfgs.h
 *  \param[in] zone LOCK_ZONE_DATA or LOCK_ZONE_CONFIG
 *  \return ATCA_SUCCESS on success, otherwise an error code.
 */

ATCA_STATUS lock_zone(ATCAIfaceCfg *cfg, uint8_t zone)
{
    if (zone != (uint8_t)LOCK_ZONE_CONFIG && zone != (uint8_t)LOCK_ZONE_DATA)
        return ATCA_BAD_PARAM;

    ATCA_STATUS status = atcab_init(cfg);

    if (status == ATCA_SUCCESS)
    {
        if (zone == (uint8_t)LOCK_ZONE_DATA)
        {
            return atcab_lock_data_zone();
        }
        else if (zone == (uint8_t)LOCK_ZONE_CONFIG)
        {
            return atcab_lock_config_zone();
        }
        else
        {
            return ATCA_BAD_PARAM;
        }
    }
    return ATCA_BAD_PARAM;
}

/** \brief Check if a the DATA_ZONE or CONFIG_ZONE is locked
 *  \param[in] cfg  Logical interface configuration. Some predefined
 *                  configurations can be found in atca_cfgs.h
 *  \param[in] zone LOCK_ZONE_DATA or LOCK_ZONE_CONFIG
 *  \return ATCA_SUCCESS on success, otherwise an error code.
 */
ATCA_STATUS check_lock_zone(ATCAIfaceCfg *cfg, uint8_t zone)
{
    ATCA_STATUS status;
    bool lock = false;

    if (zone != (uint8_t)LOCK_ZONE_CONFIG && zone != (uint8_t)LOCK_ZONE_DATA)
        return ATCA_BAD_PARAM;

    status = atcab_init(cfg);
    if (status == ATCA_SUCCESS)
    {
        if (ATCA_SUCCESS != (status = atcab_is_locked(zone, &lock)))
        {
            return ATCA_FUNC_FAIL;
        }
        if (!lock)
        {
            return ATCA_NOT_LOCKED;
        }
        return ATCA_SUCCESS;
    }
    return ATCA_BAD_PARAM;
}
/** \brief Write a new configuration to the chip.
 *  \param[in] cfg  Logical interface configuration. Some predefined
 *                  configurations can be found in atca_cfgs.h
 *  \param[in] config Array uint8_t of configuration (length 112)
 *  \param[in] len Size of the configuration array
 *  \return ATCA_SUCCESS on success, otherwise an error code.
 */
ATCA_STATUS write_configuration(ATCAIfaceCfg *cfg, uint8_t *config, size_t len)
{
    if (len != 112)
        return ATCA_BAD_PARAM;

    ATCA_STATUS status;

    status = atcab_init(cfg);
    if (status == ATCA_SUCCESS)
    {

        // Write the configuration Array to the chip
        // Padding of 16 byte (16 first bytes cannot be writed)
        status = atcab_write_bytes_zone(ATCA_ZONE_CONFIG, 0, 16, (uint8_t *)config, len);
        return status;
    }
    return status;
}

/** \brief Write AES key in a given slot.
 *  \param[in] cfg      Logical interface configuration. Some predefined
 *                      configurations can be found in atca_cfgs.h
 *  \param[in] key      key slot number
 *  \param[in] datakey  key array uint8_t
 *  \param[in] len      Size of the key array
 *  \return ATCA_SUCCESS on success, otherwise an error code.
 */
ATCA_STATUS write_key_slot(ATCAIfaceCfg *cfg, uint8_t key, uint8_t *datakey, size_t len)
{
    if (key < 1 && key > 16)
        return ATCA_BAD_PARAM;

    if (len != 32)
        return ATCA_BAD_PARAM;

    ATCA_STATUS status = atcab_init(cfg);

    if (status == ATCA_SUCCESS)
    {
        status = atcab_write_zone(ATCA_ZONE_DATA, (uint16_t)key, 0, 0, datakey, 32);
        if (status != ATCA_SUCCESS)
            return status;
    }
    return status;
}
/** \brief Encrypt data using AES CBC algorithme<br> *  \param[in] cfg          Logical interface configuration. Some predefined
 *                          configurations can be found in atca_cfgs.h
 *  \param[in] data         Words to encypt (must be divided by 16, max length 240)
 *  \param[in] len          length of Words to encypt (must be divided by 16, max length 240)
 *  \param[out] iv          Initial Vector used in the AES CBC (return the vector in this var)
 *  \param[out] ciphertext  return here the Cypher text
 *  \param[in] key          Slot number of the key
 *  \return ATCA_SUCCESS on success, otherwise an error code.
 */
ATCA_STATUS aes_cbc_encrypt(ATCAIfaceCfg *cfg, uint8_t *data, int len, uint8_t *iv, uint8_t *ciphertext, uint8_t key)
{

    atca_aes_cbc_ctx_t ctx;

    if (len > LIMIT_DATA_SIZE_CBC && len % 16 != 0)
    {
     //   Serial.print(F("ERROR : ATCA_BAD_PARAM"));
        return ATCA_BAD_PARAM;
    }
    uint8_t tmp_iv[IV_LENGTH_CBC];
    uint8_t tmp_data[len];

    ATCA_STATUS status = atcab_init(cfg);
    if (status == ATCA_SUCCESS)
    {
        status = atcab_aes_cbc_init(&ctx, key, 0, tmp_iv);

        if (status != ATCA_SUCCESS)
        {
     //       Serial.print(F("ERROR Encrypt : atcab_aes_cbc_init, Code Error 0x"));
       //     Serial.println(status, HEX);
            return;
        }
        memcpy(iv, tmp_iv, IV_LENGTH_CBC);
        memcpy(tmp_data, data, len);

        int max = len / 16;

        for (int j = 0; j < max; j++)
        {
            status = atcab_aes_cbc_encrypt_block(&ctx, &tmp_data[j * 16], &ciphertext[j * 16]);
        }
        if (status != ATCA_SUCCESS)
        {
       //     Serial.print(F("ERROR Encrypt : atcab_aes_cbc_encrypt_block, Code Error 0x"));
        //    Serial.println(status, HEX);
        }
        return status;
    }
    return status;
}

/** \brief Decrypt data using AES CBC algorithme<br> *  \param[in] cfg          Logical interface configuration. Some predefined
 *                          configurations can be found in atca_cfgs.h
 *  \param[in] ciphertext   Words to decypt (must be divided by 16, max length 240)
 *  \param[in] len          length of Words to decypt (must be divided by 16, max length 240)
 *  \param[in] iv           Initial Vector to use in the AES CBC
 *  \param[out] plaintext   return here the decrypted text
 *  \param[in] key          Slot number of the key
 *  \return ATCA_SUCCESS on success, otherwise an error code.
 */
ATCA_STATUS aes_cbc_decrypt(ATCAIfaceCfg *cfg, uint8_t *ciphertext, int len, uint8_t *iv, uint8_t *plaintext, uint8_t key)
{

    atca_aes_cbc_ctx_t ctx;

    if (len > LIMIT_DATA_SIZE_CBC || len % 16 != 0)
    {
      //  Serial.print(F("ERROR Decrypt : ATCA_BAD_PARAM"));
        return ATCA_BAD_PARAM;
    }

    ATCA_STATUS status = atcab_init(cfg);
    if (status == ATCA_SUCCESS)
    {
        status = atcab_aes_cbc_init(&ctx, key, 0, iv);

        if (status != ATCA_SUCCESS)
        {
      //      Serial.print(F("ERROR Decrypt: atcab_aes_cbc_init, Code Error 0x"));
       //     Serial.println(status, HEX);
            return ;
        }
        int max = len / 16;

        for (int j = 0; j < max; j++)
        {
            status = atcab_aes_cbc_decrypt_block(&ctx, &ciphertext[j * 16], &plaintext[j * 16]);
        }
        if (status != ATCA_SUCCESS)
        {
          //  Serial.print(F("ERROR Decrypt : atcab_aes_cbc_encrypt_block, Code Error 0x"));
       //     Serial.println(status, HEX);
        }
        return status;
    }
    return status;
}

uint32_t BE_to_SE_convert(uint32_t value)
{
uint32_t R_ChecksumValue = 0;
R_ChecksumValue = ((value >> 24) & 0xff) | // move byte 3 to byte 0
                  ((value << 8) & 0xff0000) | // move byte 1 to byte 2
                  ((value >> 8) & 0xff00) | // move byte 2 to byte 1
                  ((value << 24)& 0xff000000); // byte 0 to byte 3
return R_ChecksumValue;
}

ATCA_STATUS Crypto_u_Init(void)
{
	return atcab_init(&cfg_ateccx08a_i2c_default);
};

ATCA_STATUS Crypto_u_ReadSerialNumber(uint8_t* out_SerialNUmber)
{
#define SERIALNUMBER_SIZE 9U
	ATCA_STATUS ReturnStatus;
	uint8_t SerialNumberBuffer[SERIALNUMBER_SIZE] = {0};

	/* Reading serial number */
	ReturnStatus = atcab_read_serial_number(&SerialNumberBuffer[0]);

	if(ReturnStatus = ATCA_SUCCESS)
	{
		for(int i = 0; i < sizeof(SerialNumberBuffer);i++)
		{
			*(out_SerialNUmber + i) = SerialNumberBuffer[i];
		}
	}
	else
	{
		memset(out_SerialNUmber, INVALID_VALUE, SERIALNUMBER_SIZE);
	}

	return ReturnStatus;
#undef SERIALNUMBER_SIZE
}

ATCA_STATUS Crypto_u_ReadConfigZone(uint8_t* out_ConfigZoneBUffer)
{
#define CONFIG_ZONE_SIZE 127U
	ATCA_STATUS ReturnStatus;
	uint8_t configZoneBuffer[CONFIG_ZONE_SIZE] = {0};

	ReturnStatus = atcab_read_config_zone(&configZoneBuffer[0]);

	if(ReturnStatus == ATCA_SUCCESS)
	{
		for(int i = 0; i < CONFIG_ZONE_SIZE; i++)
		{
			*(out_ConfigZoneBUffer + i) = configZoneBuffer[i];
		}

	}
	else
	{
		memset(out_ConfigZoneBUffer, INVALID_VALUE, CONFIG_ZONE_SIZE);
	}


#undef CONFIG_ZONE_SIZE}

ATCA_STATUS Crypto_u_RandomNumberGenerate(uint8_t* out_RandomNUmberBuffer)
{
#define RANDOM_NUMBER_SIZE 32U
	ATCA_STATUS ReturnStatus;
	uint8_t RandomNumberBuffer[RANDOM_NUMBER_SIZE] = {0};

	ReturnStatus = atcab_random(&RandomNumberBuffer[0]);
	if(ReturnStatus == ATCA_SUCCESS)
	{
		for(int i = 0; i < RANDOM_NUMBER_SIZE; i++)
		{
			*(out_RandomNUmberBuffer + i) = RandomNumberBuffer[i];
		}

	}
	else
	{
		memset(out_RandomNUmberBuffer, INVALID_VALUE, RANDOM_NUMBER_SIZE);
	}


#undef RANDOM_NUMBER_SIZE
}


/* Write Configuration */
ATCA_STATUS Crypto_u_WriteConfigurationZone(uint8_t* in_ConfigurationZone)
{
	 return  atcab_write_config_zone(in_ConfigurationZone);
}
/* Lock Configuration */
ATCA_STATUS Crypto_u_LockConfigurationZone(void)
{
	return  atcab_lock_config_zone();
}
/* Write Configuration Data */
ATCA_STATUS Crypto_u_WriteDataZone( uint16_t in_slot, size_t in_offset_bytes, const uint8_t* in_data, size_t in_length)
{

	 return atcab_write_bytes_zone(ATCA_ZONE_DATA, in_slot, in_offset_bytes, in_data, in_length);
}

/* Lock Data */
ATCA_STATUS Crypto_u_LockDataZone(void)
{
	return atcab_lock_data_zone();
}
/* Calculate CMAC */
ATCA_STATUS Crypto_u_CalculateCMAC(uint16_t in_key_id, uint8_t in_key_block, uint8_t *in_Data, uint32_t in_DataSize, uint8_t* out_cmac, uint32_t in_cmac_size)
{
    ATCA_STATUS status = ATCA_SUCCESS;
    atca_aes_cmac_ctx_t ctx;



    if (ATCA_SUCCESS != (status = atcab_aes_cmac_init(&ctx, in_key_id, in_key_block)))
    {
        return ATCA_TRACE(status, "atcab_aes_cmac_init - failed");
    }

    if (ATCA_SUCCESS != (status = atcab_aes_cmac_update(&ctx, in_Data, in_DataSize)))
    {
        return ATCA_TRACE(status, "atcab_aes_cmac_update - failed");
    }

    if (ATCA_SUCCESS != (status = atcab_aes_cmac_finish(&ctx, out_cmac, in_cmac_size)))
    {
        return ATCA_TRACE(status, "atcab_aes_cmac_finish - failed");
    }

    return ATCA_SUCCESS;
}

/* Perform Signing operation  */
ATCA_STATUS Crypto_u_Sign(uint16_t in_key_id, const uint8_t* in_msg, uint8_t* out_signature)
{
	return atcab_sign_ext(_gDevice, in_key_id, in_msg, out_signature);
}

/* Perform Verify operation with Stored Public Key  */
ATCA_STATUS Crypto_u_Verify_stored(uint16_t in_key_id, const uint8_t* in_msg, uint8_t* out_signature, bool *out_isVerifiedStatus)
{
	return atcab_verify_stored(in_msg, out_signature, in_key_id, out_isVerifiedStatus);
}
/* Perform Verify operation with Public Key as input Parameter  */
ATCA_STATUS Crypto_u_Verify(const uint8_t* in_message, const uint8_t* out_signature, const uint8_t* in_public_key, bool* out_isVerifiedStatus)
{
	return  atcab_verify_extern(in_message, out_signature, in_public_key, out_isVerifiedStatus);
}

/* Perform Readout of the Secret  */
ATCA_STATUS Crypto_u_ReadSecret(uint8_t* out_Secret)
{
	return atcab_read_bytes_zone(ATCA_ZONE_DATA, SECRETS_SLOT, 0, out_Secret, 32U);
}

/* Perform Readout of the Encrypted Secret  */
ATCA_STATUS Crypto_u_EncryptedReadSecret(uint8_t* out_Secret)
{
	return atcab_read_bytes_zone(ATCA_ZONE_DATA, SECRETS_ENC_SLOT, 0, out_Secret, 32U);
}

ATCA_STATUS Crypto_u_CheckCMAC(uint16_t in_key_id, uint8_t in_key_block, uint8_t *in_Data, uint32_t in_DataSize, uint8_t* out_cmac, uint32_t in_cmac_size)
{
	/* TODO  */
	//return atcab_checkmac(mode, key_id, challenge, response, other_data)
}

/* Perform Calculation of HMAC based on Stored Key  */
ATCA_STATUS Crypto_u_CalculateHMAC(const uint8_t* in_data, size_t in_data_size, uint16_t in_key_slot, uint8_t* out_digest, uint8_t in_target)
{
	return atcab_sha_hmac(in_data, in_data_size, in_key_slot, out_digest, in_target);

}






