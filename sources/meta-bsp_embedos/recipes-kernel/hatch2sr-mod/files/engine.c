#include "engine.h"

void engine_init(engine* engine, struct pwm_device* pwm)
{
  engine->pwm = pwm;
  engine->state = ENGINE_STATE_IDLE;

	pwm_config(engine->pwm, 2000000, ENGINE_PWM_PERIOD_NS);
  pwm_enable(engine->pwm);
}

void engine_deinit(engine* engine)
{
  //TODO: NUll all pointers
  engine_stop(engine);
  pwm_put(engine->pwm);
}

void engine_start(engine* engine)
{
	pwm_enable(engine->pwm);

  engine->state = ENGINE_STATE_RUNNING;
}

void engine_stop(engine* engine)
{
  pwm_disable(engine->pwm);

  engine->state = ENGINE_STATE_IDLE;
}

engine_state engine_get_state(engine* engine)
{
  return engine->state;
}