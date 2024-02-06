#ifndef IDLE_ACTOR
#define IDLE_ACTOR

#include <../../include/types.h>
#include <BluetoothManager.h>
class IdleActor : public SliderStateActor
{
public:
  IdleActor(BluetoothManager *btManager);
  void step(SliderState* state);
private:
  BluetoothManager *btManager;
  bool readBLE(SliderState* state);
};

#endif
