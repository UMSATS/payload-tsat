/*
 * command_handling.c
 */

#include "command_handling.h"
#include "tuk/tuk.h"

void On_CAN_Message_Ready(const CAN_HandleTypeDef *hcan, const CANMessage *msg)
{
	switch (msg->cmd)
	{
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

