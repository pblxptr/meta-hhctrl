#include "sensor.h"
#include <stdbool.h>

int sensor_init(sensor* sensor, struct gpio_desc* gpio, irq_handler_t irqhandler)
{
  sensor->gpio = gpio;
	sensor->gpio_id = desc_to_gpio(sensor->gpio);
	sensor->irq = gpio_to_irq(sensor->gpio_id);

	gpiod_direction_input(sensor->gpio);
	gpiod_export(sensor->gpio, false);

	if (request_irq(sensor->irq, irqhandler, IRQF_TRIGGER_FALLING, 
			"hatch2sr.", irqhandler))
	{	
    return -1;
	}

  return 0;
}

int sensor_get_value(sensor* sensor)
{
  return gpiod_get_value(sensor->gpio);
}