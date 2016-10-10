/**
 * \file gui_weatherStation_objects.c
 * \version 1.00
 * \brief this file contains weather station app related GUI objects
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

#include "screens_common.h"
#include "fridge_private.h"
#include "fridge.h"

static guiScreen_t
		fridge_mainScreen;

guiScreen_t
    fridge_coverScreen =
    {
        .navigation =
        {
            .up     = &appsScreen,
            .down   = &settingsScreen,
            .left   = &watchScreen,
            .right  = &fridge_mainScreen
        },

        .image = fridge_cover_bmp,

        .initFunction        = NULL,
        .createTaskFunction  = NULL,
        .destroyTaskFunction = NULL
    };

static guiScreen_t
    fridge_mainScreen =
    {
        .navigation =
        {
            .up     = NULL,
            .down   = NULL,
            .left   = &fridge_coverScreen,
            .right  = NULL
        },

        .image = fridge_appScreen_bmp,

        .initFunction        = fridge_Init,
        .createTaskFunction  = fridge_CreateTasks,
        .destroyTaskFunction = fridge_DestroyTasks
    };

/** labels */
/** temperature Fresh Food*/
guiLabel_t fridge_labelTempFF =
{
    .dynamicArea =
    {
        .xCrd   = 30,
        .yCrd   = 28,
        .width  = 50,
        .height = 15
    },
    .textProperties =
    {
        .font       = guiFont_Tahoma_8_Regular,
        .fontColor  = GUI_COLOR_BLUE,
        .alignParam = OLED_TEXT_ALIGN_LEFT,
        .background = fridge_appScreen_bmp
    },
    .caption = NULL,
    .captionLength = 10
};

/** temperature Freezer */
guiLabel_t fridge_labelTempFZ =
{
    .dynamicArea =
    {
        .xCrd   = 30,
        .yCrd   = 65,
        .width  = 50,
        .height = 15
    },
    .textProperties =
    {
        .font       = guiFont_Tahoma_8_Regular,
        .fontColor  = GUI_COLOR_BLUE,
        .alignParam = OLED_TEXT_ALIGN_LEFT,
        .background = fridge_appScreen_bmp
    },
    .caption = NULL,
    .captionLength = 10
};

