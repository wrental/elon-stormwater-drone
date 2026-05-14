#include "driver/gpio.h"
#include "esp_timer.h"
#include "freertos/idf_additions.h"
#include "hal/gpio_types.h"
#include "portmacro.h"
#include "stormwater_config.h"
#include "stormwater_drone_sensors.h"
#include "stormwater_drone_spool.h"
#include "stormwater_lr1121.h"

#include <stdint.h>
#include <stdio.h>
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
  int last_spool_time = 0;
  int spool_timer = 5 * 1000 * 1000; // 10 sec
  bool spool_out = false;

  for (;;) {
    vTaskDelay(1 / portTICK_PERIOD_MS);
    // check for interrupt boolean
    if (stormwater_lr1121_interrupt()) {
      printf("interrupt\n");
      received_packet = stormwater_lr1121_interrupt_response();

      // handle received data
      if (received_packet) {
        printf("received\n");
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

    // spool logic (time)
    if (esp_timer_get_time() - last_spool_time > spool_timer) {
      if (rx_data.spool == 1 && !spool_out) {
        stormwater_drone_spool_out();
        spool_out = true;
      } else if (rx_data.spool == 0 && spool_out) {
        stormwater_drone_spool_in();
        spool_out = false;
      } else {
        stormwater_drone_spool_stop();
      }
      last_spool_time = esp_timer_get_time();
    }
  }
}

void app_main(void) {
  // literally just start the above loop
  // i should use more interrupt handlers :)
  xTaskCreatePinnedToCore(drone_main, "drone_main", 4096, NULL, 1, NULL, 1);
}
