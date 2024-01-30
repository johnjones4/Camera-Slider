#include "HomeActor.h"
#include <Arduino.h>
#include <../../include/consts.h>

HomeActor::HomeActor(Stepper *tracking, int homePin)
{
  this->tracking = tracking;
  this->homePin = homePin;
  pinMode(this->homePin, INPUT_PULLUP);
}

void HomeActor::step(SharedState* state)
{
  if (this->isHomed())
  {
#ifdef DEBUG
    Serial.println("Now homed");
#endif
    state->state = IDLE;
  }
  else
  {
    this->tracking->step(true);
  }
}

bool HomeActor::isHomed()
{
  return digitalRead(this->homePin) == LOW;
}
