#include "HomeActor.h"
#include "Arduino.h"

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
    Serial.println("now homed");
    state->state = IDLE;
  }
  else
  {
    this->tracking->step(-1);
  }
}

bool HomeActor::isHomed()
{
  return digitalRead(this->homePin) == LOW;
}
