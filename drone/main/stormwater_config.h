/* name: stormwater_config.h
 * date: 02-24-2026
 * auth: wrental
 * desc: containts configuration information for drone electronics
 */

/*!
 * @file stormwater_config.h
 *
 * @brief contains configuration information for stormwater drone
 *
 * @copyright
 * Stormwater Drone for Elon University SPEED 2025-2026
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

#ifndef STORMWATER_CONFIG_H
#define STORMWATER_CONFIG_H

#define IS_HOST false

/*!
 * @brief controller ESP32 pinout definition
 */
// 1 - NC
#define SENS_TEMP           (GPIO_NUM_2)
#define SENS_D_O2           (GPIO_NUM_3)
#define SENS_PH             (GPIO_NUM_4)
#define OUT_PUMP            (GPIO_NUM_5)
// 6 - NC

#define SRV_OUT             (GPIO_NUM_7)
#define SRV_IN              (GPIO_NUM_8)

#define ENC_DAT_B           (GPIO_NUM_9)
#define ENC_DAT_A           (GPIO_NUM_10)
#define ENC_DAT_X           (GPIO_NUM_11)

#define LR1121_INT		    (GPIO_NUM_12)
#define LR1121_BUSY 	    (GPIO_NUM_13)
#define LR1121_RESET	    (GPIO_NUM_14)
#define LR1121_MISO		    (GPIO_NUM_15)
#define LR1121_MOSI		    (GPIO_NUM_16)
#define LR1121_CLK		    (GPIO_NUM_17)
#define LR1121_CS			(GPIO_NUM_18)

/*!
 * @brief controller ESP32 SPI configuration for LR1121
 */
#define LR1121_SPI_HOST		(SPI2_HOST)
#define LR1121_SPI_CLK_HZ	8 * 1000 * 1000 // 8MHz

#endif
