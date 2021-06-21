extern "C" {
  #include <relay.h>
}

#include <mock_api.hpp>
#include <gmock_relay.hpp>

int relay_init(relay* relay, gpio_desc* gpio)
{
  if (!MockApi<GMockRelay>::has_mock()) {
    return 0;
  }

  return MockApi<GMockRelay>::get_mock()->relay_init_impl(relay, gpio);
}

void relay_deinit(relay* relay)
{
  if (!MockApi<GMockRelay>::has_mock()) {
    return;
  }

  MockApi<GMockRelay>::get_mock()->relay_deinit_impl(relay);
}

void relay_set_to_open(relay* relay)
{
  if (!MockApi<GMockRelay>::has_mock()) {
    return;
  }

  MockApi<GMockRelay>::get_mock()->relay_set_to_open_impl(relay);
}

void relay_set_to_close(relay* relay)
{
  if (!MockApi<GMockRelay>::has_mock()) {
    return;
  }

  MockApi<GMockRelay>::get_mock()->relay_set_to_close_impl(relay);
}
