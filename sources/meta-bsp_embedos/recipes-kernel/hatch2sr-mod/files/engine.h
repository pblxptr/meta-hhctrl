#pragma once

#include <linux/pwm.h>
#include <stdbool.h>

#define EIGINE_MIN_SPEED_PCT    (30)
#define ENGINE_MAX_SPEED_PCT    (100)
#define ENGINE_PWM_PERIOD_NS    (10000000)
#define ENGINE_PWM_INITIAL_DUTY (0)

typedef enum engine_state {
  ENGINE_STATE_IDLE, 
  ENGINE_STATE_RUNNING
} engine_state;

typedef struct engine {
  struct pwm_device* pwm;
  engine_state state;
} engine;

void engine_init(engine* enigne, struct pwm_device* pwm);
void engine_deinit(engine* engine);
void engine_start(engine* engine);
void engine_stop(engine* engine);
engine_state engine_get_state(engine* engine);