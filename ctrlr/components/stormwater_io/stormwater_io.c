#include "stormwater_io.h"
#include "hal/gpio_types.h"
#include "stormwater_config.h"

#include "driver/gpio.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>

stormwater_io_t stormwater_io;
#define DEBOUNCE_DELAY_US 10 * 1000 // 10 ms
uint64_t last_isr_time = 0;

static void IRAM_ATTR button_pump_isr(void *arg) {
  if (esp_timer_get_time() - last_isr_time > DEBOUNCE_DELAY_US) {
    if (stormwater_io.pump == 1) {
      stormwater_io.pump = 0;
    } else {
      stormwater_io.pump = 1;
    }
    last_isr_time = esp_timer_get_time();
  }
}

static void IRAM_ATTR button_spool_isr(void *arg) {
  if (esp_timer_get_time() - last_isr_time > DEBOUNCE_DELAY_US) {
    if (stormwater_io.spool == 1) {
      stormwater_io.spool = 0;
    } else {
      stormwater_io.spool = 1;
    }
    last_isr_time = esp_timer_get_time();
  }
}

static void IRAM_ATTR button_data_isr(void *arg) {
  if (esp_timer_get_time() - last_isr_time > DEBOUNCE_DELAY_US) {
    if (stormwater_io.data == 1) {
      stormwater_io.data = 0;
    } else {
      stormwater_io.data = 1;
    }
    last_isr_time = esp_timer_get_time();
  }
}

void set_button_input(uint8_t button) {
  gpio_set_pull_mode(button, GPIO_PULLDOWN_ONLY);
  gpio_set_direction(button, GPIO_MODE_INPUT);
  gpio_intr_enable(button);
  gpio_set_intr_type(button, GPIO_INTR_POSEDGE);
}

void stormwater_io_init(void) {
  stormwater_io.spool = 0;
  stormwater_io.pump = 0;
  stormwater_io.data = 0;

  if (IS_HOST) {
    gpio_install_isr_service(0);

    set_button_input(BTN_SPOOL);
    gpio_isr_handler_add(BTN_SPOOL, button_spool_isr, NULL);
    set_button_input(BTN_PUMP);
    gpio_isr_handler_add(BTN_PUMP, button_pump_isr, NULL);
    set_button_input(BTN_DATA);
    gpio_isr_handler_add(BTN_DATA, button_data_isr, NULL);
  }
}
