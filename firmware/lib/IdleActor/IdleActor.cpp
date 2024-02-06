#include "IdleActor.h"
#include <Arduino.h>
#include <../../include/consts.h>

IdleActor::IdleActor(BluetoothManager *btManager)
{
  this->btManager = btManager;
}

void IdleActor::step(SliderState* state)
{
  if (!state->activeProgram && this->readBLE(state))
  {
#ifdef DEBUG
    Serial.println("starting program");
#endif
    state->mode = PROGRAM;
  }
}

bool IdleActor::readBLE(SliderState* state) {
  state->activeProgram = this->btManager->readSliderParams(&state->params);
  state->percentComplete = 0;
  return state->activeProgram;
}
