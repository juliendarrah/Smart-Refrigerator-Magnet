/**
 * \file gui_weatherStation_driver.c
 * \version 1.00
 * \brief this file contains weather station app functionality
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * Neither the name of NXP, nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * visit: http://www.mikroe.com and http://www.nxp.com
 *
 * get support at: http://www.mikroe.com/forum and https://community.nxp.com
 *
 * Project HEXIWEAR, 2015
 */

#include "gui_sensorTag.h"
#include "fridge.h"
#include "fridge_private.h"

#include "HEXIWEAR_driver.h"

#include "fsl_os_abstraction.h"

#include "GE_UART.h"
#include "GeHcm.h"

/** private variables */

static task_handler_t
    fridge_taskHandler;

//static packet_pushTarget_t
//    gui_weatherStation_currentApp;

/** private declarations */

static void fridge_AppTask();

/** public API */

/**
 * initialize the weather station task
 * @param param optional parameter
 */
void fridge_Init( void* param )
{
    GuiDriver_LabelCreate( &fridge_labelTempFF );
    GuiDriver_LabelCreate( &fridge_labelTempFZ );

    GeHcm_GetTemp();
}

/**
 * create tasks for the weather station app
 * @param param optional parameter
 */
void fridge_CreateTasks( void* param )
{
    osa_status_t
        taskStatus = OSA_TaskCreate(
                                        fridge_AppTask,
                                        (uint8_t*) "fridge app",
                                        GUI_FRIDGE_STATION_STACK_SIZE,
                                        NULL,
                                        GUI_FRIDGE_STATION_PRIO,
                                        (task_param_t)0,
                                        false,
                                        &fridge_taskHandler
                                    );

    if ( kStatus_OSA_Success != taskStatus )
    {
        catch( CATCH_INIT );
    }
}

/**
 * destroy tasks in the weather station app
 * @param param optional parameter
 */
void fridge_DestroyTasks( void* param )
{
    OSA_TaskDestroy( fridge_taskHandler );

    OLED_DestroyDynamicArea();

    HEXIWEAR_SetBackupList( NULL );
    HEXIWEAR_SetTargetApp ( PACKET_PUSH_NONE );
}

/** private API */

/**
 * GUI weather station task
 */
static void fridge_AppTask()
{
	int16_t		tmpData;
	int16_t		mod;
    while (1)
    {
    	osa_status_t
    		status = OSA_SemaWait( &GE_UART_State.rxIrqSync, OSA_WAIT_FOREVER );

    	if ( kStatus_OSA_Success == status )
    	{

    		/* Fresh Food temperature */
    		tmpData = (int16_t)(GeHcm_RxFrame.Data[1]); /*LSB*/
    		tmpData = (int16_t)(GeHcm_RxFrame.Data[0]<<8)+tmpData; /*MSB*/

    		/* Handle negative values */
    		mod = tmpData % 100;
    		if( mod < 0 )
    		{
    			mod = mod + 100;
    		}

    		/* Update the Temperature reading */
    		snprintf( (char*)fridge_labelTempFF.caption, 10, "%2i.%02i F", tmpData / 100, mod );
            GuiDriver_LabelDraw(&fridge_labelTempFF);

    		/* Freezer temperature */
    		tmpData = (int16_t)(GeHcm_RxFrame.Data[3]); /*LSB*/
    		tmpData = (int16_t)(GeHcm_RxFrame.Data[2]<<8)+tmpData; /*MSB*/

    		/* Handle negative values */
    		mod = tmpData % 100;
    		if( mod < 0 )
    		{
    			mod = mod + 100;
    		}

    		/* Update the Temperature reading */
    		snprintf( (char*)fridge_labelTempFZ.caption, 10, "%2i.%02i F", tmpData / 100, mod );
            GuiDriver_LabelDraw(&fridge_labelTempFZ);
    	}

    }

}
