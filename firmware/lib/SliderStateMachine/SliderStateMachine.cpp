#include "SliderStateMachine.h"
#include <Arduino.h>
#include <../../include/consts.h>

SliderStateMachine::SliderStateMachine(BluetoothManager *btManager)
{
  this->btManager = btManager;
}

int SliderStateMachine::registerActor(SliderState state, SliderStateActor *actor)
{
  this->actors[state] = actor;
  return 0;
}

int SliderStateMachine::step()
{
#ifdef DEBUG
  SliderState startingState = this->state.state;
#endif
  SliderStateActor* actor = this->actors[this->state.state];
  actor->step(&(this->state));
  if (millis() % 1000 == 0)
  {
    btManager->updateState(this->state);
  }
#ifdef DEBUG
  if (startingState != this->state.state) {
    Serial.print("New mode: ");
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
  }
#endif
  return 0;
}
