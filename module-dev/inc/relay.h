#pragma once

typedef struct relay {
  struct gpio_desc* gpio;
  int gpio_id;
} relay;

int relay_init(relay* relay, struct gpio_desc* gpio);
void relay_deinit(relay*);
void relay_set_to_open(relay* relay);
void relay_set_to_close(relay* relay);
