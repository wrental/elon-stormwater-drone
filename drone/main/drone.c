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

        tx_data.temp++;
        printf("%.2f\n", tx_data.temp);
        tx_data.d_o2++;
        printf("%.2f\n", tx_data.d_o2);
        tx_data.ph++;
        printf("%.2f\n", tx_data.ph);

        memcpy(&rx_data, rx_buffer, rx_buffer_length);
        printf("rx_buffer: ");
        for(int i = 0; i < rx_buffer_length; i++) {
            printf("0x%X  ", rx_buffer[i]);
        }
        printf("\n");

        memcpy(tx_buffer, &tx_data, tx_buffer_length);
        printf("tx_buffer: ");
        for(int i = 0; i < tx_buffer_length; i++) {
            printf("0x%X  ", tx_buffer[i]);
        }
        printf("\n");

       }


    }
}

void app_main(void) {
    xTaskCreate(drone_main, "drone_main", 4096, NULL, 1, NULL);
}
