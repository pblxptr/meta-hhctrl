#pragma once

struct pwm_device {};

void pwm_config(struct pwm_device* pwm, int duty, int period_ns);
void pwm_put(struct pwm_device* pwm);
void pwm_enable(struct pwm_device* pwm);
void pwm_disable(struct pwm_device* pwm);