#include "SliderStateMachine.h"

int SliderStateMachine::registerActor(SliderState state, SliderStateActor *actor)
{
  this->actors[state] = actor;
  return 0;
}

int SliderStateMachine::step()
{
  SliderStateActor* actor = this->actors[this->state.state];
  actor->step(&(this->state));
  return 0;
}
