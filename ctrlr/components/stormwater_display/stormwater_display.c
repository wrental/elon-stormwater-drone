#include "hd44780.h"
#include "pcf8574.h"
#include <stdio.h>
#include <string.h>

static i2c_dev_t pcf8574;

static esp_err_t write_lcd_data(const hd44780_t *lcd, uint8_t data) {
  return pcf8574_port_write(&pcf8574, data);
}

hd44780_t lcd = {
    .write_cb =
        write_lcd_data, // use callback to send data to LCD by I2C GPIO expander
    .font = HD44780_FONT_5X8,
    .lines = 2,
    .pins = {.rs = 0, .e = 2, .d4 = 4, .d5 = 5, .d6 = 6, .d7 = 7, .bl = 3}};
