/*
 * Hcm.h
 *
 *  Created on: Jul 22, 2013
 *      Author: juliend
 */
/* Host Communication Module */

#ifndef HCM_H_
#define HCM_H_


#include "GeHcm_StateMachine.h"
#include "fsl_uart_driver.h"
#include "GE_UART.h"

#define GEHCM_UART_INSTANCE				FSL_GE_UART

/* Start of text */
#define GEHCM_FRAME_PREAMBLE			0x02
#define GEHCM_FRAME_ACK					0x06
#define GEHCM_FRAME_NACK				0x15
#define GEHCM_FRAME_END					0x03
#define GEHCM_FRAME_HOST_ADRESS			0x11
#define GEHCM_FRAME_TARGET_ADRESS		0x10

#define GEHCM_MAX_PAYLOAD_LENGTH_SIZE_BYTE			256
#define GEHCM_PREAMBLE_LENGTH_BYTE					2
#define HCM_HEADER_CRC_SIZE_BYTE					1
#define HCM_PACKET_CRC_SIZE_BYTE					2


#define HCM_COMMUNICATION_TASK_EVENT_RX			1
#define HCM_COMMUNICATION_TASK_EVENT_TX			2


#define HCM_PACKET_COMM_HEADER_TIMEOUT			100			/* Timeout in ms */
#define HCM_PACKET_COMM_PAYLOAD_TIMEOUT			150





/* Packet frame format */

typedef struct GeHCM_HostFrameFormatType_tag
{
	uint8_t					Sync;
	uint8_t					DestAdr;
	uint8_t					Lenght;
	uint8_t					Cmd;
	uint8_t					SrcAdr;
	uint8_t					Data[GEHCM_MAX_PAYLOAD_LENGTH_SIZE_BYTE];

} GeHCM_HostFrameFormatType;




void GeHcm_Init(void);
void GeHcm_RxTxCallbackFcn(void);

void GeHcm_GetTemp(void);


extern GeHCM_HostFrameFormatType GeHcm_RxFrame;
extern GeHCM_HostFrameFormatType GeHcm_TxFrame;
//EXTERN tsTaskID_t Hcm_CommunicationTaskId;

extern UART_Type * GeHcm_UartBaseAdr;


#endif /* HCM_H_ */
