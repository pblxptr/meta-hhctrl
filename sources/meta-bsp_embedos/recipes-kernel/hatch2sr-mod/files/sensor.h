#pragma once

#include <linux/gpio.h>
#include <linux/interrupt.h>

typedef struct sensor {
  struct gpio_desc* gpio;
  int gpio_id;
  int irq;
} sensor_t;

int sensor_init(sensor_t* sensor, struct gpio_desc* gpio, irq_handler_t irqhandler);
void sensor_deinit(sensor_t* sensor);
int sensor_get_value(sensor_t* sensor);