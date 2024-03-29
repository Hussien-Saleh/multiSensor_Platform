
/**
 * \file
 *
 * \brief TC related functionality declaration.
 *
 * Copyright (c) 2019 Microchip Technology Inc. and its subsidiaries.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Subject to your compliance with these terms, you may use Microchip
 * software and any derivatives exclusively with Microchip products.
 * It is your responsibility to comply with third party license terms applicable
 * to your use of third party software (including open source software) that
 * may accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
 * LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
 * SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 * RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * \asf_license_stop
 *
 */

#ifndef _TC_H_INCLUDED
#define _TC_H_INCLUDED

#include <compiler.h>
#include <utils_assert.h>

/**
 * \addtogroup tc driver
 *
 * \section tc Revision History
 * - v0.0.0.1 Initial Commit
 *
 *@{
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Initialize tc interface
 * \return Initialization status.
 */
int8_t SCHEDULER_init();

/**
 * \brief Start timer
 *
 * This function starts the timer for given channel.
 * It checks if the given channel is valid.
 *
 * \param[in] hw A TC hardware instance pointer(e.g TC0, TC1, .. etc)
 * \param[in] channel Channel of the TC instance
 *
 */
void start_timer(const void *hw, uint8_t channel);

/**
 * \brief Stop timer
 *
 * This function stop the timer for given channel.
 * It checks if the given channel is valid.
 *
 * \param[in] hw A TC hardware instance pointer(e.g TC0, TC1, .. etc)
 * \param[in] channel Channel of the TC instance
 *
 */
void stop_timer(const void *hw, uint8_t channel);

/**
 * \brief Register callback function
 *
 * This function registers one callback function to the each TC channels interrupt
 *
 * \param[in] hw A TC hardware instance pointer(e.g TC0, TC1, .. etc)
 * \param[in] channel Channel of the TC instance
 * \param[in] func Callback function pointer
 *
 */
void tc_register_callback(void *hw, uint8_t channel, void *cb);

#ifdef __cplusplus
}
#endif

#endif /* _TC_H_INCLUDED */
