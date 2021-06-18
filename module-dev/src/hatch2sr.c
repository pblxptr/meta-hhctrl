#include "hatch2sr.h"

#include <stddef.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <stdbool.h>

#include "engine.h"
#include "relay.h"
#include "sensor.h"

/* Hatch private functions declarations */
static irqreturn_t sensor_isr(int irq, void* dev_id);
static bool can_change_position();
static bool is_changing_position();
static bool is_faulty(); 

static hatch2sr hatch;

int hatch2sr_init(struct pwm_device* pwm, struct gpio_desc* openpos, struct  gpio_desc* closedpos, struct gpio_desc* relay)
{
  if (pwm == NULL || openpos == NULL || closedpos == NULL || relay == NULL) {
    return -1;
  }

  //Initialize engine
  engine_init(&hatch.engine, pwm);

  //Initialize sensors
  if (sensor_init(&hatch.openpos, openpos, sensor_isr))
    return -1;

  if (sensor_init(&hatch.closedpos, closedpos, sensor_isr))
    return -1;

  // Relay init
  if (relay_init(&hatch.relay, relay))
    return -1;

  return 0;
}

void hatch2sr_deinit()
{
}

hatch2sr* hatch2sr_get()
{
  return &hatch;
}

void hatch2sr_open()
{
  if (sensor_get_value(&hatch.openpos)) {
    return;
  }

  if (!can_change_position()) {
    return;
  }

  relay_set_to_open(&hatch.relay);
  engine_start(&hatch.engine);
}

void hatch2sr_close()
{
  if (sensor_get_value(&hatch.closedpos)) {
    return;
  }

  if (!can_change_position()) {
    return;
  }

  relay_set_to_close(&hatch.relay);
  engine_start(&hatch.engine);
}

hatch_state hatch2sr_get_state()
{
  int open_sensor_val;
  int closed_sensor_val;
  engine_state engine_s;

  open_sensor_val = sensor_get_value(&hatch.openpos);
  closed_sensor_val = sensor_get_value(&hatch.closedpos);
  engine_s = engine_get_state(&hatch.engine);

  if (is_faulty()) {
    return HATCH_STATE_FAULTY;
  }
  if (is_changing_position()) {
    return HATCH_STATE_CHANGING_POSITION;
  }

  if (open_sensor_val) {
    return HATCH_STATE_OPEN;
  } else {
    return HATCH_STATE_CLOSED;
  }
}

/* Hatch private function declarations */
irqreturn_t sensor_isr(int irq, void* dev_id)
{
  engine_stop(&hatch.engine);

	return IRQ_HANDLED;	
}

bool can_change_position()
{
  return !is_changing_position() && !is_faulty();
}

bool is_changing_position()
{
  return engine_get_state(&hatch.engine) == ENGINE_STATE_RUNNING;
}

bool is_faulty()
{
  int open_sensor_val;
  int closed_sensor_val;

  open_sensor_val = sensor_get_value(&hatch.openpos);
  closed_sensor_val = sensor_get_value(&hatch.closedpos);

  return open_sensor_val && closed_sensor_val;
}