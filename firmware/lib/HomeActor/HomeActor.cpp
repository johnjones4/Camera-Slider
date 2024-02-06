#include "HomeActor.h"
#include <Arduino.h>
#include <../../include/consts.h>

HomeActor::HomeActor(Stepper *tracking, int homePin)
{
  this->tracking = tracking;
  this->homePin = homePin;
  pinMode(this->homePin, INPUT_PULLUP);
}

void HomeActor::step(SliderState* state)
{
  if (this->isHomed())
  {
#ifdef DEBUG
    Serial.println("Now homed");
#endif
    state->mode = IDLE;
  }
  else
  {
    this->tracking->step(true);
    delayMicroseconds(STEP_WAIT_MICROS);
  }
}

bool HomeActor::isHomed()
{
  return digitalRead(this->homePin) == LOW;
}
