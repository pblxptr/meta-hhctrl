extern "C" {
  #include <linux/gpio.h>
}

#include <mock_api.hpp>
#include <gmock_gpio.hpp>

void gpiod_set_value(struct gpio_desc* gpio, int val)
{
  if (!MockApi<GMockGpio>::has_mock()) {
    return;
  }

  MockApi<GMockGpio>::get_mock()->gpiod_set_value_impl(gpio, val);
}

int gpiod_get_value(struct gpio_desc* gpio)
{
  if (!MockApi<GMockGpio>::has_mock()) {
    return 0;
  }

  return MockApi<GMockGpio>::get_mock()->gpiod_get_value_impl(gpio);
}

int desc_to_gpio(struct gpio_desc* gpio)
{
  if (!MockApi<GMockGpio>::has_mock()) {
    return 0;
  }

  return MockApi<GMockGpio>::get_mock()->desc_to_gpio_impl(gpio);
}

int gpio_to_irq(int gpio)
{
  if (!MockApi<GMockGpio>::has_mock()) {
    return 0;
  }

  return MockApi<GMockGpio>::get_mock()->gpio_to_irq_impl(gpio);
}

void gpiod_direction_input(struct gpio_desc* gpio)
{
  if (!MockApi<GMockGpio>::has_mock()) {
    return;
  }

  MockApi<GMockGpio>::get_mock()->gpiod_direction_input_impl(gpio);
}

int gpiod_direction_output(struct gpio_desc *gpio, int value)
{
  if (!MockApi<GMockGpio>::has_mock()) {
    return 0;
  }

  return MockApi<GMockGpio>::get_mock()->gpiod_direction_output_impl(gpio, value);
}

void gpiod_export(struct gpio_desc* gpio, int may_change)
{
  if (!MockApi<GMockGpio>::has_mock()) {
    return;
  }

  MockApi<GMockGpio>::get_mock()->gpiod_export_impl(gpio, may_change);
}
