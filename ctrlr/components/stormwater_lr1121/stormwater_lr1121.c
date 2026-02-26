/*	name: stormwater_lr1121.c
 *	date: 02-24-2026
 *	auth: wrental
 *	desc: contains functional implementation of Waveshare Core1121-XF module
 */

/*!
 * @file stormwater_lr1121.c
 *
 * @brief contains implementation of Waveshare Core1121-XF module
 *
 * @copyright
 * Stormwater Drone Controller for Elon University SPEED 2025-2026
 * Copyright (C) 2026 wrental
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTIBILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#include "stormwater_lr1121.h"
#include "esp_err.h"
#include "lr11xx_hal.h"
#include "stormwater_config.h"

#include <stdio.h>
#include <string.h>

#include "lr11xx_radio.h"
#include "lr11xx_radio_types.h"
#include "lr11xx_system_types.h"
#include "lr11xx_regmem.h"
#include "lr11xx_system.h"

// HOST VS CLIENT CHANGES
#if IS_HOST
uint8_t tx_buffer[HOST_TX_BYTES];
uint8_t tx_buffer_length = HOST_TX_BYTES;
uint8_t rx_buffer[HOST_RX_BYTES];
uint8_t rx_buffer_length = HOST_RX_BYTES;
#define TX_TIMEOUT 100 // ms
#define RX_TIMEOUT 500 // ms
#else
uint8_t tx_buffer[HOST_RX_BYTES];
uint8_t tx_buffer_length = HOST_RX_BYTES;
uint8_t rx_buffer[HOST_TX_BYTES];
uint8_t rx_buffer_length = HOST_TX_BYTES;
#define TX_TIMEOUT 100 // ms
#define RX_TIMEOUT 3000 // ms 
#endif

#define IRQ_MASK ( LR11XX_SYSTEM_IRQ_TX_DONE | LR11XX_SYSTEM_IRQ_RX_DONE | LR11XX_SYSTEM_IRQ_TIMEOUT )

uint8_t rssi;



// LoRa modulation parameters
static const lr11xx_radio_mod_params_lora_t lora_mod_params = {
  .sf   = LR11XX_RADIO_LORA_SF7,
  .bw   = LR11XX_RADIO_LORA_BW_125,
  .cr   = LR11XX_RADIO_LORA_CR_4_5,
  .ldro = 0  // Low Data Rate Optimization (initialized in radio init)
};

// LoRa packet parameters
static const lr11xx_radio_pkt_params_lora_t lora_pkt_params = {
  .preamble_len_in_symb = 8,  // min required
  .header_type          = LR11XX_RADIO_LORA_PKT_EXPLICIT,
  .pld_len_in_bytes     = 0,	// allows variable length packet up to max
  .crc                  = LR11XX_RADIO_LORA_CRC_OFF,
  .iq                   = LR11XX_RADIO_LORA_IQ_STANDARD,
};

// Power Amplifier params
static const lr11xx_radio_pa_cfg_t lora_pa_params = {
	.pa_sel 		= LR11XX_RADIO_PA_SEL_HP,
	.pa_reg_supply 	= LR11XX_RADIO_PA_REG_SUPPLY_VBAT,
	.pa_duty_cycle	= 4,	// for 22dBm, need 4 duty cycle, 7 hp sel
	.pa_hp_sel		= 7,
};

// ESP SPI config
static spi_device_handle_t stormwater_lr1121_spi_handle = NULL;

static spi_bus_config_t stormwater_lr1121_spi_cfg = {
	.sclk_io_num 		= LR1121_CLK,
	.mosi_io_num		= LR1121_MOSI,
	.miso_io_num		= LR1121_MISO,
	.quadwp_io_num		= -1,
	.quadhd_io_num		= -1,
	.max_transfer_sz	= 64,
};

static spi_device_interface_config_t stormwater_lr1121_spi_dev_int_cfg = {
	.clock_speed_hz 	= LR1121_SPI_CLK_HZ,
	.mode 				= 0,
	.spics_io_num		= -1,
	.queue_size			= 1,
};

// lr1121 context struct

lr1121_t lr1121 = {
  LR1121_CS,
  LR1121_RESET,
  LR1121_BUSY,
  LR1121_INT,
  NULL,
};
  
/*
 * ****************************************************************************
 *
 * 		COPIED/MODIFIED CODE FROM ESP-IDF COMPONENT waveshare/esp_lora_1121
 *
 * ****************************************************************************
 */
void lora_init_io(const void *context)
{
    //Set the output pin
    gpio_config_t io_conf = {};
    io_conf.intr_type = GPIO_INTR_DISABLE; // Disable interrupts for this pin
    io_conf.pin_bit_mask = 1ULL << (lr1121.cs ) | \
                           1ULL << (lr1121.reset);    // Select the GPIO pin using a bitmask
    io_conf.mode = GPIO_MODE_INPUT_OUTPUT;          // Set pin as input
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE; // Enable internal pull-up resistor
    gpio_config(&io_conf); // Apply the configuration

    //Set the input pin
    io_conf.pin_bit_mask = 1ULL << (lr1121.busy) | \
                           1ULL << (lr1121.irq);    // Select the GPIO pin using a bitmask
    io_conf.mode = GPIO_MODE_INPUT;          // Set pin as input
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE; // Enable internal pull-up resistor
    gpio_config(&io_conf); // Apply the configuration

    gpio_set_level(lr1121.cs, 1); // Set the GPIO pin level
    gpio_set_level(lr1121.reset, 1); // Set the GPIO pin level
}

void lora_init_irq(const void *context, gpio_isr_t handler)
{
    // Zero-initialize the GPIO configuration structure
    gpio_config_t io_conf = {};
    io_conf.intr_type = GPIO_INTR_POSEDGE;        // Trigger on negative edge (falling edge)
    io_conf.mode = GPIO_MODE_INPUT;               // Set pin as input mode
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE; // Disable pull-down
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;      // Enable pull-up resistor
    io_conf.pin_bit_mask = 1ULL << (lr1121.irq);           // Select the GPIO pin using a bitmask

    gpio_config(&io_conf); // Apply the configuration

    // Install the GPIO interrupt service if not already installed
    gpio_install_isr_service(0); // Pass 0 for default ISR flags

    // Register the interrupt handler for the specified pin
    gpio_isr_handler_add(((lr1121_t *)context)->irq, handler, (void *)((lr1121_t *)context)->irq);
}

static bool lora_irq_flag = false;

static void IRAM_ATTR isr(void* arg) {
	lora_irq_flag = true;
}

void lora_spi_write_bytes(const void* context,const uint8_t *write,const uint16_t write_length)
{
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));
    t.length = write_length * 8;       // Length is in bits
    t.tx_buffer = write;

    spi_device_transmit(lr1121.spi, &t);
}

void lora_spi_read_bytes(const void* context, uint8_t *read,const uint16_t read_length)
{
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));
    t.length = read_length * 8;       // Length is in bits
    t.rx_buffer = read;

    spi_device_transmit(lr1121.spi, &t);
}

// ******** END COPIED CODE *****************************************************************


// init spi bus, device; lora module/params; interrupt routine
void stormwater_lr1121_init(void) {

    // Waveshare esp_lora_1121 copied io init
    lora_init_io(&lr1121);

	// SPI Bus and Device Init
    lr1121.spi = stormwater_lr1121_spi_handle;
	spi_bus_initialize(LR1121_SPI_HOST, &stormwater_lr1121_spi_cfg, SPI_DMA_CH_AUTO);
	spi_bus_add_device(LR1121_SPI_HOST, &stormwater_lr1121_spi_dev_int_cfg, &lr1121.spi);

	// LoRa Module Init

    // LoRa System
    lr11xx_system_reset(&lr1121);
    lr11xx_hal_wakeup(&lr1121);
    lr11xx_system_enable_spi_crc(&lr1121, false);

    lr11xx_system_set_standby(&lr1121, LR11XX_SYSTEM_STANDBY_CFG_XOSC);
    lr11xx_system_calibrate_image(&lr1121, 0xE1, 0xE9);
    lr11xx_system_set_tcxo_mode(&lr1121, LR11XX_SYSTEM_TCXO_CTRL_3_0V, 300 );

    lr11xx_system_clear_errors(&lr1121);
    lr11xx_system_calibrate(&lr1121, 0x3F);

    lr11xx_system_clear_errors(&lr1121);
    lr11xx_system_clear_irq_status(&lr1121, LR11XX_SYSTEM_IRQ_ALL_MASK);

    lr11xx_system_set_dio_irq_params(&lr1121, LR11XX_SYSTEM_IRQ_ALL_MASK, 0);

    // LoRa Radio Init
	lr11xx_radio_set_pkt_type(&lr1121, LR11XX_RADIO_PKT_TYPE_LORA);
    lr11xx_radio_set_rf_freq(&lr1121, 915 * 1000 * 1000); // 915MHz
	lr11xx_radio_set_lora_mod_params(&lr1121, &lora_mod_params);
	lr11xx_radio_set_lora_pkt_params(&lr1121, &lora_pkt_params);
    lr11xx_radio_set_lora_sync_word(&lr1121, 0x12); // 0x12 = private network
	lr11xx_radio_set_pa_cfg(&lr1121, &lora_pa_params);
	lr11xx_radio_set_tx_params(&lr1121, 22, LR11XX_RADIO_RAMP_48_US);

    // waveshare copied start ISR
    lora_init_irq(&lr1121, isr);

    // Set to RX - if host, will timeout and re-send packet; else will remain in RX
    lr11xx_radio_set_rx(&lr1121, RX_TIMEOUT);
    
    uint16_t errors;
    lr11xx_system_get_errors(&lr1121, &errors);
    printf("errors: %i\n", errors);

}

//! check interrupt flag
bool stormwater_lr1121_interrupt(void) {
  return lora_irq_flag;
}

static void on_tx_done(void) {
  lr11xx_radio_set_rx(&lr1121, RX_TIMEOUT);
}

static void on_rx_done(void) {
  lr11xx_radio_pkt_status_lora_t pkt_status;
  lr11xx_radio_get_lora_pkt_status(&lr1121, &pkt_status);
  rssi = pkt_status.rssi_pkt_in_dbm;
  
  lr11xx_radio_rx_buffer_status_t buffer_status;
  lr11xx_radio_get_rx_buffer_status(&lr1121, &buffer_status);

  lr11xx_regmem_read_buffer8(&lr1121, rx_buffer, buffer_status.buffer_start_pointer, buffer_status.pld_len_in_bytes);
  
}

static void on_rx_timeout(void) {
  printf("Connection error: rx timeout.\n");
  if(IS_HOST) {
    lr11xx_radio_set_tx(&lr1121, TX_TIMEOUT);
  } else {
    lr11xx_radio_set_rx(&lr1121, RX_TIMEOUT);
  }
}

void stormwater_lr1121_interrupt_response(void) {
  lora_irq_flag = false;
  lr11xx_system_irq_mask_t irq_regs;
  lr11xx_system_get_and_clear_irq_status(&lr1121, &irq_regs);

  irq_regs &= IRQ_MASK;
  printf("filtered irq: %li\n", irq_regs);

  if((irq_regs & LR11XX_SYSTEM_IRQ_TX_DONE) == LR11XX_SYSTEM_IRQ_TX_DONE) {
      on_tx_done();
  }
  else if((irq_regs & LR11XX_SYSTEM_IRQ_RX_DONE) == LR11XX_SYSTEM_IRQ_RX_DONE) {
      on_rx_done();
  }
  else if((irq_regs & LR11XX_SYSTEM_IRQ_TIMEOUT) == LR11XX_SYSTEM_IRQ_TIMEOUT) {
      on_rx_timeout();
  }
  else {
    uint16_t errors;
    lr11xx_system_get_errors(&lr1121, &errors);
    printf("errors: %i\n", errors);
  }
}

