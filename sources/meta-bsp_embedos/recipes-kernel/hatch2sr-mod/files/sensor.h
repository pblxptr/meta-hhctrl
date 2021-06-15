#pragma once

#include <linux/gpio.h>

typedef enum __Sensor_state__ {
  SENSOR_STATE_NOT_ACTIVE, 
  SENSOR_STATE_ACTIVE
} Sensor_state;

typedef struct __Sensor__ {
  struct gpio_desc* gpio_desc;
  int gpio_id;
  int irq;
  Sensor_state state;
} Sensor ;

void sensor_init(Sensor* sensor, struct gpio_desc*);
void sensor_deinit(Sensor* sensor);
int sensor_irq(Sensor* sensor);
Sensor_state sensor_get_val(Sensor* sensor);