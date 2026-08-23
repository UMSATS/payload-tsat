/*
 * core.c
 *
 *  Created on: Jan 9, 2024
 *      Author: Logan Furedi
 */

#include <stm32l452xx.h>
#include "core.h"

// Standard Libraries
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <sys/_stdint.h>

// High Level Drivers
#include <heaters.h>
#include <leds.h>
#include <photocells.h>
#include <power.h>
#include <thermistors.h>
#include <well_id.h>

// Hardware Peripheral Drivers
#include <MAX6822_driver.h>
#include <tca9539.h>
#include <tmp235.h>

#include "tuk/tuk.h"

typedef enum {
	IDLE = 0,
	ACTIVE
} State;

static State s_state = IDLE;
static uint8_t s_temp_sequence = 0;
static uint8_t s_light_sequence = 0;

static void on_message_received(CANMessage msg, NodeID sender, bool is_ack);
static void on_error_occured(CANWrapper_ErrorInfo error);
static void report_well_temp_data(WellID well_id);
static void report_well_light_data(WellID well_id);
//static void process_errors(ErrorBuffer *p_error_buffer);
static void print_well_info();

#define PRINT_SUBJECT "Core"

void Core_Init()
{
	bool success;

	success = TCA9539_Init();
	if (!success)
	{
		PRINT_ERROR("failed to initialise IO Expander driver.");
		//PUT_ERROR(ERR_PLD_TCA9539_INIT);
	}

/*
	if (ErrorBuffer_Has_Error(&s_error_buffer)) // TODO: replace with error code
	{
		process_errors(&s_error_buffer);
	}
*/
}

// callback for timers.
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
//	//ErrorBuffer error_buffer;
//	//ErrorTracker_Push_Buffer(&error_buffer);
//
//	if (htim == &htim2)
//	{
//		// FIXME: these repeated calls can result in an error buffer overflow if they produce errors.
//		for (int i = WELL_0; i <= WELL_15; i++)
//		{
//			report_well_temp_data(i);
//		}
//		for (int i = WELL_0; i <= WELL_15; i++)
//		{
//			report_well_light_data(i);
//		}
//	}
//
//	//process_errors(&error_buffer);
//
//	//ErrorTracker_Pop_Buffer();
}



// function to get temperature data, package it and send it through CAN
static void report_well_temp_data(WellID well_id)
{
//	uint16_t temp;
//	bool success = Thermistors_Get_Temp(well_id, &temp);
//
//	if (!success)
//	{
//		PRINT_ERROR("failed to report temperature of well %d: could not get temperature.", well_id);
//		// TODO: Add PUT_ERROR?
//		return;
//	}
//
//	CANMessage msg;
//	msg.cmd = CMD_CDH_PROCESS_TELEMETRY_REPORT;
//	uint8_t tel_key = CREATE_TELEMETRY_KEY(TEL_WELL_TEMP, well_id);
//	SET_ARG(msg, 0, uint8_t, tel_key);
//	SET_ARG(msg, 1, uint8_t, s_temp_sequence);
//	SET_ARG(msg, 2, uint8_t, 0); // packet #
//	SET_ARG(msg, 3, uint16_t, temp);
//
//	s_temp_sequence++;
//
//	CANWrapper_Transmit(NODE_CDH, &msg);
}

// function to get light level data, package it and send it through CAN
static void report_well_light_data(WellID well_id)
{
//	uint16_t light;
//	bool success = Photocells_Get_Light_Level(well_id, &light);
//
//	if (!success)
//	{
//		PRINT_ERROR("failed to report temperature of well %d: could not get temperature.", well_id);
//		// TODO: Add PUT_ERROR?
//		return;
//	}
//
//	CANMessage msg;
//	msg.cmd = CMD_CDH_PROCESS_TELEMETRY_REPORT;
//	uint8_t tel_key = CREATE_TELEMETRY_KEY(TEL_WELL_LUMINOSITY, well_id);
//	SET_ARG(msg, 0, uint8_t, tel_key);
//	SET_ARG(msg, 1, uint8_t, s_light_sequence);
//	SET_ARG(msg, 2, uint8_t, 0); // packet #
//	SET_ARG(msg, 3, uint16_t, light);
//
//	s_light_sequence++;
//
//	CANWrapper_Transmit(NODE_CDH, &msg);
}

/*
static void process_errors(ErrorBuffer *p_error_buffer)
{
	if (ErrorBuffer_Has_Error(p_error_buffer))
	{
		CANMessage error_report;
		error_report.cmd = CMD_CDH_PROCESS_RUNTIME_ERROR;
		//SET_ARG(error_report, 0, *p_error_buffer); // TODO

		CANWrapper_Transmit(NODE_CDH, &error_report);

		ErrorBuffer_Clear(p_error_buffer);
	}
}
*/

static void print_well_info()
{
	uint16_t therm_data[16];
	uint16_t light_data[16];

	uint16_t temp;

	for (int i = 0; i < 16; i++)
	{
		Thermistors_Get_Temp(i, &temp);
		therm_data[i] = temp;

		Photocells_Get_Light_Level(i, &temp);
		light_data[i] = temp;
	}

	PRINT_INFO("_________________________");
	PRINT_INFO("| WELL  | TEMPS | LIGHT |");
	PRINT_INFO("|-----------------------|");
	for (int i = WELL_0; i <= WELL_15; i++)
	{
		PRINT_INFO("| %6d| %6d| %6d|", i, therm_data[i], light_data[i]);
	}
	PRINT_INFO("-------------------------");
}
