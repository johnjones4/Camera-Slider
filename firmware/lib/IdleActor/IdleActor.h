#ifndef IDLE_ACTOR
#define IDLE_ACTOR

#include <../../include/SliderStateActor.h>
#include <BluetoothManager.h>
class IdleActor : public SliderStateActor
{
public:
  IdleActor(BluetoothManager *btManager);
  void step(SharedState* state);
private:
  BluetoothManager *btManager;
  bool readBLE(SharedState* state);
};

#endif
