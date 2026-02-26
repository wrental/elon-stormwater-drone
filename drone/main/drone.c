#include "stormwater_config.h"
#include "stormwater_lr1121.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "freertos/task.h"

void drone_main(void *pvParameters) {

    stormwater_lr1121_init();

    for(;;) {

        vTaskDelay(1 / portTICK_PERIOD_MS);

        if(stormwater_lr1121_interrupt()) {
            stormwater_lr1121_interrupt_response();
        }

        tx_data.temp++;
        tx_data.d_o2++;
        tx_data.ph++;

        memcpy(&rx_data, rx_buffer, rx_buffer_length);
        memcpy(tx_buffer, &tx_data, tx_buffer_length);

    }
}

void app_main(void) {
    xTaskCreate(drone_main, "drone_main", 4096, NULL, 1, NULL);
}
