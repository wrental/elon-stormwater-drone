/*	name: ctrlr.c
 *	date: 02-24-2026
 *	auth: wrental
 *	desc: contains main app method calls and logic for stormwater
 *				drone controller
 */

/*!
 * @file ctrlr.c
 *
 * @brief contains main app method calls and logic for stormwater
 * drone controller
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
#include "stormwater_io.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "freertos/task.h"

void ctrlr_main(void *pvParameters) {

    stormwater_lr1121_init();
    stormwater_io_init();
    bool received_packet;

    for(;;) {
        vTaskDelay(1 / portTICK_PERIOD_MS);

        tx_data.pump = stormwater_io.pump;
        tx_data.spool = stormwater_io.spool;

        if(stormwater_lr1121_interrupt()) {
            received_packet = stormwater_lr1121_interrupt_response();

            memcpy(&rx_data, rx_buffer, rx_buffer_length);
            memcpy(tx_buffer, &tx_data, tx_buffer_length);
            
            if(!stormwater_io.data && received_packet) {
                printf("RSSI: %i; TEMP: %.2f; D_O2: %.2f; PH: %.2f; SPOOL: %i; PUMP: %i\n", 
                rssi, rx_data.temp, rx_data.d_o2, rx_data.ph, tx_data.spool, tx_data.pump);
            }
            rssi = 0;

#if 0
        printf("rx_buffer: ");
        for(int i = 0; i < rx_buffer_length; i++) {
            printf("0x%X  ", rx_buffer[i]);
        }
        printf("\n");

        printf("tx_buffer: ");
        for(int i = 0; i < tx_buffer_length; i++) {
            printf("0x%X  ", tx_buffer[i]);
        }
        printf("\n");
#endif
        
        }
    }
}

void app_main(void) {
    xTaskCreate(ctrlr_main, "ctrlr_main", 4096, NULL, 1, NULL);
}
