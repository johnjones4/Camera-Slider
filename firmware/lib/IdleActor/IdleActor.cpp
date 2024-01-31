#include "IdleActor.h"
#include <Arduino.h>
#include <../../include/consts.h>

IdleActor::IdleActor(BluetoothManager *btManager)
{
  this->btManager = btManager;
}

void IdleActor::step(SharedState* state)
{
  if (!state->activeProgram && this->readBLE(state))
  {
#ifdef DEBUG
    Serial.println("starting program");
#endif
    state->state = PROGRAM;
  }
}

bool IdleActor::readBLE(SharedState* state) {
  state->activeProgram = this->btManager->readSliderParams(&state->params);
  state->percentComplete = 0;
  return state->activeProgram;
}
