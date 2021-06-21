#include "sensor.h"
#include <stdbool.h>
#include <stddef.h>

//TODO: Make dependencies null

int sensor_init(sensor* sensor, struct gpio_desc* gpio, irq_handler_t irqhandler)
{
  sensor->gpio = gpio;
	sensor->gpio_id = desc_to_gpio(sensor->gpio);
	sensor->irq = gpio_to_irq(sensor->gpio_id);

	gpiod_direction_input(sensor->gpio);
	gpiod_export(sensor->gpio, false);

	if (request_irq(sensor->irq, irqhandler, IRQF_TRIGGER_FALLING, 
			"hatch2sr.", NULL)) //TODO: Should it be null or func ptr?
	{	
    return -1;
	}

  return 0;
}

void sensor_deinit(sensor* sensor)
{
	free_irq(sensor->irq, NULL);
	gpiod_unexport(sensor->gpio);
}

int sensor_get_value(sensor* sensor)
{
  return gpiod_get_value(sensor->gpio);
}