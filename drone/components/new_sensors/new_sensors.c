#include "new_sensors.h"
#include "esp_adc/adc_oneshot.h"

static adc_oneshot_unit_handle_t adc_handle = NULL;

static float temp_c = 0.0f;

static const uint16_t DO_Table[41] = {
    14460, 14220, 13820, 13440, 13090, 12740, 12420, 12110, 11810, 11530,
    11260, 11010, 10770, 10530, 10300, 10080, 9860, 9660, 9460, 9270,
    9080, 8900, 8730, 8570, 8410, 8250, 8110, 7960, 7820, 7690,
    7560, 7430, 7300, 7180, 7070, 6950, 6840, 6730, 6630, 6530, 6410
};

void new_sensors_init(void) {
  // Initialize and configure ADC channels for pH and DO sensors
  adc_oneshot_unit_init_cfg_t init_config = {
    .unit_id = ADC_UNIT_1,
  };
  adc_oneshot_new_unit(&init_config, &adc_handle);

  adc_oneshot_chan_cfg_t channel_config = {
    .atten = ADC_ATTEN_DB_12,
    .bitwidth = ADC_BITWIDTH_12,
  };

  adc_oneshot_config_channel(adc_handle, PH_ADC_CHANNEL, &channel_config);
  adc_oneshot_config_channel(adc_handle, DO_ADC_CHANNEL, &channel_config);
}

float new_get_temp(void) {
  // Returns the temperature from its port in DEG Celsius

  uint8_t data[12];

  onewire_reset(TEMP_GPIO);
  onewire_skip_rom(TEMP_GPIO);
  onewire_write(TEMP_GPIO, 0x44); // start conversion, with parasite power on at the end
  // vTaskDelay(pdMS_TO_TICKS(750)); // conversion wait
  onewire_reset(TEMP_GPIO);

  onewire_skip_rom(TEMP_GPIO);
  onewire_write(TEMP_GPIO, 0xBE); // Read Scratchpad

  for (int i = 0; i < 9; i++) { // we need 9 bytes
    data[i] = (uint8_t)onewire_read(TEMP_GPIO);
  }

  uint8_t MSB = data[1];
  uint8_t LSB = data[0];

  float tempRead = ((MSB << 8) | LSB); //using two's compliment
  float TemperatureSum = tempRead / 16.f;
  temp_c = TemperatureSum;
  
  return temp_c;

}

float new_get_ph(void) {
  int raw = 0;
  float ph = 0;
  adc_oneshot_read(adc_handle, PH_ADC_CHANNEL, &raw);
  ph = PH_GAIN * raw + PH_OFFSET;
  return ph;
}

float new_get_d_o2(void) {
  uint16_t V_saturation = (int16_t)((float)temp_c - CAL2_T) * (CAL1_V - CAL2_V) / (CAL1_T - CAL2_T) + CAL2_V;

  int d_o2_voltage_raw = 0;
  adc_oneshot_read(adc_handle, DO_ADC_CHANNEL, &d_o2_voltage_raw);

  uint16_t d_o2_voltage_calc = (uint32_t)VREF_MV * d_o2_voltage_raw / ADC_RES;

  float d_o2 = (float)(d_o2_voltage_calc * DO_Table[(int)temp_c]) / V_saturation;

  return d_o2;
}
