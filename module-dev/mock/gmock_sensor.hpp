#pragma once

extern "C" {
  #include <sensor.h>
}

#include <gmock/gmock.h>

struct GMockSensor {
  virtual int sensor_init(sensor* sensor, gpio_desc* gpio, irq_handler_t irq)
  {
    return sensor_init_impl(sensor, gpio, irq);
  }

  virtual void sensor_deinit(sensor* sensor)
  {
    sensor_deinit_impl(sensor);
  }

  virtual int sensor_get_value(sensor* sensor)
  {
    return sensor_get_value_impl(sensor);
  }

  MOCK_METHOD(int, sensor_init_impl, (sensor*, gpio_desc*, irq_handler_t));
  MOCK_METHOD(void, sensor_deinit_impl, (sensor*));
  MOCK_METHOD(int, sensor_get_value_impl, (sensor*));
};