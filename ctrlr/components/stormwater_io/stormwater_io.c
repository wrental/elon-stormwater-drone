#include "stormwater_io.h"
#include "hal/gpio_types.h"
#include "stormwater_config.h"

#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"

stormwater_io_t stormwater_io;
#define DEBOUNCE_DELAY_US 1000000ULL
static uint64_t last_isr_time = 0;


static void IRAM_ATTR button_isr(void* arg) {
    uint64_t now = esp_timer_get_time();
    if (now - last_isr_time > DEBOUNCE_DELAY_US) {
        stormwater_io.data = gpio_get_level(BTN_DATA);
        gpio_set_level(LED_DATA, gpio_get_level(BTN_DATA));
        stormwater_io.pump = gpio_get_level(BTN_PUMP);
        stormwater_io.spool = gpio_get_level(BTN_SPOOL);
    }
}

void set_button_input(uint8_t button) {
    gpio_set_pull_mode(button, GPIO_PULLDOWN_ONLY);
    gpio_set_direction(button, GPIO_MODE_INPUT);
    gpio_intr_enable(button);
    gpio_set_intr_type(button, GPIO_INTR_ANYEDGE);
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
        stormwater_io.data = gpio_get_level(BTN_DATA);
        stormwater_io.pump = gpio_get_level(BTN_PUMP);
        stormwater_io.spool = gpio_get_level(BTN_SPOOL);
    }
}



