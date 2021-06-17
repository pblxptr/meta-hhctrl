#pragma once

#include <linux/gpio.h>
#include <linux/pwm.h>

typedef enum hatch_state {
  HATCH_STATE_OPEN,
  HATCH_STATE_CLOSED,
  HATCH_STATE_CHANGING_POSITION,
  HATCH_STATE_FAULTY
} hatch_state; 

int hatch2sr_init(struct pwm_device* pwm, struct gpio_desc* openpos, struct  gpio_desc* closepos, struct gpio_desc* relay);
void hatch2sr_deinit();
void hatch2sr_open();
void hatch2sr_close();
hatch_state hatch2sr_get_state();