#include "engine.h"

/* Private function declarations */
static int speed_pct_to_duty_ns(int speed_pct, int period_ns);

void engine_init(engine* engine, struct pwm_device* pwm)
{
  engine->pwm = pwm;
  engine->state = ENGINE_STATE_IDLE;

  //TODO: Once device tree is fixed, add pwm->state.period instaed of ENIGNE_PWM_PERIOD_NS
  pwm_config(engine->pwm, speed_pct_to_duty_ns(20, ENGINE_PWM_PERIOD_NS), ENGINE_PWM_PERIOD_NS);
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

/* Private function definitions */
int speed_pct_to_duty_ns(int speed_pct, int period_ns)
{
  return (double)speed_pct / 100 * (double)period_ns; 
}