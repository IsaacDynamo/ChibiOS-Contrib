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
 * @file    GPIOv3/hal_pal_lld.h
 * @brief   SN32 PAL low level driver header.
 * Supported devices are 
 * - SN32F24x     (Supports pull-down on inputs and 20mA outputs, has input repeater instead of input disconnect)
 * - SN32F24xB
 * - SN32F26x     (Supports 20mA outputs)
 *
 * @addtogroup PAL
 * @{
 */

#ifndef HAL_PAL_LLD_H
#define HAL_PAL_LLD_H

#if HAL_USE_PAL || defined(__DOXYGEN__)

/*===========================================================================*/
/* Unsupported modes and specific modes                                      */
/*===========================================================================*/

// /* Specifies palInit() without parameter, required until all platforms will
//    be updated to the new style.*/
#define PAL_NEW_INIT


// Redefine all PAL modes
#undef PAL_MODE_RESET                  
#undef PAL_MODE_UNCONNECTED            
#undef PAL_MODE_INPUT                  
#undef PAL_MODE_INPUT_PULLUP           
#undef PAL_MODE_INPUT_PULLDOWN         
#undef PAL_MODE_INPUT_ANALOG           
#undef PAL_MODE_OUTPUT_PUSHPULL        
#undef PAL_MODE_OUTPUT_OPENDRAIN       

// NOTE values for PAL_MODE_* are choses such that they somewhat match GPIOn_CFG.CFGn

#define PAL_MODE_INPUT_PULLUP           0x0

// The SN3224xB has pull-down support
#if defined(SN32_SERIES_SN32F24xB)
#define PAL_MODE_INPUT_PULLDOWN         0x1
#endif

// GPIO are configured as input on reset
#define PAL_MODE_RESET                  0x2
#define PAL_MODE_INPUT                  0x2

// SN32F24x has repeater mode, while others have an unconnected mode.
#if defined(SN32_SERIES_SN32F24x)
#define PAL_MODE_INPUT_REPEATER         0x3
#else
#define PAL_MODE_UNCONNECTED            0x3
#endif

#define PAL_MODE_OUTPUT_PUSHPULL        0x10

// The SN32F24x and SN32F26x support an extra 20mA output mode
#if defined(SN32_SERIES_SN32F24x) || defined(SN32_SERIES_SN32F26x)
#define PAL_MODE_OUTPUT_PUSHPULL_20MA   0x20  
#endif

/** @} */

/*===========================================================================*/
/* I/O Ports Types and constants.                                            */
/*===========================================================================*/

/**
 * @name    Port related definitions
 * @{
 */
/**
 * @brief   Width, in bits, of an I/O port.
 */
#define PAL_IOPORTS_WIDTH 16

/**
 * @brief   Whole port mask.
 * @details This macro specifies all the valid bits into a port.
 */
#define PAL_WHOLE_PORT ((ioportmask_t)0xFFFF)
/** @} */


/**
 * @name    Line handling macros
 * @{
 */
/**
 * @brief   Forms a line identifier.
 * @details A port/pad pair are encoded into an @p ioline_t type. The encoding
 *          of this type is platform-dependent.
 * @note    In this driver the pad number is encoded in the lower 4 bits of
 *          the GPIO address which are guaranteed to be zero.
 */
#define PAL_LINE(port, pad)                                                 \
  ((ioline_t)((uint32_t)(port)) | ((uint32_t)(pad)))

/**
 * @brief   Decodes a port identifier from a line identifier.
 */
#define PAL_PORT(line)                                                      \
  ((SN_GPIO0_Type *)(((uint32_t)(line)) & 0xFFFFFFF0U))

/**
 * @brief   Decodes a pad identifier from a line identifier.
 */
#define PAL_PAD(line)                                                       \
  ((uint32_t)((uint32_t)(line) & 0x0000000FU))

/**
 * @brief   Value identifying an invalid line.
 */
#define PAL_NOLINE                      0U
/** @} */

/**
 * @brief   Type of digital I/O port sized unsigned integer.
 */
typedef uint32_t ioportmask_t;

/**
 * @brief   Type of digital I/O modes.
 */
typedef uint32_t iomode_t;

/**
 * @brief   Type of an I/O line.
 */
typedef uint32_t ioline_t;

/**
 * @brief   Port Identifier.
 * @details This type can be a scalar or some kind of pointer, do not make
 *          any assumption about it, use the provided macros when populating
 *          variables of this type.
 */
typedef SN_GPIO0_Type * ioportid_t;

/*===========================================================================*/
/* I/O Ports Identifiers.                                                    */
/* The low level driver wraps the definitions already present in the SN32   */
/* firmware library.                                                         */
/*===========================================================================*/

/**
 * @brief   GPIO port A identifier.
 */
#if SN32_HAS_GPIOA || defined(__DOXYGEN__)
#define GPIOA                         ((SN_GPIO0_Type *)SN_GPIO0_BASE)
#define IOPORT1         GPIOA
#endif

/**
 * @brief   GPIO port B identifier.
 */
#if SN32_HAS_GPIOB || defined(__DOXYGEN__)
#define GPIOB                         ((SN_GPIO0_Type *)SN_GPIO1_BASE)
#define IOPORT2         GPIOB
#endif

/**
 * @brief   GPIO port C identifier.
 */
#if SN32_HAS_GPIOC || defined(__DOXYGEN__)
#define GPIOC                         ((SN_GPIO0_Type *)SN_GPIO2_BASE)
#define IOPORT3         GPIOC
#endif

/**
 * @brief   GPIO port D identifier.
 */
#if SN32_HAS_GPIOD || defined(__DOXYGEN__)
#define GPIOD                         ((SN_GPIO0_Type *)SN_GPIO3_BASE)
#define IOPORT4         GPIOD
#endif

/*===========================================================================*/
/* Implementation, some of the following macros could be implemented as      */
/* functions, if so please put them in pal_lld.c.                            */
/*===========================================================================*/

/**
 * @brief   Low level PAL subsystem initialization.
 *
 * @notapi
 */
#define pal_lld_init() _pal_lld_init()

/**
 * @brief   Reads the physical I/O port states.
 *
 * @param[in] port      port identifier
 * @return              The port bits.
 *
 * @notapi
 */
#define pal_lld_readport(port) ((port)->DATA)

/**
 * @brief   Reads the output latch.
 * @details The purpose of this function is to read back the latched output
 *          value.
 *
 * @param[in] port      port identifier
 * @return              The latched logical states.
 *
 * @notapi
 */
// pal_lld_readlatch() is not support because SN32 has no way to readback the port output data
#define pal_lld_readlatch(port)   _pal_lld_readlatch_not_suported_on_SN32(port)

/**
 * @brief   Writes a bits mask on a I/O port.
 *
 * @param[in] port      port identifier
 * @param[in] bits      bits to be written on the specified port
 *
 * @notapi
 */
#define pal_lld_writeport(port, bits) ((port)->DATA = (uint16_t)(bits))

/**
 * @brief   Sets a bits mask on a I/O port.
 * @note    The @ref PAL provides a default software implementation of this
 *          functionality, implement this function if can optimize it by using
 *          special hardware functionalities or special coding.
 *
 * @param[in] port      port identifier
 * @param[in] bits      bits to be ORed on the specified port
 *
 * @notapi
 */
#define pal_lld_setport(port, bits) ((port)->BSET = (uint16_t)(bits))

/**
 * @brief   Clears a bits mask on a I/O port.
 * @note    The @ref PAL provides a default software implementation of this
 *          functionality, implement this function if can optimize it by using
 *          special hardware functionalities or special coding.
 *
 * @param[in] port      port identifier
 * @param[in] bits      bits to be cleared on the specified port
 *
 * @notapi
 */
#define pal_lld_clearport(port, bits) ((port)->BCLR = (uint16_t)(bits))

/**
 * @brief   Toggles a bits mask on a I/O port.
 * @note    The @ref PAL provides a default software implementation of this
 *          functionality, implement this function if can optimize it by using
 *          special hardware functionalities or special coding.
 *
 * @param[in] port      port identifier
 * @param[in] bits      bits to be XORed on the specified port
 *
 * @notapi
 */
// pal_lld_toggleport() is not supported, SN32 has no way to toggle a port with a bit mask
#define pal_lld_toggleport(port, bits) _pal_lld_toggleport_not_suported_on_SN32()

/**
 * @brief   Reads a group of bits.
 * @note    The @ref PAL provides a default software implementation of this
 *          functionality, implement this function if can optimize it by using
 *          special hardware functionalities or special coding.
 *
 * @param[in] port      port identifier
 * @param[in] mask      group mask
 * @param[in] offset    group bit offset within the port
 * @return              The group logical states.
 *
 * @notapi
 */
// No pal_lld_readgroup(), implemented in terms of pal_lld_readport()

/**
 * @brief   Writes a group of bits.
 * @note    The @ref PAL provides a default software implementation of this
 *          functionality, implement this function if can optimize it by using
 *          special hardware functionalities or special coding.
 *
 * @param[in] port      port identifier
 * @param[in] mask      group mask
 * @param[in] offset    group bit offset within the port
 * @param[in] bits      bits to be written. Values exceeding the group width
 *                      are masked.
 *
 * @notapi
 */
// TODO
#define pal_lld_writegroup(port, mask, offset, bits) _pal_lld_writegroup(port, mask, offset, bits)
static inline void _pal_lld_writegroup(ioportid_t port, ioportmask_t mask, unsigned int offset, ioportmask_t bits){
    pal_lld_setport(port, (bits & mask) << offset);
    pal_lld_clearport(port, (~bits & mask) << offset);
}

/**
 * @brief   Pads group mode setup.
 * @details This function programs a pads group belonging to the same port
 *          with the specified mode.
 * @note    Programming an unknown or unsupported mode is silently ignored.
 *
 * @param[in] port      port identifier
 * @param[in] mask      group mask
 * @param[in] offset    group bit offset within the port
 * @param[in] mode      group mode
 *
 * @notapi
 */
#define pal_lld_setgroupmode(port, mask, offset, mode) _pal_lld_setgroupmode(port, mask << offset, mode)

/**
 * @brief   Reads a logical state from an I/O pad.
 * @note    The @ref PAL provides a default software implementation of this
 *          functionality, implement this function if can optimize it by using
 *          special hardware functionalities or special coding.
 *
 * @param[in] port      port identifier
 * @param[in] pad       pad number within the port
 * @return              The logical state.
 * @retval PAL_LOW      low logical state.
 * @retval PAL_HIGH     high logical state.
 *
 * @notapi
 */
// No pal_lld_readpad(), Implemented in terms of pal_lld_readport()

/**
 * @brief   Writes a logical state on an output pad.
 * @note    This function is not meant to be invoked directly by the
 *          application  code.
 * @note    The @ref PAL provides a default software implementation of this
 *          functionality, implement this function if can optimize it by using
 *          special hardware functionalities or special coding.
 *
 * @param[in] port      port identifier
 * @param[in] pad       pad number within the port
 * @param[in] bit       logical value, the value must be @p PAL_LOW or
 *                      @p PAL_HIGH
 *
 * @notapi
 */
#define pal_lld_writepad(port, pad, bit) ( bit ? pal_lld_setpad(port, pad) : pal_lld_clearpad(port, pad) )

/**
 * @brief   Sets a pad logical state to @p PAL_HIGH.
 * @note    The @ref PAL provides a default software implementation of this
 *          functionality, implement this function if can optimize it by using
 *          special hardware functionalities or special coding.
 *
 * @param[in] port      port identifier
 * @param[in] pad       pad number within the port
 *
 * @notapi
 */
// No pal_lld_setpad(), Implemented in terms of pal_lld_setport()

/**
 * @brief   Clears a pad logical state to @p PAL_LOW.
 * @note    The @ref PAL provides a default software implementation of this
 *          functionality, implement this function if can optimize it by using
 *          special hardware functionalities or special coding.
 *
 * @param[in] port      port identifier
 * @param[in] pad       pad number within the port
 *
 * @notapi
 */
// No pal_lld_clearpad(), Implemented in terms of pal_lld_clearport()

/**
 * @brief   Toggles a pad logical state.
 * @note    The @ref PAL provides a default software implementation of this
 *          functionality, implement this function if can optimize it by using
 *          special hardware functionalities or special coding.
 *
 * @param[in] port      port identifier
 * @param[in] pad       pad number within the port
 *
 * @notapi
 */
// pal_lld_togglepad() is not supported, SN32 had no optimized way to toggle a pad.
#define pal_lld_togglepad(port, pad) _pal_lld_togglepad_not_suported_on_SN32()

/**
 * @brief   Pad mode setup.
 * @details This function programs a pad with the specified mode.
 * @note    The @ref PAL provides a default software implementation of this
 *          functionality, implement this function if can optimize it by using
 *          special hardware functionalities or special coding.
 * @note    Programming an unknown or unsupported mode is silently ignored.
 *
 * @param[in] port      port identifier
 * @param[in] pad       pad number within the port
 * @param[in] mode      pad mode
 *
 * @notapi
 */
// No pal_lld_setpadmode(), Implemented in terms of pal_lld_setgroupmode()

#ifdef __cplusplus
extern "C" {
#endif
    void _pal_lld_init(void);
    void _pal_lld_setgroupmode(ioportid_t port, ioportmask_t mask, iomode_t mode);

    // Defined but not implentented, these will trigger linker errors when used.
    void _pal_lld_readlatch_not_suported_on_SN32(void);
    void _pal_lld_toggleport_not_suported_on_SN32(void);
    void _pal_lld_togglepad_not_suported_on_SN32(void);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_PAL */

#endif /* HAL_PAL_LLD_H */

/** @} */
