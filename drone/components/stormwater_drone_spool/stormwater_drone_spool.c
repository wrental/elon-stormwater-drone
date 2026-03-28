#include "stormwater_drone_spool.h"
#include "driver/mcpwm_cmpr.h"
#include "driver/mcpwm_types.h"
#include "hal/mcpwm_types.h"
#include "stormwater_config.h"


#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "hal/gpio_types.h"
#include "driver/mcpwm_prelude.h"
#include "xtensa/xtruntime.h"

#define SERVO_CLOCKWISE         2000 // microseconds (ticks)
#define SERVO_STOP              1500 // microseconds (ticks)
#define SERVO_COUNTER_CLOCKWISE 1000 // microseconds (ticks)


// MCPWM Controls

// set up timer for a default 50Hz PWM signal
mcpwm_timer_handle_t timer_0;
mcpwm_timer_config_t timer_0_config = {
    .group_id = 0,
    .clk_src = MCPWM_TIMER_CLK_SRC_DEFAULT, // default 80MHz
    .resolution_hz = 1 * 1000 * 1000, // 1 MHz
    .count_mode = MCPWM_TIMER_COUNT_MODE_UP,
    .period_ticks = 20000, // ticks per period - at 20k should create 50Hz PWM with 20000 microsecond duty cycle
    .intr_priority = 0,
    .flags.update_period_on_empty = 1,
};

// set up operator
mcpwm_oper_handle_t oper_0;
mcpwm_operator_config_t oper_0_config = {
    .group_id = 0,
    .intr_priority = 0,
};

// removed comparator component - (hopefully) not needed
// added it back - i needed it :(
mcpwm_cmpr_handle_t cmpr_0;
mcpwm_comparator_config_t cmpr_0_config = {
    .intr_priority = 0,
};

// set up PWM generator
mcpwm_gen_handle_t generator_0;
mcpwm_generator_config_t generator_0_config = {
    .gen_gpio_num = SRV_OUT,
};


static int pos_ticks = 0;


// --- PRIVATE METHODS ---
static void IRAM_ATTR enc_isr(void* arg) {
    if(gpio_get_level(ENC_DAT_A) == gpio_get_level(ENC_DAT_B)) {
        pos_ticks++;
    }
    else {
        pos_ticks--;
    }
}


// setup to read values from encoder
static void enc_init(void) {
    gpio_install_isr_service(0);
    gpio_reset_pin(ENC_DAT_A);
    gpio_set_direction(ENC_DAT_A, GPIO_MODE_INPUT);
    gpio_set_pull_mode(ENC_DAT_A, GPIO_PULLUP_ONLY);
    gpio_intr_enable(ENC_DAT_A);
    gpio_set_intr_type(ENC_DAT_A, GPIO_INTR_ANYEDGE);
    gpio_isr_handler_add(ENC_DAT_A, enc_isr, NULL);

    gpio_reset_pin(ENC_DAT_B);
    gpio_set_direction(ENC_DAT_B, GPIO_MODE_INPUT);
    gpio_set_pull_mode(ENC_DAT_B, GPIO_PULLUP_ONLY);
}



static void srv_init(void) {
    gpio_reset_pin(SRV_OUT);
    gpio_set_direction(SRV_OUT, GPIO_MODE_OUTPUT);
    gpio_set_pull_mode(SRV_OUT, GPIO_PULLDOWN_ONLY);
    gpio_set_level(SRV_OUT, 0);

    mcpwm_new_timer(&timer_0_config, &timer_0);
    mcpwm_new_operator(&oper_0_config, &oper_0);
    mcpwm_operator_connect_timer(oper_0, timer_0);
    mcpwm_new_generator(oper_0, &generator_0_config, &generator_0);

    mcpwm_generator_set_action_on_timer_event(generator_0, 
            MCPWM_GEN_TIMER_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, MCPWM_TIMER_EVENT_FULL, MCPWM_GEN_ACTION_LOW));
    mcpwm_generator_set_action_on_timer_event(generator_0, 
            MCPWM_GEN_TIMER_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, MCPWM_TIMER_EVENT_EMPTY, MCPWM_GEN_ACTION_HIGH));

    mcpwm_new_comparator(oper_0, &cmpr_0_config, &cmpr_0);
    mcpwm_generator_set_action_on_compare_event(generator_0,
            MCPWM_GEN_COMPARE_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, cmpr_0, MCPWM_GEN_ACTION_LOW));

    mcpwm_timer_enable(timer_0);
    mcpwm_timer_start_stop(timer_0, MCPWM_TIMER_START_NO_STOP);

}


// --- PUBLIC METHODS ---

void stormwater_drone_spool_init(void) {
    enc_init();
    srv_init();
}

int stormwater_drone_spool_get_pos_ticks(void) {
    return pos_ticks;
}

void stormwater_drone_spool_out(void) {
    mcpwm_comparator_set_compare_value(cmpr_0, SERVO_CLOCKWISE);
}

void stormwater_drone_spool_in(void) {
    mcpwm_comparator_set_compare_value(cmpr_0, SERVO_COUNTER_CLOCKWISE);
}

void stormwater_drone_spool_stop(void) {
    mcpwm_comparator_set_compare_value(cmpr_0, SERVO_STOP);
}

