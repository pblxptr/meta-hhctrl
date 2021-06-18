#pragma once

extern "C" {
  #include <relay.h>
}

#include <gmock/gmock.h>

struct GMockRelay {
  MOCK_METHOD(int, relay_init_impl, (relay*, gpio_desc*));
  MOCK_METHOD(void, relay_set_to_open_impl, (relay*));
  MOCK_METHOD(void, relay_set_to_close_impl, (relay*));
};
