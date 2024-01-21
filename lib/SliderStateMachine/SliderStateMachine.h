#ifndef SLIDER_STATE_MACHINE
#define SLIDER_STATE_MACHINE

#include <../../include/SliderStateActor.h>

class SliderStateMachine
{
public:
  int step();
  int registerActor(SliderState state, SliderStateActor *actor);
private:
  SharedState state = SharedState{
    activeProgram: false,
    state: HOMING,
  };
  SliderStateActor *actors[N_SLIDER_STATES];
};

#endif
