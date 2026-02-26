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

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "freertos/task.h"

void ctrlr_main(void *pvParameters) {

    stormwater_lr1121_init();

    for(;;) {
        vTaskDelay(1 / portTICK_PERIOD_MS);

        // DEBUG PRINTING TEST
       #if 0
        printf("[DEBUG] tx_buffer: ");
        for(int i = 0; i < tx_buffer_length; i++) {
            printf("0x%X, ", tx_buffer[i]);
        }
        printf("\n");

        printf("[DEBUG] rx_buffer: ");
        for(int i = 0; i < rx_buffer_length; i++) {
            printf("0x%X, ", rx_buffer[i]);
        }
        printf("\n");
#endif

        if(stormwater_lr1121_interrupt()) {
            stormwater_lr1121_interrupt_response();
        }

        tx_data.pump = 0;
        tx_data.spool = 0;

        memcpy(&rx_data, rx_buffer, rx_buffer_length);
        memcpy(tx_buffer, &tx_data, tx_buffer_length);
        
        printf("RSSI: %i; TEMP: %.2f; D_O2: %.2f; PH: %.2f\n", 
                rssi, rx_data.temp, rx_data.d_o2, rx_data.ph);
    }
}



void app_main(void) {
    xTaskCreate(ctrlr_main, "ctrlr_main", 4096, NULL, 1, NULL);
}
