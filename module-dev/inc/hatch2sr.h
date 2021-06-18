#pragma once

#include <linux/gpio.h>
#include <linux/pwm.h>

#include <engine.h>
#include <sensor.h>
#include <relay.h>

typedef enum hatch_state {
  HATCH_STATE_OPEN,
  HATCH_STATE_CLOSED,
  HATCH_STATE_CHANGING_POSITION,
  HATCH_STATE_FAULTY
} hatch_state; 

typedef struct hatch2sr {
  struct engine engine;
  struct sensor openpos;
  struct sensor closedpos;
  struct relay relay;
} hatch2sr;

int hatch2sr_init(struct pwm_device* pwm, struct gpio_desc* openpos, struct  gpio_desc* closepos, struct gpio_desc* relay);
void hatch2sr_deinit();
hatch2sr* hatch2sr_get();
void hatch2sr_open();
void hatch2sr_close();

hatch_state hatch2sr_get_state();