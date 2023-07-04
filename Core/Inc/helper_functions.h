/*
 * helper_functions.h
 *
 *  Created on: Jul 3, 2023
 *      Author: LENOVO
 */

#ifndef INC_HELPER_FUNCTIONS_H_
#define INC_HELPER_FUNCTIONS_H_

#include <stdbool.h>
#include "pn532.h"
#include "pn532_stm32f4.h"

#define READER_INTERRUPTED 0x1
#define READER_INTERRUPT_CLEARED 0x0

typedef enum lock_status{
	LOCKED = 5,
	UNLOCKED,
	REJECTED
}lock_status_e;

int RFID_Init(PN532* pn532, uint8_t* version);
bool read_uid(uint8_t* response);
uint8_t validation_check(uint8_t* interrupt_checker, uint8_t* response);

#include <stdlib.h>
#include <string.h>
#include "stm32f4xx_hal.h"
#endif /* INC_HELPER_FUNCTIONS_H_ */
