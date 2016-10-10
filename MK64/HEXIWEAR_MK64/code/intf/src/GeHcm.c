/*
 * Hcm.c
 *
 *  Created on: Jul 22, 2013
 *      Author: juliend
 */

/* Host Communication Module */

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "GeHcm.h"
//#include "Uart.h"

/* Communication Buffer */
GeHCM_HostFrameFormatType		GeHcm_RxFrame;
GeHCM_HostFrameFormatType		GeHcm_TxFrame;

/* Uart base address */
UART_Type * GeHcm_UartBaseAdr;

/* Communication Task ID */
//tsTaskID_t Hcm_CommunicationTaskId;


void GeHcm_Init(void)
{

    GeHcm_UartBaseAdr = g_uartBase[GEHCM_UART_INSTANCE];
	//memset(&Hcm_RxFrame, 0x00, sizeof(HCM_HostFrameFormatType));
	//Hcm_CommunicationTaskId = TS_CreateTask(HcmCommunicationTaskPriority, Hcm_CommunicationTask);
	GeHcm_StateMachineInit();
	//UART_RX_ENABLE;



}

void GeHcm_RxTxCallbackFcn(void)
{

	/* receive packet mode */
	if(GeHcm_CommTransferFlag == GeHcm_RxTranferEnable)
	{

		GeHcm_CommRxStateMachine();

	} /* Transmit packet mode */
	else if(GeHcm_CommTransferFlag == GeHcm_TxTranferEnable)
	{
		GeHcm_CommTxStateMachine();

	}

}

void GeHcm_GetTemp(void)
{
	/* Temp Init of the packet for testing */
	GeHcm_TxFrame.Sync = GEHCM_FRAME_PREAMBLE;
	GeHcm_TxFrame.DestAdr = GEHCM_FRAME_TARGET_ADRESS;
	GeHcm_TxFrame.Lenght = 0x08;
	GeHcm_TxFrame.Cmd = 0x8E; /* Read Temperature Command */
	GeHcm_TxFrame.SrcAdr = GEHCM_FRAME_HOST_ADRESS;
	GeHcm_TxFrame.Data[0] = 0x88; /* 16 bit CRC MSB */
	GeHcm_TxFrame.Data[1] = 0x81; /* 16 bit CRC LSB */
	GeHcm_TxFrame.Data[2] = GEHCM_FRAME_END;

	GeHcm_CommTransferFlag = GeHcm_TxTranferEnable;
	GeHcm_CommTxStateMachine();


}

