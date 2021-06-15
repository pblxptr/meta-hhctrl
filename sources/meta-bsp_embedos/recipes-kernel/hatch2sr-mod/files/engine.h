#pragma once

#include <linux/pwm.h>
#include <stdbool.h>

#define EIGINE_MIN_SPEED_PCT    (30)
#define ENGINE_MAX_SPEED_PCT    (100)
#define ENGINE_PWM_PERIOD_NS    (10000000)
#define ENGINE_PWM_INITIAL_DUTY (0)

typedef struct __Engine__ {
  struct pwm_device* pwm;
} Engine;

void engine_init(Engine* enigne, struct pwm_device* pwm);
void engine_deinit(Engine* engine);
void engine_start(Engine* engine);
void engine_stop(Engine* engine);