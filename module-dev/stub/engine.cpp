extern "C" {
  #include <engine.h>
}

#include <mock_api.hpp>
#include <gmock_engine.hpp>

void engine_init(engine* engine, pwm_device* pwm)
{
  if (!MockApi<GMockEngine>::has_mock()) {
    return;
  }

  MockApi<GMockEngine>::get_mock()->engine_init_impl(engine, pwm);
}

void engine_deinit(engine* engine)
{
  if (!MockApi<GMockEngine>::has_mock()) {
    return;
  }

  MockApi<GMockEngine>::get_mock()->engine_deinit_impl(engine);
}

void engine_start(engine* engine)
{
  if (!MockApi<GMockEngine>::has_mock()) {
    return;
  }

  MockApi<GMockEngine>::get_mock()->engine_start_impl(engine);
}

void engine_stop(engine* engine)
{
  if (!MockApi<GMockEngine>::has_mock()) {
    return;
  }

  MockApi<GMockEngine>::get_mock()->engine_stop_impl(engine);
}

engine_state engine_get_state(engine* engine)
{
  if (!MockApi<GMockEngine>::has_mock()) {
    return ENGINE_STATE_IDLE;
  }

  return MockApi<GMockEngine>::get_mock()->engine_get_state_impl(engine);
}