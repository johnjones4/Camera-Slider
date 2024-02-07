#include "SliderStateMachine.h"
#include <Arduino.h>
#include <../../include/consts.h>

SliderStateMachine::SliderStateMachine(BluetoothManager *btManager)
{
  this->btManager = btManager;
}

int SliderStateMachine::registerActor(SliderMode mode, SliderStateActor *actor)
{
  this->actors[mode] = actor;
  return 0;
}

int SliderStateMachine::step()
{
  SliderMode startingState = this->state.mode;
  SliderStateActor* actor = this->actors[this->state.mode];
  actor->step(&(this->state));
  if (startingState != this->state.mode || millis() % 1000 == 0)
  {
    btManager->updateState(this->state);
  }
#ifdef DEBUG
  if (startingState != this->state.mode) {
    Serial.print("New mode: ");
    switch (this->state.mode)
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
