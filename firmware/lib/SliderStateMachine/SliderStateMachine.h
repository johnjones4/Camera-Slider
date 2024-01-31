#ifndef SLIDER_STATE_MACHINE
#define SLIDER_STATE_MACHINE

#include <../../include/SliderStateActor.h>
#include <BluetoothManager.h>
class SliderStateMachine
{
public:
  SliderStateMachine(BluetoothManager *btManager);
  int step();
  int registerActor(SliderState state, SliderStateActor *actor);
private:
  BluetoothManager *btManager;
  SharedState state = SharedState{
    activeProgram: false,
    state: HOMING,
  };
  SliderStateActor *actors[N_SLIDER_STATES];
};

#endif
