/*	name: stormwater_lr1121.h
 *	date: 02-24-2026
 *	auth: wrental
 *	desc: contains configuration information for stormwater implementation
 *				of Waveshare Core1121-XF module
 */

/*!
 * @file ctrlr.h
 *
 * @brief contains configuration information for stormwater implementation 
 * of Waveshare Core1121-XF module
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

#ifndef STORMWATER_LR1121_H
#define STORMWATER_LR1121_H

//! If controller, include controller configuration
#ifdef CTRL_H
#include "ctrlr.h"
#endif

//! If drone, include drone configuration
#ifdef DRONE_H
#include "drone.h"
#endif


/*!
 * @brief LoRa sync/setup parameters and definitions
 */
//! General Parameters:
#define PACKET_TYPE						LR1121XX_RADIO_PKY_TYPE_LORA
#define RF_FREQ_IN_HZ					915 * 1000 * 1000	// 915MHz
#define TX_OUTPUT_POWER_DBM		22	// -9~22
#define PA_RAMP_TIME					LR11XX_RADIO_RAMP_48_US
#define FALLBACK_MODE					LR11XX_RADIO_FALLBACK_STDBY_RC
#define ENABLE_RX_BOOST_MODE	true

//! Modulation Parameters:
#define LORA_SPREADING_FACTOR LR11XX_RADIO_LORA_SF7
#define LORA_BANDWIDTH LR11XX_RADIO_LORA_BW_125
#define LORA_CODING_RATE LR11XX_RADIO_LORA_CR_4_5

//! Packet Parameters:
#define LORA_PREAMBLE_LENGTH 8
#define LORA_PKT_LEN_MODE LR11XX_RADIO_LORA_PKT_EXPLICIT
#define LORA_IQ LR11XX_RADIO_LORA_IQ_STANDARD
#define LORA_CRC LR11XX_RADIO_LORA_CRC_OFF

#define LORA_SYNCWORD 0x12  // 0x12 Private Network, 0x34 Public Network

#endif
