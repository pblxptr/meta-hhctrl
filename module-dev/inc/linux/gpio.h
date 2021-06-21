#pragma once

typedef struct gpio_desc {

} gpio_desc;

void gpiod_set_value(struct gpio_desc* gpio, int val);
int gpiod_get_value(struct gpio_desc* gpio);
int desc_to_gpio(struct gpio_desc* gpio);
int gpio_to_irq(int gpio);
void gpiod_direction_input(struct gpio_desc* gpio);
int gpiod_direction_output(struct gpio_desc *desc, int value);
void gpiod_export(struct gpio_desc* gpio, int change);
void gpiod_unexport(struct gpio_desc*);