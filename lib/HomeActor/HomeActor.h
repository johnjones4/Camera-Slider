#ifndef HOME_ACTOR
#define HOME_ACTOR

#include <SliderStateActor.h>
#include <Stepper.h>

class HomeActor : public SliderStateActor
{
public:
  HomeActor(Stepper *tracking, int homePin);
  void step(SharedState* state);
private:
  bool isHomed();
  Stepper *tracking;
  int homePin;
};

#endif
