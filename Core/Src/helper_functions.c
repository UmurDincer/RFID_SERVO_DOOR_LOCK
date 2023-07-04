/*
 * helper_functions.c
 *
 *  Created on: Jul 3, 2023
 *      Author: LENOVO
 */

#include "helper_functions.h"

extern TIM_HandleTypeDef htim3;
extern PN532 pn532;
extern int debounce;
uint8_t valid_uid[] = {0x93, 0x10, 0xEB, 0x6};

int RFID_Init(PN532* pn532, uint8_t* version)
{
	PN532_I2C_Init(pn532);
	PN532_GetFirmwareVersion(pn532, version);
	if (PN532_GetFirmwareVersion(pn532, version) != PN532_STATUS_OK) {
		return -1;
	}

	PN532_SamConfiguration(pn532);

	return 1;
}


bool read_uid(uint8_t* response)
{
	uint8_t len;
	int i;
	int counter = 0;
	len = PN532_ReadPassiveTarget(&pn532, response, PN532_MIFARE_ISO14443A, 1000);

	if(len > 0){
		for(i = 0; i < 4; i++)
			if(valid_uid[i] == response[i])
			{
				counter++;
			}
	}

	if(counter == 4) return true;
	else return false;
}


uint8_t validation_check(uint8_t *interrupt_checker, uint8_t* response)
{
	uint8_t card_query;
	uint8_t status = 0x00;

	static long int read_counter = 0;
	if(*interrupt_checker == READER_INTERRUPTED)
	{
		card_query = read_uid(response);
		read_counter++;

		if(card_query == true && read_counter % 2 == 0)
		{
			//LOCKED
			status = LOCKED;
			htim3.Instance->CCR1 = 75; // angle/position of the servo
			memset(response, 0, 10 * sizeof(uint8_t));

		}
		else if(card_query == true && read_counter % 2 == 1)
		{
			//UNLOCKED
			status = UNLOCKED;
			htim3.Instance->CCR1 = 25; // angle/position of the servo
			memset(response, 0, 10 * sizeof(uint8_t));
		}
		else if(card_query == false)
		{
			status = REJECTED;
		}
		*interrupt_checker = READER_INTERRUPT_CLEARED;
	}

	HAL_Delay(500);
	debounce = 0;

	return status;

}
