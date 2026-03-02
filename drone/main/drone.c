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
#if 0
        servo_config_t servo_cfg = {
        .max_angle = 180,
        .min_width_us = 500,
        .max_width_us = 2500,
        .freq = 50,
        .timer_number = LEDC_TIMER_0,
        .channels = {
            .servo_pin = {
                OUT_SRV,
            },
            .ch = {
                LEDC_CHANNEL_0,
            },
        },
        .channel_number = 1,
    };

    // Initialize the servo
    // iot_servo_init(LEDC_LOW_SPEED_MODE, &servo_cfg);
    uint16_t start_angle = 90;
    // iot_servo_write_angle(LEDC_LOW_SPEED_MODE, 0, start_angle);
#endif


    uint64_t start = 0;
    uint64_t end = 0;
    bool spool_busy = false;

    for(;;) {

        vTaskDelay(1 / portTICK_PERIOD_MS);

        if(stormwater_lr1121_interrupt()) {
            stormwater_lr1121_interrupt_response();

        tx_data.temp = get_temp();
        printf("%.2f\n", tx_data.temp);
        // tx_data.d_o2;
        printf("%.2f\n", tx_data.d_o2);
        // tx_data.ph++;
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

        gpio_set_level(OUT_PUMP, rx_data.pump);
#if 0
        if(rx_data.spool && !spool_busy) {
            start = esp_timer_get_time();
            end = (1 * 1000 * 1000) - start;
            spool_busy = true;
            uint16_t angle = 180;
            // iot_servo_write_angle(LEDC_LOW_SPEED_MODE, 0, angle);

        }
        else if(!rx_data.spool && !spool_busy) {
            start = esp_timer_get_time();
            end = (1 * 1000 * 1000) - start;
            spool_busy = true;
            uint16_t angle = 0;
            // iot_servo_write_angle(LEDC_LOW_SPEED_MODE, 0, angle);
        }

        if(spool_busy && (esp_timer_get_time() > end)) {
            uint16_t angle = 90;
            // iot_servo_write_angle(LEDC_LOW_SPEED_MODE, 0, angle);
            spool_busy = false;
        }
#endif
    }
}

void app_main(void) {
    xTaskCreate(drone_main, "drone_main", 4096, NULL, 1, NULL);
}
