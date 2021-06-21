#pragma once

#include <gmock/gmock.h>

extern "C" {
  #include <linux/gpio.h>
}

struct GMockGpio 
{
  MOCK_METHOD(void, gpiod_set_value_impl, (gpio_desc*, int));
  MOCK_METHOD(int, gpiod_get_value_impl, (gpio_desc*));
  MOCK_METHOD(int, desc_to_gpio_impl, (gpio_desc*));
  MOCK_METHOD(int, gpio_to_irq_impl, (int));
  MOCK_METHOD(void, gpiod_direction_input_impl, (gpio_desc*));
  MOCK_METHOD(int, gpiod_direction_output_impl, (gpio_desc*, int));
  MOCK_METHOD(void, gpiod_export_impl, (gpio_desc*, int));
  MOCK_METHOD(void, gpiod_unexport_impl, (gpio_desc*));
};