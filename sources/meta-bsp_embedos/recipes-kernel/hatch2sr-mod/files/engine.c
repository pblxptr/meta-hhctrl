#include "engine.h"

#include <linux/module.h>	

void engine_init(Engine* engine, struct pwm_device* pwm)
{
  engine->pwm = pwm;

	pwm_config(engine->pwm, ENGINE_PWM_INITIAL_DUTY, ENGINE_PWM_PERIOD_NS);
}

void engine_deinit(Engine* engine)
{
  engine_stop(engine);
  pwm_put(engine->pwm);
}

void engine_start(Engine* engine)
{
	pwm_enable(engine->pwm);
}

void engine_stop(Engine* engine)
{
  pwm_disable(engine->pwm);
}
