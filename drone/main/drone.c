#include "driver/gpio.h"
#include "esp_timer.h"
#include "hal/gpio_types.h"
#include "stormwater_config.h"
#include "stormwater_lr1121.h"
#include "stormwater_drone_sensors.h"
#include "stormwater_drone_spool.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "freertos/task.h"

void drone_main(void *pvParameters) {

    stormwater_lr1121_init();
    bool received_packet = false;

    gpio_reset_pin(OUT_PUMP);
    gpio_set_direction(OUT_PUMP, GPIO_MODE_OUTPUT);
    gpio_set_pull_mode(OUT_PUMP, GPIO_PULLDOWN_ONLY);
    gpio_set_level(OUT_PUMP, 0);

    gpio_reset_pin(SENS_TEMP);
    gpio_set_direction(SENS_TEMP, GPIO_MODE_INPUT);
    gpio_set_pull_mode(SENS_TEMP, GPIO_PULLUP_ONLY);

    stormwater_drone_sensors_init();
    stormwater_drone_spool_init();


    for(;;) {
        // check for interrupt boolean
        // TODO: switch to interrupt handler
        if(stormwater_lr1121_interrupt()) {
            // handle interrupt
            received_packet = stormwater_lr1121_interrupt_response();
            
            // handle received data
            if(received_packet) {
                memcpy(&rx_data, rx_buffer, rx_buffer_length);

                // set pump, spool to appropriate values
                // send back current status after setting
                gpio_set_level(OUT_PUMP, rx_data.pump);
                tx_data.pump = rx_data.pump;
                // TODO: spool implementation
                tx_data.spool = rx_data.spool;
            }

        }

        // writing to tx buffer
        // TODO: add actual measurements from sensors
        memcpy(tx_buffer, &tx_data, tx_buffer_length);
        tx_data.temp = stormwater_drone_sensors_get_temp();
        tx_data.d_o2 = stormwater_drone_sensors_get_d_o2();
        tx_data.ph = stormwater_drone_sensors_get_ph();
        if(rx_data.spool == 1 && stormwater_drone_spool_get_pos_ticks() < SPOOL_OUT) {
            stormwater_drone_spool_in();
        }
        else if (rx_data.spool == 0 && stormwater_drone_spool_get_pos_ticks() > SPOOL_IN + 100 ) {
            stormwater_drone_spool_out();
        }
        else {
            stormwater_drone_spool_stop();
        }

    }
}

void app_main(void) {
    // literally just start the above loop
    // i should use more interrupt handlers :)
    xTaskCreate(drone_main, "drone_main", 4096, NULL, 1, NULL);
}
