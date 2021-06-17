#pragma once

#include <linux/gpio.h>
#include <linux/interrupt.h>

typedef struct sensor {
  struct gpio_desc* gpio;
  int gpio_id;
  int irq;
} sensor;

int sensor_init(sensor* sensor, struct gpio_desc* gpio, irq_handler_t irqhandler);
int sensor_get_value(sensor* sensor);