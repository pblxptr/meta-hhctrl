#include "sensor.h"

#include <stdbool.h>
#include <linux/timer.h>
#include <linux/jiffies.h>
#include <linux/printk.h>
#include <stddef.h>

//TODO: Make dependencies null

/* Private function declarations */
static void sensor_handle_timer(struct timer_list* timer);

/* Public function implementations */
int sensor_init(sensor_t* sensor, struct gpio_desc* gpio, irq_handler_t irqhandler)
{
  sensor->gpio = gpio;
	sensor->gpio_id = desc_to_gpio(sensor->gpio);
	sensor->irq = gpio_to_irq(sensor->gpio_id);
	sensor->is_active = true;

	gpiod_direction_input(sensor->gpio);
	gpiod_export(sensor->gpio, false);

	if (request_irq(sensor->irq, irqhandler, IRQF_TRIGGER_FALLING,
			"hatch2sr", NULL)) //TODO: Should it be null or func ptr?
	{
    return -1;
	}

  timer_setup(&sensor->timer, sensor_handle_timer, 0);

  return 0;
}

void sensor_deinit(sensor_t* sensor)
{
	free_irq(sensor->irq, NULL);
	gpiod_unexport(sensor->gpio);
	gpiod_put(sensor->gpio);
  del_timer_sync(&sensor->timer);
}

int sensor_get_value(sensor_t* sensor)
{
  return gpiod_get_value(sensor->gpio);
}

void sensor_deactivate(sensor_t* sensor, int deactivation_time_ms)
{
  pr_info("%s\n", __FUNCTION__);

  mod_timer(&sensor->timer, jiffies + msecs_to_jiffies(deactivation_time_ms));

  sensor->is_active = false;
}

/* Private function implementations */
void sensor_handle_timer(struct timer_list* timer)
{
  sensor_t* sensor = NULL;

  pr_info("%s\n", __FUNCTION__);

  sensor = container_of_safe(timer, sensor_t, timer); //TODO: Add check
  sensor->is_active = true;
}