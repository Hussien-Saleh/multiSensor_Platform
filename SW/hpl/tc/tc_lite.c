
/**
 * \file
 *
 * \brief TC related functionality implementation.
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

#include "tc_lite.h"

#ifndef TC_CMR_CPCSTOP_Pos
#define TC_CMR_CPCSTOP_Pos 6
#endif
#ifndef TC_CMR_CPCDIS_Pos
#define TC_CMR_CPCDIS_Pos 7
#endif
#ifndef TC_CMR_EEVTEDG_Pos
#define TC_CMR_EEVTEDG_Pos 8
#endif
#ifndef TC_CMR_EEVT_Pos
#define TC_CMR_EEVT_Pos 10
#endif
#ifndef TC_CMR_ENETRG_Pos
#define TC_CMR_ENETRG_Pos 12
#endif
#ifndef TC_CMR_WAVSEL_Pos
#define TC_CMR_WAVSEL_Pos 13
#endif
#ifndef TC_CMR_ACPA_Pos
#define TC_CMR_ACPA_Pos 16
#endif
#ifndef TC_CMR_ACPC_Pos
#define TC_CMR_ACPC_Pos 18
#endif
#ifndef TC_CMR_AEEVT_Pos
#define TC_CMR_AEEVT_Pos 20
#endif
#ifndef TC_CMR_ASWTRG_Pos
#define TC_CMR_ASWTRG_Pos 22
#endif
#ifndef TC_CMR_BCPB_Pos
#define TC_CMR_BCPB_Pos 24
#endif
#ifndef TC_CMR_BCPC_Pos
#define TC_CMR_BCPC_Pos 26
#endif
#ifndef TC_CMR_BEEVT_Pos
#define TC_CMR_BEEVT_Pos 28
#endif
#ifndef TC_CMR_BSWTRG_Pos
#define TC_CMR_BSWTRG_Pos 30
#endif
#ifndef TC_CMR_SBSMPLR_Pos
#define TC_CMR_SBSMPLR_Pos 20
#endif

#define TC_CHANNEL_0 0
#define TC_CHANNEL_1 1
#define TC_CHANNEL_2 2
#define TC_CHANNEL_COUNT 3

/**
 * \brief TC IRQ callback type
 */
typedef void (*tc_channel_ptr)(uint32_t);

/* TC1 channel interrupt callback array */
tc_channel_ptr tc1_channel_cb[TC_CHANNEL_COUNT];

/**
 * \brief Initialize TC interface
 */
int8_t SCHEDULER_init()
{

	/* TC1 Channel 2 configuration */

	hri_tc_write_CMR_reg(TC1, TC_CHANNEL_2, 1 << TC_CMR_TCCLKS_Pos | 1 << TC_CMR_CPCTRG_Pos);

	hri_tc_write_RC_reg(TC1, TC_CHANNEL_2, 0xbb8 << TC_RC_RC_Pos);

	hri_tc_write_IMR_reg(TC1, TC_CHANNEL_2, 1 << TC_IMR_CPCS_Pos);

	tc1_channel_cb[TC_CHANNEL_0] = NULL;
	NVIC_DisableIRQ(TC3_IRQn);
	NVIC_ClearPendingIRQ(TC3_IRQn);
	NVIC_EnableIRQ(TC3_IRQn);
	tc1_channel_cb[TC_CHANNEL_1] = NULL;
	NVIC_DisableIRQ(TC4_IRQn);
	NVIC_ClearPendingIRQ(TC4_IRQn);
	NVIC_EnableIRQ(TC4_IRQn);
	tc1_channel_cb[TC_CHANNEL_2] = NULL;
	NVIC_DisableIRQ(TC5_IRQn);
	NVIC_ClearPendingIRQ(TC5_IRQn);
	NVIC_EnableIRQ(TC5_IRQn);

	return 0;
}

void start_timer(const void *hw, uint8_t channel)
{
	if (channel < TC_CHANNEL_COUNT) {
		hri_tc_write_CCR_reg(hw, channel, TC_CCR_CLKEN | TC_CCR_SWTRG);
	}
}

void stop_timer(const void *hw, uint8_t channel)
{
	if (channel < TC_CHANNEL_COUNT) {
		hri_tc_write_CCR_reg(hw, channel, TC_CCR_CLKDIS);
	}
}

void tc_register_callback(void *hw, uint8_t channel, void *cb)
{
	ASSERT(hw && (channel < TC_CHANNEL_COUNT));
	if (hw == TC1) {
		tc1_channel_cb[channel] = cb;
	}
}

/* TC1 Channel 0 interrupt handler */
void TC3_Handler(void)
{
	uint32_t status;
	status = hri_tc_get_SR_reg(TC1, TC_CHANNEL_0, TC_SR_Msk);
	if (tc1_channel_cb[TC_CHANNEL_0] != NULL) {
		tc1_channel_cb[TC_CHANNEL_0](status);
	}
}
/* TC1 Channel 1 interrupt handler */
void TC4_Handler(void)
{
	uint32_t status;
	status = hri_tc_get_SR_reg(TC1, TC_CHANNEL_1, TC_SR_Msk);
	if (tc1_channel_cb[TC_CHANNEL_1] != NULL) {
		tc1_channel_cb[TC_CHANNEL_1](status);
	}
}
/* TC1 Channel 2 interrupt handler */
void TC5_Handler(void)
{
	uint32_t status;
	status = hri_tc_get_SR_reg(TC1, TC_CHANNEL_2, TC_SR_Msk);
	if (tc1_channel_cb[TC_CHANNEL_2] != NULL) {
		tc1_channel_cb[TC_CHANNEL_2](status);
	}
}
