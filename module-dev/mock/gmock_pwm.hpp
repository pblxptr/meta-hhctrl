#pragma once

#include <gmock/gmock.h>

struct GMockPwm {
  MOCK_METHOD(void, pwm_config_impl, (pwm_device*, int, int));
  MOCK_METHOD(void, pwm_put_impl, (pwm_device*));
  MOCK_METHOD(void, pwm_enable_impl, (pwm_device*));
  MOCK_METHOD(void, pwm_disable_impl, (pwm_device*));
};
