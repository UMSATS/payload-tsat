/*
 * command_handling.c
 */

#include "command_handling.h"

#include <stdint.h>

#include <MAX6822_driver.h>

#include "tuk/tuk.h"

void On_CAN_Message_Ready(const CAN_HandleTypeDef *hcan, const CANMessage *msg)
{
	switch (msg->cmd)
	{
	case CMD_COMM_RESET:
		// trigger a hardware reset.
		MAX6822_Manual_Reset();
		break;
	case CMD_COMM_SET_TELEMETRY_INTERVAL:
		break;
	case CMD_PLD_SET_ACTIVE_ENVS:
		break;
	case CMD_PLD_GET_ACTIVE_ENVS:
		break;
	case CMD_PLD_SET_SETPOINT:
		break;
	case CMD_PLD_GET_SETPOINT:
		break;
	case CMD_PLD_SET_TOLERANCE:
		break;
	case CMD_PLD_GET_TOLERANCE:
		break;
	case CMD_PLD_TEST_LEDS:
		break;
	default:
		break;
	}
}

void On_CAN_Error(const CANWrapper_ErrorInfo *error)
{
	// TODO: Handle CAN errors.
}







// TODO Look through old message received and determine what to keep.Maybe make into RTOS tasks.
//static void on_message_received(CANMessage msg, NodeID sender, bool is_ack)
//{
//	LogBuffer buffer; // stores debug information.
//	DebugLogger_Push_Buffer(&buffer);
//
//	bool success = false;
//
//	switch (msg.cmd)
//	{
//	case CMD_COMM_RESET:
//	{
//		// trigger a hardware reset.
//		MAX6822_Manual_Reset();
//		Core_Halt(); // wait for the reset.
//		break;
//	}
//	/*
//	case CMD_PLD_SET_WELL_LED:
//	{
//		uint8_t well_id = GET_ARG(msg, 0, uint8_t);
//		uint8_t power   = GET_ARG(msg, 1, uint8_t);
//
//		success = LEDs_Set_LED(well_id, power);
//		break;
//	}
//	case CMD_PLD_SET_WELL_HEATER:
//	{
//		uint8_t well_id = GET_ARG(msg, 0, uint8_t);
//		uint8_t power   = GET_ARG(msg, 1, uint8_t);
//
//		success = Heaters_Set_Heater(well_id, power);
//		break;
//	}
//	case CMD_PLD_SET_SETPOINT:
//	{
//		uint8_t well_id = GET_ARG(msg, 0, uint8_t);
//		float temp      = GET_ARG(msg, 1, float);
//
//		// TODO: update the target temperature of one of the wells in the TCS.
//		break;
//	}
//	*/
//	/*
//	case CMD_PLD_GET_WELL_LIGHT:
//	{
//		uint8_t well_id = GET_ARG(msg, 0, uint8_t);
//
//		uint16_t light;
//		success = Photocells_Get_Light_Level(well_id, &light);
//		if (success)
//		{
//			CANMessage response;
//			response.cmd = CMD_CDH_PROCESS_WELL_LIGHT;
//			SET_ARG(response, 0, light);
//			CANWrapper_Transmit(NODE_CDH, &response);
//		}
//		break;
//	}
//	case CMD_PLD_GET_WELL_TEMP:
//	{
//		uint8_t well_id = GET_ARG(msg, 0, uint8_t);
//
//		uint16_t temp;
//		success = Thermistors_Get_Temp(well_id, &temp);
//		if (success)
//		{
//			CANMessage response;
//			response.cmd = CMD_CDH_PROCESS_WELL_TEMP;
//			SET_ARG(response, 0, temp);
//			CANWrapper_Transmit(NODE_CDH, &response);
//		}
//		break;
//	}
//	*/
//	case CMD_COMM_SET_TELEMETRY_INTERVAL:
//	{
//		uint32_t period = GET_ARG(msg, 0, uint32_t);
//
//		// set interrupt timer period.
//		__HAL_TIM_SET_AUTORELOAD(&htim2, period);
//
//		success = true;
//		break;
//	}
//	case CMD_PLD_TEST_LEDS:
//	{
//		// TODO
//		break;
//	}
//	default:
//	{
//		PRINT_ERROR("unknown command: 0x%02X.", msg.cmd);
//		//PUT_ERROR(ERR_UNKNOWN_COMMAND, (uint8_t)msg.cmd);
//		break;
//	}
//	}
//
//	DebugLogger_Pop_Buffer();
//
//	/*
//	if (ErrorBuffer_Has_Error(&cmd_error_buffer))
//	{
//		success = false; // FIXME: either do something with this variable or remove it.
//		process_errors(&cmd_error_buffer);
//	}
//	*/
//}
