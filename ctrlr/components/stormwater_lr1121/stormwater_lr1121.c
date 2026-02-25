/*	name: stormwater_lr1121.c
 *	date: 02-24-2026
 *	auth: wrental
 *	desc: contains functional implementation of Waveshare Core1121-XF module
 */

/*!
 * @file stormwater_lr1121.c
 *
 * @brief contains implementation of Waveshare Core1121-XF module
 *
 * @copyright
 * Stormwater Drone Controller for Elon University SPEED 2025-2026
 * Copyright (C) 2026 wrental
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTIBILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#include "stormwater_lr1121.h"
#include "ctrlr.h"
#include "lr11xx_radio.h"
#include "lr11xx_radio_types.h"

static struct {
	uint8_t cs, 		LR1121_CS;
	uint8_t reset, 	LR1121_RESET;
	uint8_t busy, 	LR1121_BUSY;
	uint8_t irq, 		LR1121_INT;
} lr1121;

// LoRa modulation parameters
static const lr11xx_radio_mod_params_lora_t lora_mod_params = {
  .sf   = LR11XX_RADIO_LORA_SF7,
  .bw   = LR11XX_RADIO_LORA_BW_125,
  .cr   = LR11XX_RADIO_LORA_CR_4_5,
  .ldro = 0  // Low Data Rate Optimization (initialized in radio init)
};

// LoRa packet parameters
static const lr11xx_radio_pkt_params_lora_t lora_pkt_params = {
  .preamble_len_in_symb = 8,  // min required
  .header_type          = LR11XX_RADIO_LORA_PKT_EXPLICIT,
  .pld_len_in_bytes     = 0,	// allows variable length packet up to max
  .crc                  = LR11XX_RADIO_LORA_CRC_OFF,
  .iq                   = LR11XX_RADIO_LORA_IQ_STANDARD,
};

// Power Amplifier params
static const lr11xx_radio_pa_cfg_t lora_pa_params = {
	.pa_sel 				= LR11XX_RADIO_PA_SEL_HP,
	.pa_reg_supply 	= LR11XX_RADIO_PA_REG_SUPPLY_VBAT,
	.pa_duty_cycle	= 4,	// for 22dBm, need 4 duty cycle, 7 hp sel
	.pa_hp_sel			= 7,
};

void stormwater_lr1121_init(bool is_host) {
	if(is_host) {
		uint8_t tx_buffer[HOST_TX_BYTES];
		uint8_t rx_buffer[HOST_RX_BYTES];
	} else {
		uint8_t tx_buffer[HOST_RX_BYTES];
		uint8_t rx_buffer[HOST_TX_BYTES];
	}

	lr11xx_radio_set_pkt_type(&lr1121, LR11XX_RADIO_PKT_TYPE_LORA);
	lr11xx_radio_set_lora_mod_params(&lr1121, &lora_mod_params);
	lr11xx_radio_set_lora_pkt_params(&lr1121, &lora_pkt_params);
	lr11xx_radio_set_pa_cfg(&lr1121, &lora_pa_params);
	lr11xx_radio_set_tx_params(&lr1121, 22, LR11XX_RADIO_RAMP_48_US);


}

	


