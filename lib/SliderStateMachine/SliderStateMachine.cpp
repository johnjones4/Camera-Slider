#include "SliderStateMachine.h"
#include <Arduino.h>
#include <../../include/consts.h>

int SliderStateMachine::registerActor(SliderState state, SliderStateActor *actor)
{
  this->actors[state] = actor;
  return 0;
}

int SliderStateMachine::step()
{
#ifdef DEBUG
  Serial.print("Current mode: ");
  switch (this->state.state)
  {
  case HOMING:
    Serial.println("homing");
    break;
  case IDLE:
    Serial.println("idle");
    break;
  case PROGRAM:
    Serial.println("program");
    break;
  }
#endif
  SliderStateActor* actor = this->actors[this->state.state];
  actor->step(&(this->state));
  return 0;
}
