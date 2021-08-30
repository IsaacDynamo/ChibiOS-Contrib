/*
    ChibiOS - Copyright (C) 2006..2016 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

/**
 * @file    GPIOv3/hal_pal_lld.c
 * @brief   SN32 PAL low level driver code.
 *
 * @addtogroup PAL
 * @{
 */

#include "hal.h"

#if HAL_USE_PAL || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   SN32 I/O ports configuration.
 * @details Ports A-D clocks enabled.
 *
 * @param[in] config    the SN32 ports configuration
 *
 * @notapi
 */
void _pal_lld_init(void) {

    #if SN32_PAL_DISABLE_RESET
        SN_SYS0->EXRSTCTRL_b.RESETDIS = 1; // Disable RESET
    #endif

    #if SN32_PAL_DISABLE_SWD
        SN_SYS0->SWDCTRL_b.SWDDIS = 1; // Disable SWD
    #endif
}

/**
 * @brief   Pads mode setup.
 * @details This function programs a pads group belonging to the same port
 *          with the specified mode.
 *
 * @param[in] port      the port identifier
 * @param[in] mask      the group mask
 * @param[in] mode      the mode
 *
 * @notapi
 */
void _pal_lld_setgroupmode(ioportid_t port,
                           ioportmask_t mask,
                           iomode_t mode) {
            
    switch (mode)
    {

    //   PAL_MODE_RESET
    case PAL_MODE_INPUT:            // fallthrough
#ifdef PAL_MODE_INPUT_PULLDOWN
    case PAL_MODE_INPUT_PULLDOWN:   // fallthrough
#endif
#ifdef PAL_MODE_UNCONNECTED
    case PAL_MODE_UNCONNECTED:      // fallthrough
#endif
#ifdef PAL_MODE_INPUT_REPEATER
    case PAL_MODE_INPUT_REPEATER:   // fallthrough
#endif
    case PAL_MODE_INPUT_PULLUP:
    {    
        // Create value and mask for CFG
        uint32_t cfg_value = 0;
        uint32_t cfg_mask = 0;
        for (int bit = 0; bit < PAL_IOPORTS_WIDTH; bit++) {
            if (mask & (1<<bit)) {
                cfg_value |= mode << (bit * 2);
                cfg_mask  |=  0x3 << (bit * 2);
            }
        }

        // Update the CFG with the value and mask
        port->CFG  = (port->CFG  & ~cfg_mask) | cfg_value;

        // Clear CURRENT and MODE
        port->MODE = port->MODE & ~(mask << 16) & ~mask;
    }   break;

    case PAL_MODE_OUTPUT_PUSHPULL:
        // Clear CURRENT and set MODE
        port->MODE = (port->MODE & ~(mask << 16)) | mask;
        break;

#ifdef PAL_MODE_OUTPUT_PUSHPULL_20MA
    case PAL_MODE_OUTPUT_PUSHPULL_20MA:
        // Set CURRENT and MODE
        port->MODE = port->MODE | (mask << 16) | mask;
        break;
#endif

    default:
        break;
    }
}

#endif /* HAL_USE_PAL */

/** @} */
