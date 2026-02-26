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
        vTaskDelay(1 / portTICK_PERIOD_MS);
        temp += 1;
        d_o2 += 1;
        ph += 1;

        memcpy(&tx_buffer[0], &temp, 4);
        memcpy(&tx_buffer[4], &d_o2, 4);
        memcpy(&tx_buffer[8], &ph, 4);

        for(int i = 0; i < tx_buffer_length; i++) {
            printf("0x%X, ", tx_buffer[i]);
        }
        printf("\n");

        if(stormwater_lr1121_interrupt()) {
            stormwater_lr1121_interrupt_response();
        }
    }
}

void app_main(void) {
    xTaskCreate(drone_main, "drone_main", 4096, NULL, 1, NULL);
}
