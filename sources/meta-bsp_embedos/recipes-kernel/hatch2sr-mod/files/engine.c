#include "engine.h"


/* Private function declarations */
static bool engine_is_slow_start_enabled(engine* engine);
static void engine_set_speed_pct(engine* engine, int speed);
static void engine_perform_slow_start(engine* engine);

/* Helper function declarations */
static int speed_pct_to_duty_ns(int speed_pct, int period_ns);

/* Public function defintitions */
void engine_init(engine* engine, struct pwm_device* pwm)
{
  engine->pwm = pwm;
  engine->state = ENGINE_STATE_IDLE;
  engine->slow_start = false;

  //TODO: Once device tree is fixed, add pwm->state.period instaed of ENIGNE_PWM_PERIOD_NS
  pwm_config(engine->pwm, speed_pct_to_duty_ns(0, ENGINE_PWM_PERIOD_NS), ENGINE_PWM_PERIOD_NS);
  pwm_enable(engine->pwm);
}

void engine_deinit(engine* engine)
{
  engine_stop(engine);
  pwm_put(engine->pwm);

  engine->pwm = NULL;
}

void engine_start(engine* engine)
{
  if (engine_is_slow_start_enabled(engine)) {
    engine_perform_slow_start(engine);
  } else {
    engine_set_speed_pct(engine, 100);
  }

  engine->state = ENGINE_STATE_RUNNING;
}

void engine_stop(engine* engine)
{
  engine_set_speed_pct(engine, 0);

  engine->state = ENGINE_STATE_IDLE;
}

engine_state engine_get_state(engine* engine)
{
  return engine->state;
}

/* Private function definitions */
bool engine_is_slow_start_enabled(engine* engine)
{
  return engine->slow_start == true;
}

void engine_set_speed_pct(engine* engine, int speed)
{
  engine->speed = speed;
  pwm_config(engine->pwm, speed_pct_to_duty_ns(speed, engine->pwm->state.period), engine->pwm->state.period);
}

void engine_perform_slow_start(engine* engine)
{

}

/* Helper function defintions */
int speed_pct_to_duty_ns(int speed_pct, int period_ns)
{
  return period_ns / 100 * speed_pct;
}

