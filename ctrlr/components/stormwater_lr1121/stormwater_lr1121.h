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

extern uint8_t rx_buffer[];
extern uint8_t tx_buffer[];

void stormwater_lr1121_init(bool is_host);

bool stormwater_lr1121_interrupt(void);

void stormwater_lr1121_interrupt_response(void);

uint8_t* stormwater_lr1121_read_rx_buffer(void);

void stormwater_lr1121_write_tx_buffer(uint8_t* tx_buffer[]);

void stormwater_lr1121_set_tx(void);

void stormwater_lr1121_set_rx(void);

#endif
