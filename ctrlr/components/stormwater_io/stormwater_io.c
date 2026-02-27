#include "stormwater_io.h"
#include "stormwater_config.h"

#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"

stormwater_io_t stormwater_io;
#define DEBOUNCE_DELAY_US 200000ULL
static uint64_t last_isr_time = 0;


static void IRAM_ATTR button_isr(void* arg) {
    uint64_t now = esp_timer_get_time();
    if (now - last_isr_time > DEBOUNCE_DELAY_US) {

    if(gpio_get_level(BTN_SPOOL) == 1) {
        if(stormwater_io.spool == 0) {
            stormwater_io.spool = 1;
            gpio_set_level(LED_SPOOL, 1);
        } else {
            stormwater_io.spool = 0;
            gpio_set_level(LED_SPOOL, 0);
        }
    }

    if(gpio_get_level(BTN_PUMP) == 1) {
        if(stormwater_io.pump == 0) {
            stormwater_io.pump = 1;
            gpio_set_level(LED_PUMP, 1);
        } else {
            stormwater_io.pump = 0;
            gpio_set_level(LED_PUMP, 0);
        }
    }

    if(gpio_get_level(BTN_DATA) == 1) {
        if(stormwater_io.data == 0) {
            stormwater_io.data = 1;
            gpio_set_level(LED_DATA, 1);
        } else {
            stormwater_io.data = 0;
            gpio_set_level(LED_DATA, 0);
        }
    }
    }
}

void set_button_input(uint8_t button) {
    gpio_set_pull_mode(button, GPIO_PULLDOWN_ONLY);
    gpio_set_direction(button, GPIO_MODE_INPUT);
    gpio_intr_enable(button);
    gpio_set_intr_type(button, GPIO_INTR_POSEDGE);
    gpio_isr_handler_add(button, button_isr, NULL);
}



void stormwater_io_init(void) {
    stormwater_io.spool = 0;
    stormwater_io.pump = 0;
    stormwater_io.spool = 0;

    if(IS_HOST) {
        gpio_install_isr_service(0);

        set_button_input(BTN_SPOOL);
        gpio_set_direction(LED_SPOOL, GPIO_MODE_OUTPUT);
        set_button_input(BTN_PUMP);
        gpio_set_direction(LED_PUMP, GPIO_MODE_OUTPUT);
        set_button_input(BTN_DATA);
        gpio_set_direction(LED_DATA, GPIO_MODE_OUTPUT);
    }


}



