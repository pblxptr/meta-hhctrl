extern "C" {
  #include <sensor.h>
}

#include <mock_api.hpp>
#include <gmock_sensor.hpp>

int sensor_init(sensor* sensor, struct gpio_desc* gpio, irq_handler_t irqhandler)
{
  if (!MockApi<GMockSensor>::has_mock()) {
    return 0;
  }

  return MockApi<GMockSensor>::get_mock()->sensor_init(sensor, gpio, irqhandler);
}

int sensor_get_value(sensor* sensor)
{
  if (!MockApi<GMockSensor>::has_mock()) {
    return 0;
  }

  return MockApi<GMockSensor>::get_mock()->sensor_get_value(sensor);
}

