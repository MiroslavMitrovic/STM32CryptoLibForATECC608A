/*
 * CryptoWrapper.h
 *
 *  Created on: Jul 4, 2023
 *      Author: mmitr
 */

#ifndef INC_CRYPTOWRAPPER_H_
#define INC_CRYPTOWRAPPER_H_

#include "cryptoauthlib.h"
#include "sha256.h"

#define IV_LENGTH_CBC 16U
#define LIMIT_DATA_SIZE_CBC 240U
#define PRIVATE_KEY_SLOT 2U
#define AES_KEY1_SLOT 9U
#define PUBLIC_KEY_SLOT 10U
#define AES_KEY2_SLOT 11U
#define SECRETS_SLOT 13U
#define SECRETS_ENC_SLOT 14U
#define INVALID_VALUE 0xFFU


void atecc608_configure_config_zone(atecc608_config_t *configZone);
uint32_t BE_to_SE_convert(uint32_t value);
ATCA_STATUS lock_zone(ATCAIfaceCfg *cfg, uint8_t zone);
ATCA_STATUS check_lock_zone(ATCAIfaceCfg *cfg, uint8_t zone);
ATCA_STATUS write_configuration(ATCAIfaceCfg *cfg, uint8_t *config, size_t len);
ATCA_STATUS write_key_slot(ATCAIfaceCfg *cfg, uint8_t key, uint8_t *datakey, size_t len);
ATCA_STATUS aes_cbc_encrypt(ATCAIfaceCfg *cfg, uint8_t *data, int len, uint8_t *iv, uint8_t *ciphertext, uint8_t key);
ATCA_STATUS aes_cbc_decrypt(ATCAIfaceCfg *cfg, uint8_t *ciphertext, int len, uint8_t *iv, uint8_t *plaintext, uint8_t key);
ATCA_STATUS Crypto_u_Init(void);
ATCA_STATUS Crypto_u_ReadSerialNumber(uint8_t* out_SerialNUmber);
ATCA_STATUS Crypto_u_ReadConfigZone(uint8_t* out_ConfigZoneBUffer);
ATCA_STATUS Crypto_u_RandomNumberGenerate(uint8_t* out_RandomNUmberBuffer);
ATCA_STATUS Crypto_u_WriteConfigurationZone(uint8_t* in_ConfigurationZone);
ATCA_STATUS Crypto_u_LockConfigurationZone(void);
ATCA_STATUS Crypto_u_LockDataZone(void);
ATCA_STATUS Crypto_u_CalculateCMAC(uint16_t in_key_id, uint8_t in_key_block, uint8_t *in_Data, uint32_t in_DataSize, uint8_t* out_cmac, uint32_t in_cmac_size);
ATCA_STATUS Crypto_u_CheckCMAC(uint16_t in_key_id, uint8_t in_key_block, uint8_t *in_Data, uint32_t in_DataSize, uint8_t* out_cmac, uint32_t in_cmac_size);
ATCA_STATUS Crypto_u_Sign(uint16_t in_key_id, const uint8_t* in_msg, uint8_t* out_signature);
ATCA_STATUS Crypto_u_Verify_stored(uint16_t in_key_id, const uint8_t* in_msg, uint8_t* out_signature, bool *out_isVerifiedStatus);
ATCA_STATUS Crypto_u_Verify(const uint8_t* in_message, const uint8_t* out_signature, const uint8_t* in_public_key, bool* out_isVerifiedStatus);
ATCA_STATUS Crypto_u_ReadSecret(uint8_t* out_Secret);
ATCA_STATUS Crypto_u_EncryptedReadSecret(uint8_t* out_Secret);

#endif /* INC_CRYPTOWRAPPER_H_ */
