/*	name: ctrlr.h
 *	date: 02-24-2026
 *	auth: wrental
 *	desc: containts configuration information for controller electronics
 */

/*!
 * @file ctrlr.h
 *
 * @brief contains configuration information for stormwater drone controller electronics
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

#ifndef CTRLR_H
#define CTRLR_H

#include "esp_lora_1121.h"

/*!
 * @brief controller ESP32 pinout definition
 */
#define BTN_SPOOL			(GPIO_NUM_1)
#define BTN_PUMP			(GPIO_NUM_2)
#define BTN_DATA			(GPIO_NUM_3)
#define LED_SPOOL			(GPIO_NUM_4)
#define LED_PUMP			(GPIO_NUM_5)
#define LED_DATA			(GPIO_NUM_6)
#define LR1121_INT		(GPIO_NUM_12)
#define LR1121_BUSY 	(GPIO_NUM_13)
#define LR1121_RESET	(GPIO_NUM_14)
#define LR1121_MISO		(GPIO_NUM_15)
#define LR1121_MOSI		(GPIO_NUM_16)
#define LR1121_CLK		(GPIO_NUM_17)
#define LR1121_CS			(GPIO_NUM_18)

/*!
 * @brief controller ESP32 SPI configuration for LR1121
 */
#define LR1121_SPI_HOST		(SPI2_HOST)
#define LR1121_SPI_CLK_HZ	8 * 1000 * 1000 // 8MHz

/*!
 * @brief LR1121 controller-specific settings
 */
#define IS_HOST 								true
#define RX_CONTINUOUS						0xFFFFFF
#define RX_TIMEOUT_VALUE				20	// ms
#define TX_TIMEOUT_VALUE				10	// ms
#define TX_RX_TRANSITION_DELAY	10	// ms
#define ITERATION_DELAY					20 	// ms

/*!
 * @brief global variable for LoRa interrupt status
 */
extern bool lr1121_irq_flag;

/*!
 * @brief global variable for LoRa device context
 */
extern lr1121_t lr1121;

/*!
 * @brief define receive packet parameters and global variable
 */
typedef struct {
	float temp;
	float do2;
	float ph;
	bool spool; // cm
	bool pump;
} Receive_Packet;
extern Receive_Packet receive_packet;

/*!
 * @brief define send packet parameters and global variable
 */
typedef struct {
	bool spool;
	bool pump;
} Send_Packet;
extern Send_Packet send_packet;

#endif
