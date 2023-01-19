/**
 * \file
 *
 * \brief SAM Analog to Digital Converter (ADC)
 *
 * Copyright (c) 2016-2018 Microchip Technology Inc. and its subsidiaries.
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

#include <hpl_adc_sync.h>
#include <hpl_adc_async.h>
#include <utils_assert.h>
#include <utils.h>
#include "hpl_adc_config.h"

/** Pointer to hpl device */
static struct _adc_async_device *_adc_dev = NULL;

/**
 * \brief Initialize ADC
 *
 * \param[in] hw The pointer to hardware instance
 * \param[in] i The number of hardware instance
 */
static int32_t _adc_init(void *const hw)
{
	hri_adc_write_MR_reg(hw, CONF_ADC_MR_REG);
	hri_adc_write_EMR_reg(hw, CONF_ADC_EMR_REG);
	hri_adc_write_SEQR1_reg(hw, CONF_ADC_SEQR1_REG);
	hri_adc_write_ACR_reg(hw, CONF_ADC_ACR_REG);
	hri_adc_write_LCCWR_reg(hw, CONF_ADC_LCCWR_REG);
	hri_adc_set_CHSR_reg(hw, CONF_ADC_CHER_REG);
	hri_adc_write_COR_reg(hw, CONF_ADC_COR_REG);

	return ERR_NONE;
}

/**
 * \brief De-initialize ADC
 *
 * \param[in] hw The pointer to hardware instance
 */
static inline void _adc_deinit(void *hw)
{
	hri_adc_clear_CHSR_reg(hw, ADC_CHDR_MASK);
	hri_adc_write_CR_reg(hw, ADC_CR_SWRST);
}

/**
 * \internal ADC interrupt handler
 */
void ADC_Handler(void)
{
	void *const       hw = _adc_dev->hw;
	volatile uint32_t status;
	uint8_t           cnt = 0;

	status = hri_adc_read_ISR_reg(hw) & hri_adc_read_IMR_reg(hw);
	if (status & ADC_IMR_COMPE) {
		_adc_dev->adc_async_cb.window_cb(_adc_dev, cnt);
	}
	if (status & ADC_IMR_GOVRE) {
		_adc_dev->adc_async_cb.error_cb(_adc_dev, cnt);
	}
	status &= 0xFF; /* Mask EOC0 to EOC7 */
	cnt = 32 - clz(status);
	while (cnt) {
		cnt--;
		_adc_dev->adc_async_ch_cb.convert_done(_adc_dev, cnt, hri_adc_read_CDR_reg(_adc_dev->hw, cnt));
		status &= ~(1 << cnt);
		cnt = 32 - clz(status);
	}
}

/**
 * \brief Initialize ADC
 */
int32_t _adc_sync_init(struct _adc_sync_device *const device, void *const hw)
{
	ASSERT(device);

	device->hw = hw;

	return _adc_init(hw);
}

/**
 * \brief Initialize ADC
 */
int32_t _adc_async_init(struct _adc_async_device *const device, void *const hw)
{
	int32_t init_status;

	ASSERT(device);

	init_status = _adc_init(hw);
	if (init_status) {
		return init_status;
	}

	device->hw = hw;
	_adc_dev   = device;
	NVIC_DisableIRQ(ADC_IRQn);
	NVIC_ClearPendingIRQ(ADC_IRQn);
	NVIC_EnableIRQ(ADC_IRQn);

	return ERR_NONE;
}

/**
 * \brief De-initialize ADC
 */
void _adc_sync_deinit(struct _adc_sync_device *const device)
{
	_adc_deinit(device->hw);
}

/**
 * \brief De-initialize ADC
 */
void _adc_async_deinit(struct _adc_async_device *const device)
{
	NVIC_DisableIRQ(ADC_IRQn);
	NVIC_ClearPendingIRQ(ADC_IRQn);

	_adc_deinit(device->hw);
}

/**
 * \brief Enable ADC
 */
void _adc_sync_enable_channel(struct _adc_sync_device *const device, const uint8_t channel)
{
	hri_adc_set_CHSR_reg(device->hw, (1 << channel));
}

/**
 * \brief Enable ADC
 */
void _adc_async_enable_channel(struct _adc_async_device *const device, const uint8_t channel)
{
	hri_adc_set_CHSR_reg(device->hw, (1 << channel));
}

/**
 * \brief Disable ADC
 */
void _adc_sync_disable_channel(struct _adc_sync_device *const device, const uint8_t channel)
{
	hri_adc_clear_CHSR_reg(device->hw, (1 << channel));
}

/**
 * \brief Disable ADC
 */
void _adc_async_disable_channel(struct _adc_async_device *const device, const uint8_t channel)
{
	hri_adc_clear_CHSR_reg(device->hw, (1 << channel));
}

/**
 * \brief Retrieve ADC conversion data size
 */
uint8_t _adc_sync_get_data_size(const struct _adc_sync_device *const device)
{
	(void)device;

	return 2;
}

/**
 * \brief Retrieve ADC conversion data size
 */
uint8_t _adc_async_get_data_size(const struct _adc_async_device *const device)
{
	(void)device;

	return 2;
}

/**
 * \brief Check if conversion is done
 */
bool _adc_sync_is_channel_conversion_done(const struct _adc_sync_device *const device, const uint8_t channel)
{
	return (bool)hri_adc_get_ISR_reg(device->hw, (1 << channel));
}

/**
 * \brief Check if conversion is done
 */
bool _adc_async_is_channel_conversion_done(const struct _adc_async_device *const device, const uint8_t channel)
{
	return (bool)hri_adc_get_ISR_reg(device->hw, (1 << channel));
}

/**
 * \brief Make conversion
 */
void _adc_sync_convert(struct _adc_sync_device *const device)
{
	hri_adc_write_CR_reg(device->hw, ADC_CR_START);
}

/**
 * \brief Make conversion
 */
void _adc_async_convert(struct _adc_async_device *const device)
{
	hri_adc_write_CR_reg(device->hw, ADC_CR_START);
}

/**
 * \brief Retrieve the conversion result
 */
uint16_t _adc_sync_read_channel_data(const struct _adc_sync_device *const device, const uint8_t channel)
{
	return hri_adc_read_CDR_reg(device->hw, channel);
}

/**
 * \brief Retrieve the conversion result
 */
uint16_t _adc_async_read_channel_data(const struct _adc_async_device *const device, const uint8_t channel)
{
	return hri_adc_read_CDR_reg(device->hw, channel);
}

/**
 * \brief Set reference source
 */
void _adc_sync_set_reference_source(struct _adc_sync_device *const device, const adc_reference_t reference)
{
	(void)device;
	(void)reference;
}

/**
 * \brief Set reference source
 */
void _adc_async_set_reference_source(struct _adc_async_device *const device, const adc_reference_t reference)
{
	(void)device;
	(void)reference;
}

/**
 * \brief Set resolution
 */
void _adc_sync_set_resolution(struct _adc_sync_device *const device, const adc_resolution_t resolution)
{
	hri_adc_write_EMR_OSR_bf(device->hw, resolution);
}

/**
 * \brief Set resolution
 */
void _adc_async_set_resolution(struct _adc_async_device *const device, const adc_resolution_t resolution)
{
	hri_adc_write_EMR_OSR_bf(device->hw, resolution);
}

/**
 * \brief Set channels input sources
 */
void _adc_sync_set_inputs(struct _adc_sync_device *const device, const adc_pos_input_t pos_input,
                          const adc_neg_input_t neg_input, const uint8_t channel)
{
	(void)device;
	(void)pos_input;
	(void)neg_input;
	(void)channel;
}

/**
 * \brief Set channels input sources
 */
void _adc_async_set_inputs(struct _adc_async_device *const device, const adc_pos_input_t pos_input,
                           const adc_neg_input_t neg_input, const uint8_t channel)
{
	(void)device;
	(void)pos_input;
	(void)neg_input;
	(void)channel;
}

/**
 * \brief Set thresholds
 */
void _adc_sync_set_thresholds(struct _adc_sync_device *const device, const adc_threshold_t low_threshold,
                              const adc_threshold_t up_threshold)
{
	hri_adc_write_LCCWR_reg(device->hw, ADC_LCCWR_LOWTHRES(low_threshold) | ADC_LCCWR_HIGHTHRES(up_threshold));
}

/**
 * \brief Set thresholds
 */
void _adc_async_set_thresholds(struct _adc_async_device *const device, const adc_threshold_t low_threshold,
                               const adc_threshold_t up_threshold)
{
	hri_adc_write_LCCWR_reg(device->hw, ADC_LCCWR_LOWTHRES(low_threshold) | ADC_LCCWR_HIGHTHRES(up_threshold));
}

/**
 * \brief Set gain
 */
void _adc_sync_set_channel_gain(struct _adc_sync_device *const device, const uint8_t channel, const adc_gain_t gain)
{
	(void)device;
	(void)channel;
	(void)gain;
}

/**
 * \brief Set gain
 */
void _adc_async_set_channel_gain(struct _adc_async_device *const device, const uint8_t channel, const adc_gain_t gain)
{
	(void)device;
	(void)channel;
	(void)gain;
}

/**
 * \brief Set conversion mode
 */
void _adc_sync_set_conversion_mode(struct _adc_sync_device *const device, const enum adc_conversion_mode mode)
{
	if (ADC_CONVERSION_MODE_FREERUN == mode) {
		hri_adc_set_MR_FREERUN_bit(device->hw);
	} else {
		hri_adc_clear_MR_FREERUN_bit(device->hw);
	}
}

/**
 * \brief Set conversion mode
 */
void _adc_async_set_conversion_mode(struct _adc_async_device *const device, const enum adc_conversion_mode mode)
{
	if (ADC_CONVERSION_MODE_FREERUN == mode) {
		hri_adc_set_MR_FREERUN_bit(device->hw);
	} else {
		hri_adc_clear_MR_FREERUN_bit(device->hw);
	}
}

/**
 * \brief Set differential mode
 */
void _adc_sync_set_channel_differential_mode(struct _adc_sync_device *const device, const uint8_t channel,
                                             const enum adc_differential_mode mode)
{
	if (ADC_DIFFERENTIAL_MODE_DIFFERENTIAL == mode) {
		hri_adc_set_COR_reg(device->hw, (ADC_COR_DIFF0 << channel));
	} else {
		hri_adc_clear_COR_reg(device->hw, (ADC_COR_DIFF0 << channel));
	}
}

/**
 * \brief Set differential mode
 */
void _adc_async_set_channel_differential_mode(struct _adc_async_device *const device, const uint8_t channel,
                                              const enum adc_differential_mode mode)
{
	ASSERT(!(channel % 2));
	if (ADC_DIFFERENTIAL_MODE_DIFFERENTIAL == mode) {
		hri_adc_set_COR_reg(device->hw, (ADC_COR_DIFF0 << channel));
	} else {
		hri_adc_clear_COR_reg(device->hw, (ADC_COR_DIFF0 << channel));
	}
}

/**
 * \brief Set window mode
 */
void _adc_sync_set_window_mode(struct _adc_sync_device *const device, const adc_window_mode_t mode)
{
	hri_adc_write_EMR_CMPMODE_bf(device->hw, mode);
}

/**
 * \brief Set window mode
 */
void _adc_async_set_window_mode(struct _adc_async_device *const device, const adc_window_mode_t mode)
{
	hri_adc_write_EMR_CMPMODE_bf(device->hw, mode);
}

/**
 * \brief Retrieve threshold state
 */
void _adc_sync_get_threshold_state(const struct _adc_sync_device *const device, adc_threshold_status_t *const state)
{
	*state = hri_adc_get_ISR_COMPE_bit(device->hw);
}

/**
 * \brief Retrieve threshold state
 */
void _adc_async_get_threshold_state(const struct _adc_async_device *const device, adc_threshold_status_t *const state)
{
	*state = hri_adc_get_ISR_COMPE_bit(device->hw);
}

/**
 * \brief Enable/disable ADC interrupt
 *
 * param[in] device The pointer to ADC device instance
 * param[in] type The type of interrupt to disable/enable if applicable
 * param[in] state Enable or disable
 */
void _adc_async_set_irq_state(struct _adc_async_device *const device, const uint8_t channel,
                              const enum _adc_async_callback_type type, const bool state)
{
	void *const hw = device->hw;

	if (ADC_ASYNC_DEVICE_MONITOR_CB == type) {
		hri_adc_write_IMR_COMPE_bit(hw, state);
	} else if (ADC_ASYNC_DEVICE_ERROR_CB == type) {
		hri_adc_write_IMR_GOVRE_bit(hw, state);
	} else if (ADC_ASYNC_DEVICE_CONVERT_CB == type) {
		if (state) {
			hri_adc_set_IMR_reg(hw, (0x1u << channel));
		} else {
			hri_adc_clear_IMR_reg(hw, (0x1u << channel));
		}
	}
}
