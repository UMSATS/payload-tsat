/*
 * flash.c
 *
 *  Created on: Jan 17, 2024
 *      Author: Jascha Petersen
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "main.h"

#define FLASH_MEMORY_START 	0x08000000
#define FLASH_MEMORY_END  	0x08080000

#define FLASH_PAGE_SIZE		0x800
#define FLASH_DWORD_SIZE	0x8

#define FLASH_PREAMBLE		0xDEADBEEF

typedef struct Flash_Page_Data {
	uint32_t preamble;
	uint32_t write_count;
};

void flash_init(size_t size) {
	return;
}

void flash_write(uint8_t page, const void* data, size_t size) {

	// TODO: preconditions (page no, data size must be less than 2kB, etc)

	const uint8_t* byte_data = (const uint8_t*)data;

	flash_erase(page);

	uint32_t current_address = page * FLASH_PAGE_SIZE;

	uint64_t dword_buffer = 0;
	uint8_t byte_index = 0;

	for (size_t i = 0; i < size; i++) {

		dword_buffer |= ((uint32_t)byte_data[i]) << (byte_index * 8);
		byte_index++;

		if (byte_index == FLASH_DWORD_SIZE || i == size - 1) {
			if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, current_address, dword_buffer) != HAL_OK) {
				return -1;
			}

			current_address += FLASH_DWORD_SIZE;
			dword_buffer = 0;
			byte_index = 0;
		}

	}



}

void flash_read(uint8_t page, void* data, size_t size) {

}

void flash_erase(uint8_t page) {

	FLASH_EraseInitTypeDef EraseInitStruct;
	EraseInitStruct.TypeErase = FLASH_TYPEERASE_MASSERASE;
	EraseInitStruct.Banks = FLASH_BANK_1; // only 1 bank on this chip

}
