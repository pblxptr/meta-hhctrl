#pragma once

extern "C" {
  #include <engine.h>
}

#include <gmock/gmock.h>

struct GMockEngine {
  MOCK_METHOD(void, engine_init_impl, (engine*, pwm_device*));
  MOCK_METHOD(void, engine_deinit_impl, (engine*));
  MOCK_METHOD(void, engine_start_impl, (engine*));
  MOCK_METHOD(void, engine_stop_impl, (engine*));
  MOCK_METHOD(engine_state, engine_get_state_impl, (engine*));
};
