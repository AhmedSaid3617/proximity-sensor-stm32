#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include "nvic.h"
#include "gpio.h"
#include "timer.h"
#include "exti.h"

#define TRIGGER 9
#define ECHO 10

void US_send_pulse();
void US_init();

#endif