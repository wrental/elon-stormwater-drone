#include "driver/gpio.h"
#include "esp_timer.h"
#include "stormwater_lr1121.h"
#include "stormwater_sensors.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "freertos/task.h"
#include "iot_servo.h"

void drone_main(void *pvParameters) {

    stormwater_lr1121_init();

    for(;;) {

        // check for interrupt boolean
        // TODO: switch to interrupt handler
        if(stormwater_lr1121_interrupt()) {
            stormwater_lr1121_interrupt_response();
        }

        // TODO: add sensor data cp to tx buff

        // cpy rx data to rx buffer, tx data to tx buffer
        memcpy(&rx_data, rx_buffer, rx_buffer_length);
        memcpy(tx_buffer, &tx_data, tx_buffer_length);

        gpio_set_level(OUT_PUMP, rx_data.pump);
    }
}

void app_main(void) {
    // literally just start the above loop
    // i should use more interrupt handlers :)
    xTaskCreate(drone_main, "drone_main", 4096, NULL, 1, NULL);
}
