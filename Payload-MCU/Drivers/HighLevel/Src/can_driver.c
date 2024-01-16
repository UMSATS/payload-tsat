/*
 * FILENAME: can_driver.c
 *
 * DESCRIPTION: Functions for CAN initialization, message reception, and message transmission.
 *              Received messages are read into a Queue, which can be handled by a dedicated task.
 *
 * Link to Documentation: https://drive.google.com/file/d/1HHNWpN6vo-JKY5VvzY14uecxMsGIISU7/view?usp=share_link
 *
 * AUTHORS:
 *  - Graham Driver (graham.driver@umsats.ca)
 *
 * CREATED ON: May 25, 2022
 */

//###############################################################################################
// Include Directives
//###############################################################################################
#include <can_driver.h>
#include <stdio.h>
#include "can_message_queue.h"

extern CANQueue_t can_queue;

//###############################################################################################
//Public Functions
//###############################################################################################
/**
 * @brief Boots the CAN Bus
 *
 * @return HAL_StatusTypeDef
 */
HAL_StatusTypeDef CAN_Init(){
    HAL_StatusTypeDef operation_status;

	CAN_FilterTypeDef sFilterConfig = {
			.FilterFIFOAssignment = CAN_FILTER_FIFO0,
			.FilterMode           = CAN_FILTERMODE_IDMASK,
			.FilterScale          = CAN_FILTERSCALE_32BIT,
			.FilterActivation     = ENABLE,
			.SlaveStartFilterBank = 14,
	};

	operation_status = HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig);
	if (operation_status != HAL_OK) return operation_status;

	operation_status = HAL_CAN_Start(&hcan1); // Turn on the CAN Bus
	if (operation_status != HAL_OK) return operation_status;

	operation_status = HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);
	return operation_status;
}

/**
 * @brief Used to send messages over CAN
 *
 * @param myMessage: The CAN message
 *
 * @return HAL_StatusTypeDef
 */
HAL_StatusTypeDef CAN_Transmit_Message(CANMessage_t myMessage){
	uint32_t txMailbox; // Transmit Mailbox
	CAN_TxHeaderTypeDef txMessage;

	// TX Message Parameters
	uint16_t ID = (myMessage.priority << 4) | (SOURCE_ID << 2) | (myMessage.DestinationID);
	uint8_t message[8] = {myMessage.command, myMessage.data[0], myMessage.data[1], myMessage.data[2], myMessage.data[3], myMessage.data[4], myMessage.data[5],myMessage.data[6]};

	txMessage.StdId = ID;
	txMessage.IDE = CAN_ID_STD;
	txMessage.RTR = CAN_RTR_DATA;
	txMessage.DLC = MAX_CAN_DATA_LENGTH;

	while (HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) == 0){} //wait to send CAN message

	return HAL_CAN_AddTxMessage(&hcan1, &txMessage, message, &txMailbox);
}

/**
 * @brief Interrupt Handler for received CAN messages
 *
 * @return HAL_StatusTypeDef
 */
HAL_StatusTypeDef CAN_Message_Received(){
    HAL_StatusTypeDef operation_status;
	CAN_RxHeaderTypeDef rxMessage; // Received Message Header
	uint8_t rxData[8]; // Received data
	uint8_t receivedDestinationId; // Destination ID of Received Message

	/* Get RX message */
	operation_status = HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &rxMessage, rxData);
	if (operation_status != HAL_OK) goto error;
	receivedDestinationId = RECEIVED_DESTINATION_ID_MASK & rxMessage.StdId;

	if(receivedDestinationId == SOURCE_ID){
	    // *NOTE* Send message to queue per your subsystem here
	    CANMessage_t can_message = {
	        .priority = rxMessage.RTR == CAN_RTR_REMOTE ? 0x7F : rxMessage.ExtId >> 24,
	        .SenderID = (RECEIVED_SENDER_ID_MASK & rxMessage.StdId) >> 2,
	        .DestinationID = receivedDestinationId,
	        .command = rxData[0],
	        .data = {rxData[1], rxData[2], rxData[3], rxData[4], rxData[5], rxData[6], rxData[7]}
	    };
	    CAN_Queue_Enqueue(&can_queue, &can_message);

		// *NOTE* program custom handling per your subsystem here
	}

error:
	return operation_status;
}

/**
 * @brief Send a CAN default ACK message for the given CAN message
 *
 * @param myMessage: The received CAN message to send the ACK for
 *
 * @return HAL_StatusTypeDef
 */
HAL_StatusTypeDef CAN_Send_Default_ACK(CANMessage_t myMessage){
    CANMessage_t ack_message = {
        .priority = myMessage.priority,
        .SenderID = SOURCE_ID,
        .DestinationID = myMessage.SenderID,
        .command = 0x01,
        .data = {myMessage.command, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
    };
    return CAN_Transmit_Message(ack_message);
}

/**
 * @brief Send a CAN default ACK message for the given CAN message, with data
 *
 * @param myMessage: The received CAN message to send the ACK for
 * @param p_data: The 6 bytes of data to send
 *
 * @return HAL_StatusTypeDef
 */
HAL_StatusTypeDef CAN_Send_Default_ACK_With_Data(CANMessage_t myMessage, uint8_t *p_data){
    CANMessage_t ack_message = {
        .priority = myMessage.priority,
        .SenderID = SOURCE_ID,
        .DestinationID = myMessage.SenderID,
        .command = 0x01,
        .data = {myMessage.command, p_data[0], p_data[1], p_data[2], p_data[3], p_data[4], p_data[5]}
    };
    return CAN_Transmit_Message(ack_message);
}
