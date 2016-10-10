/*
 * Hcm_StateMachine.c
 *
 *  Created on: Jul 22, 2013
 *      Author: juliend
 */

/* Host Communication State Machine */


#include "GeHcm.h"
#include <string.h>
//#include "Uart.h"
//#include "Utils.h"


GeHcm_CommTransferFlag_type GeHcm_CommTransferFlag;

GeHcm_CommRxStatesMachineStatesType GeHcm_CommRxStatesMachineStates;
GeHcm_CommTxStatesMachineStatesType GeHcm_CommTxStatesMachineStates;

static uint8_t* GeHcm_packetPtr;
static uint16_t GeHcm_ByteCounter;


void GeHcm_StateMachineInit()
{
	//Rx mode is the default mode
	GeHcm_CommTransferFlag = GeHcm_RxTranferEnable;
	//Hcm_StatesMachineStates = Hcm_StateRxSync;
	GeHcm_CommRxStatesMachineStates = GeHcm_CommRxState_Init;
	GeHcm_CommTxStatesMachineStates = GeHcm_CommTxState_Init;
	/* Init Byte counter */
	GeHcm_ByteCounter = 0;

	/* Disable the TX Empty Int */
	UART_BWR_C2_TCIE(GeHcm_UartBaseAdr, 0U);
	/* Init Software Timer */
	//Hcm_TimoutTimerId = SoftTmr_AllocateTimer();

}

//void Hcm_TimerTimeOutCallBack(tmrTimerID_t timerId)
//{
//	(void)timerId;
//	//LED = !LED;                            // change state of LED
//	/* Reset State machine */
//	Hcm_CommRxStatesMachineStates = Hcm_CommRxState_Preamble;
//
//}

void GeHcm_CommRxStateMachine(void)
{
	uint8_t RxData;

	switch (GeHcm_CommRxStatesMachineStates)
	{

		case GeHcm_CommRxState_Init:
			//Rx mode is the default mode
			GeHcm_CommTransferFlag = GeHcm_RxTranferEnable;
			/* Disable the TX Empty Int */
			UART_BWR_C2_TCIE(GeHcm_UartBaseAdr, 0U);
			/* Enable the Receive Int */
			UART_BWR_C2_RIE(GeHcm_UartBaseAdr, 1U);
			/* Enable Receiver */
			UART_HAL_EnableReceiver(GeHcm_UartBaseAdr);
			/* Clear the data */
			memset(&GeHcm_RxFrame, 0, sizeof(GeHcm_RxFrame));
			/* Init Pointer to the Packet Structure */
			GeHcm_packetPtr = (uint8_t*)(&GeHcm_RxFrame.Sync);
			/* Init Byte counter */
			GeHcm_ByteCounter = 0;
			/* Go to the next state directly */
			GeHcm_CommRxStatesMachineStates = GeHcm_CommRxState_Preamble;
		//break;
		/* Send Start of Packet Preamble */
		case GeHcm_CommRxState_Preamble:
			/* Read Rx Data */
			UART_HAL_Getchar(GeHcm_UartBaseAdr, &RxData);
			/* Is this the Sync byte? */
			if( RxData == GEHCM_FRAME_PREAMBLE)
			{
				/* Save the Data */
				*(GeHcm_packetPtr) = RxData;
				/* Point to the Next Byte */
				GeHcm_packetPtr++;
				GeHcm_ByteCounter++;
				/* Next State */
				GeHcm_CommRxStatesMachineStates = GeHcm_CommRxState_DestAdr;
			}
		break;

		case GeHcm_CommRxState_DestAdr:
			/* Read Rx Data */
			UART_HAL_Getchar(GeHcm_UartBaseAdr, &RxData);
			/* Is this the Sync byte? */
			if( RxData == GEHCM_FRAME_HOST_ADRESS)
			{
				/* Save the Data */
				*(GeHcm_packetPtr) = RxData;
				/* Point to the Next Byte */
				GeHcm_packetPtr++;
				GeHcm_ByteCounter++;

				/* Disable the Receive Int */
				UART_BWR_C2_RIE(GeHcm_UartBaseAdr, 0U);
				/* Disable Receiver */
				UART_HAL_DisableReceiver(GeHcm_UartBaseAdr);
				/* Disable the TX Empty Int */
				UART_BWR_C2_TCIE(GeHcm_UartBaseAdr, 0U);
				/* Send Byte - Putchar also clear the TX Int */
				UART_HAL_Putchar(GeHcm_UartBaseAdr, GEHCM_FRAME_ACK);
				/* Enable the TX Empty Int */
				UART_BWR_C2_TCIE(GeHcm_UartBaseAdr, 1U);

				/* Next State */
				GeHcm_CommRxStatesMachineStates = GeHcm_CommRxState_TxAck;
			}
			else /* Wrong Address */
			{
				/* Next State */
				GeHcm_CommRxStatesMachineStates = GeHcm_CommRxState_Init;
			}
		break;

		case GeHcm_CommRxState_TxAck:
			/* Disable the TX Empty Int */
			UART_BWR_C2_TCIE(GeHcm_UartBaseAdr, 0U);
			/* Enable the Receive Int */
			UART_BWR_C2_RIE(GeHcm_UartBaseAdr, 1U);
			/* Enable Receiver */
			UART_HAL_EnableReceiver(GeHcm_UartBaseAdr);

			/* Next State */
			GeHcm_CommRxStatesMachineStates = GeHcm_CommRxState_RxByte;
		break;

		case GeHcm_CommRxState_RxByte:
			/* Read Rx Data */
			UART_HAL_Getchar(GeHcm_UartBaseAdr, &RxData);

			/* Save the Data */
			*(GeHcm_packetPtr) = RxData;
			/* Point to the Next Byte */
			GeHcm_packetPtr++;
			GeHcm_ByteCounter++;

			/* Disable the Receive Int */
			UART_BWR_C2_RIE(GeHcm_UartBaseAdr, 0U);
			/* Disable Receiver */
			UART_HAL_DisableReceiver(GeHcm_UartBaseAdr);
			/* Disable the TX Empty Int */
			UART_BWR_C2_TCIE(GeHcm_UartBaseAdr, 0U);
			/* Send Byte - Putchar also clear the TX Int */
			UART_HAL_Putchar(GeHcm_UartBaseAdr, GEHCM_FRAME_ACK);
			/* Enable the TX Empty Int */
			UART_BWR_C2_TCIE(GeHcm_UartBaseAdr, 1U);

			if( RxData == GEHCM_FRAME_END)
			{
				/* Next State */
				GeHcm_CommRxStatesMachineStates = GeHcm_CommRxState_Done;
			}
			else
			{
				/* Next State */
				GeHcm_CommRxStatesMachineStates = GeHcm_CommRxState_TxAck;
			}
		break;

		case GeHcm_CommRxState_Done:
			/* Disable the TX Empty Int */
			UART_BWR_C2_TCIE(GeHcm_UartBaseAdr, 0U);
			/* Send Byte - Putchar also clear the TX Int */
			UART_HAL_Putchar(GeHcm_UartBaseAdr, GEHCM_FRAME_ACK);
			/* Read Rx Data */
			UART_HAL_Getchar(GeHcm_UartBaseAdr, &RxData);
			/* Disable the Receive Int */
			UART_BWR_C2_RIE(GeHcm_UartBaseAdr, 0U);
			/* Next State */
			GeHcm_CommRxStatesMachineStates = GeHcm_CommRxState_Init;
			/* Post the task */
			OSA_SemaPost(&GE_UART_State.rxIrqSync);
		break;

		default:
			/* We are done, Reset the state machine */
			GeHcm_CommRxStatesMachineStates = GeHcm_CommRxState_Init;
		break;

	}

}

void GeHcm_CommTxStateMachine(void)
{
	uint8_t Dummy;

	switch (GeHcm_CommTxStatesMachineStates)
	{

		case GeHcm_CommTxState_Init:
			/* Disable the Receive Int */
			UART_BWR_C2_RIE(GeHcm_UartBaseAdr, 0U);
			/* Dummy read - Getchar call takes care of clearing the Int flag */
			UART_HAL_Getchar(GeHcm_UartBaseAdr, &Dummy);
			/* Disable Receiver */
			UART_HAL_DisableReceiver(GeHcm_UartBaseAdr);
			/* Init Pointer to the Packet Structure */
			GeHcm_packetPtr = (uint8_t*)(&GeHcm_TxFrame.Sync);
			/* Init Byte counter */
			GeHcm_ByteCounter = 0;
			/* Go to the next state directly */
			GeHcm_CommTxStatesMachineStates = GeHcm_CommTxState_Preamble;
		/* Send Start of Packet Preamble */
		case GeHcm_CommTxState_Preamble:
			/* Disable the TX Empty Int */
			UART_BWR_C2_TCIE(GeHcm_UartBaseAdr, 0U);
			/* Send Byte - Putchar also clear the TX Int */
			UART_HAL_Putchar(GeHcm_UartBaseAdr, *(GeHcm_packetPtr));
			/* Enable the TX Empty Int */
			UART_BWR_C2_TCIE(GeHcm_UartBaseAdr, 1U);

			/* Point to the Next Byte */
			GeHcm_packetPtr++;
			GeHcm_ByteCounter++;
			/* Next State */
			if( GeHcm_ByteCounter >= GEHCM_PREAMBLE_LENGTH_BYTE)
			{
				GeHcm_CommTxStatesMachineStates = GeHcm_CommTxState_RxAck;
			}

		break;

		case GeHcm_CommTxState_RxAck:
			/* Disable the TX Empty Int */
			UART_BWR_C2_TCIE(GeHcm_UartBaseAdr, 0U);
			/* Dummy read - Getchar call takes care of clearing the Int flag */
			UART_HAL_Getchar(GeHcm_UartBaseAdr, &Dummy);
			/* Enable Receiver */
			UART_HAL_EnableReceiver(GeHcm_UartBaseAdr);
			/* Enable the Receive Int */
			UART_BWR_C2_RIE(GeHcm_UartBaseAdr, 1U);


			/* is this the last byte? */
			if( GeHcm_ByteCounter >= GeHcm_TxFrame.Lenght )
			{
				/* Next State */
				GeHcm_CommTxStatesMachineStates = GeHcm_CommTxState_Done;
			}
			else
			{
				GeHcm_CommTxStatesMachineStates = GeHcm_CommTxState_TxByte;
			}

		break;
		/* Disable Tx Int and Enable Rx Int */
		case GeHcm_CommTxState_TxByte:
			/* Dummy read - Getchar call takes care of clearing the Int flag */
			UART_HAL_Getchar(GeHcm_UartBaseAdr, &Dummy);
			/* Disable Receiver */
			UART_HAL_DisableReceiver(GeHcm_UartBaseAdr);
			/* Disable the Receive Int */
			UART_BWR_C2_RIE(GeHcm_UartBaseAdr, 0U);
			/* Send Byte - The Putchar call takes care of clearing the Int Flag */
			UART_HAL_Putchar(GeHcm_UartBaseAdr, *(GeHcm_packetPtr));
			/* Enable the TX Empty Int */
			UART_BWR_C2_TCIE(GeHcm_UartBaseAdr, 1U);

			/* Next Byte pointer increment */
			GeHcm_packetPtr++;
			GeHcm_ByteCounter++;
			/* Next State */
			GeHcm_CommTxStatesMachineStates = GeHcm_CommTxState_RxAck;

		break;

		case GeHcm_CommTxState_TxLastByte:
			/* Dummy read - Getchar call takes care of clearing the Int flag */
			UART_HAL_Getchar(GeHcm_UartBaseAdr, &Dummy);
			/* Disable the Receive Int */
			UART_BWR_C2_RIE(GeHcm_UartBaseAdr, 0U);
			/* Disable Receiver */
			UART_HAL_DisableReceiver(GeHcm_UartBaseAdr);
			/* Send Ack Byte */
			UART_HAL_Putchar(GeHcm_UartBaseAdr, GEHCM_FRAME_ACK);
			/* Enable the TX Empty Int */
			UART_BWR_C2_TCIE(GeHcm_UartBaseAdr, 1U);

			/* Next State */
			GeHcm_CommTxStatesMachineStates = GeHcm_CommTxState_Done;

		break;

		/* Packet Tx Done  */
		case GeHcm_CommTxState_Done:
			/* Disable the TX Empty Int */
			UART_BWR_C2_TCIE(GeHcm_UartBaseAdr, 0U);
			/* Dummy read - Getchar call takes care of clearing the Int flag */
			UART_HAL_Getchar(GeHcm_UartBaseAdr, &Dummy);
			/* Switch back to Receive mode */
			GeHcm_CommTransferFlag = GeHcm_RxTranferEnable;
			/* Disable the Receive Int */
			//UART_BWR_C2_RIE(GeHcm_UartBaseAdr, 0U);
			/* Enable the Receive Int */
			UART_BWR_C2_RIE(GeHcm_UartBaseAdr, 1U);
			/* Enable Receiver */
			UART_HAL_EnableReceiver(GeHcm_UartBaseAdr);
			/* We are done, Reset the state machine */
			GeHcm_CommTxStatesMachineStates = GeHcm_CommTxState_Init;

		break;

		default:
			/* We are done, Reset the state machine */
			GeHcm_CommTxStatesMachineStates = GeHcm_CommTxState_Init;
			/* Reset counter */
		break;

	}


}

