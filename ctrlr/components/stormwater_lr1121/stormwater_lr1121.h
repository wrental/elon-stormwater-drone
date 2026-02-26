/*	name: stormwater_lr1121.h
 *	date: 02-24-2026
 *	auth: wrental
 *	desc: contains configuration information for stormwater implementation
 *				of Waveshare Core1121-XF module
 */

/*!
 * @file stormwater_lr1121.h
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

#include <stdbool.h>
#include <stdint.h>
#include "driver/spi_master.h"
#include "driver/gpio.h"

// LoRa packet setup:
/* droneTx >> ctrlrRx:
 * bytes 0-3: temp
 * bytes 4-7: D_O2
 * bytes 8-11: pH
 *
 * ctrlrTx >> droneRx:
 * byte 0: spool position
 * byte 1: pump on/off
 */
#define HOST_TX_BYTES 12
#define HOST_RX_BYTES 2

typedef struct lr1121_s {
	uint8_t cs;
	uint8_t reset;
	uint8_t busy;
	uint8_t irq;
    spi_device_handle_t spi;
} lr1121_t;

extern uint8_t rx_buffer[];
extern uint8_t rx_buffer_length;
extern uint8_t tx_buffer[];
extern uint8_t tx_buffer_length;
extern uint8_t rssi;
extern lr1121_t lr1121;

void stormwater_lr1121_init(void);

bool stormwater_lr1121_interrupt(void);

void stormwater_lr1121_interrupt_response(void);

// helper functions for lr11xx_hal.c
void lora_spi_write_bytes(const void* context,const uint8_t *write,const uint16_t write_length);
void lora_spi_read_bytes(const void* context, uint8_t *read,const uint16_t read_length);

#endif
