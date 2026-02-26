#include "stormwater_config.h"
#include "stormwater_lr1121.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "freertos/task.h"

void drone_main(void *pvParameters) {

    stormwater_lr1121_init();

    float temp = 0;
    float d_o2 = 0;
    float ph = 0;

    memset(rx_buffer, 0, rx_buffer_length);
    memset(tx_buffer, 0, tx_buffer_length);

    for(;;) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        temp++;
        d_o2++;
        ph++;

        if(stormwater_lr1121_interrupt()) {
            stormwater_lr1121_interrupt_response();
        }
    }
}

void app_main(void) {
    xTaskCreate(drone_main, "drone_main", 4096, NULL, 5, NULL);
}
