#ifndef SLIDER_STATE_MACHINE
#define SLIDER_STATE_MACHINE

#include <../../include/types.h>
#include <BluetoothManager.h>
class SliderStateMachine
{
public:
  SliderStateMachine(BluetoothManager *btManager);
  int step();
  int registerActor(SliderMode mode, SliderStateActor *actor);
private:
  BluetoothManager *btManager;
  SliderState state = SliderState{
    activeProgram: false,
    mode: HOMING,
  };
  SliderStateActor *actors[N_SLIDER_STATES];
};

#endif
