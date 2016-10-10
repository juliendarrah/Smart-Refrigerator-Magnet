/*
 * Hcm_StateMachine.h
 *
 *  Created on: Jul 22, 2013
 *      Author: juliend
 */

#ifndef GEHCM_STATEMACHINE_H_
#define GEHCM_STATEMACHINE_H_

//#include "Std_Types.h"
//#include "SoftTmr.h"
#include "GeHcm.h"

/* Rx/Tx Flag */
typedef enum GeHcm_CommTransferFlag_tag
{
	GeHcm_TxTranferEnable,
	GeHcm_RxTranferEnable

} GeHcm_CommTransferFlag_type;


/* Rx State Machine states */
typedef enum GeHcm_CommRxStatesMachineStates_tag
{
	GeHcm_CommRxState_Init,
	GeHcm_CommRxState_Preamble,
	GeHcm_CommRxState_DestAdr,
	GeHcm_CommRxState_TxAck,
	GeHcm_CommRxState_RxByte,
	GeHcm_CommRxState_Done

} GeHcm_CommRxStatesMachineStatesType;


/* Tx State Machine states */
typedef enum GeHcm_CommTxStatesMachineStates_tag
{
	GeHcm_CommTxState_Init,
	GeHcm_CommTxState_Preamble,
	GeHcm_CommTxState_RxAck,
	GeHcm_CommTxState_TxByte,
	GeHcm_CommTxState_TxLastByte,
	GeHcm_CommTxState_Done

} GeHcm_CommTxStatesMachineStatesType;



void GeHcm_StateMachineInit();
void GeHcm_CommRxStateMachine(void);
void GeHcm_CommTxStateMachine(void);

extern GeHcm_CommTransferFlag_type GeHcm_CommTransferFlag;


#endif /* GEHCM_STATEMACHINE_H_ */
