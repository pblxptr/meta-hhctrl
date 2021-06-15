#include "sensor.h"

void sensor_init(Sensor* sensor, struct gpio_desc* gpio_desc)
{
  sensor->gpio_desc = gpio_desc;
	sensor->gpio_id = desc_to_gpio(sensor->gpio_desc);
	sensor->irq = gpio_to_irq(sensor->gpio_id);

	gpiod_direction_input(sensor->gpio_desc);
	gpiod_export(sensor->gpio_desc, false);
}

void sensor_deinit(Sensor* sensor)
{
  gpiod_unexport(sensor->gpio_desc);
}

int sensor_irq(Sensor* sensor)
{
  return sensor->irq;
}

Sensor_state sensor_get_val(Sensor* sensor)
{
  if (gpiod_get_value(sensor->gpio_desc)) {
    return SENSOR_STATE_ACTIVE;
  }
  
  return SENSOR_STATE_NOT_ACTIVE;
}