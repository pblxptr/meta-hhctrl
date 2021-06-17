#include "relay.h"
#include <linux/gpio.h>
#include <stdbool.h>

int relay_init(relay* relay, struct gpio_desc* gpio)
{
  relay->gpio = gpio;
	relay->gpio_id = desc_to_gpio(relay->gpio);

	gpiod_direction_output(relay->gpio, 0);
	gpiod_export(relay->gpio, false);

  return 0;
}

void relay_set_to_open(relay* relay)
{
  gpiod_set_value(relay->gpio, 1);
}

void relay_set_to_close(relay* relay)
{
  gpiod_set_value(relay->gpio, 0);
}
