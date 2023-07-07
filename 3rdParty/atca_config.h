/*
 * atca_config.h
 *
 *  Created on: May 4, 2023
 *      Author: mmitr
 */

#ifndef ATCA_CONFIG_H_
#define ATCA_CONFIG_H_

#define ATCA_HAL_I2C
#define ATCA_ATECC608_SUPPORT
#define ATCA_HAL_LEGACY_API
#define ATCA_NO_HEAP
#define ATCA_USE_ATCAB_FUNCTIONS
#define ATCA_PRINTF
#define ATCA_USE_CONSTANT_HOST_NONCE


#ifndef ATCA_POST_DELAY_MSEC
#define ATCA_POST_DELAY_MSEC 25
#endif

#define atca_delay_ms   hal_delay_ms
#define atca_delay_us   hal_delay_us


//atecc608_config_t atecc608_cfg;




#endif /* ATCA_CONFIG_H_ */
