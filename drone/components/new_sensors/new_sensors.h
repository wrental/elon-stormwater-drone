#ifndef NEW_SENSORS_H
#define NEW_SENSORS_H

#include "stormwater_config.h"

#define DO_ADC_CHANNEL      ADC1_GPIO3_CHANNEL
#define PH_ADC_CHANNEL      ADC1_GPIO4_CHANNEL
#define TEMP_GPIO           SENS_TEMP

// Calibration constants for pH sensor
#define PH_OFFSET       -17.125f
#define PH_GAIN         12.5f

// ADC reference voltage and resolution
#define VREF_MV         5000
#define ADC_RES         4095

// DO2 calibration constants
#define CAL1_V          1601
#define CAL1_T          36.25f
#define CAL2_V          1132
#define CAL2_T          17.94f

void new_sensors_init(void);
float new_get_temp(void);
float new_get_d_o2(void);
float new_get_ph(void);

#endif
