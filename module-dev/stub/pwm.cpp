extern "C" {
  #include <linux/pwm.h>
}

#include <mock_api.hpp>
#include <gmock_pwm.hpp>

void pwm_config(struct pwm_device* pwm, int duty, int period_ns)
{
  if (!MockApi<GMockPwm>::has_mock()) {
    return;
  }

  MockApi<GMockPwm>::get_mock()->pwm_config_impl(pwm, duty, period_ns);
}

void pwm_put(struct pwm_device* pwm)
{
  if (!MockApi<GMockPwm>::has_mock()) {
    return;
  }

  MockApi<GMockPwm>::get_mock()->pwm_put_impl(pwm);
}

void pwm_enable(struct pwm_device* pwm)
{
  if (!MockApi<GMockPwm>::has_mock()) {
    return;
  }

  MockApi<GMockPwm>::get_mock()->pwm_enable_impl(pwm);
}

void pwm_disable(struct pwm_device* pwm)
{
  if (!MockApi<GMockPwm>::has_mock()) {
    return;
  }

  MockApi<GMockPwm>::get_mock()->pwm_disable_impl(pwm);
}