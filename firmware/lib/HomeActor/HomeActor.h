#ifndef HOME_ACTOR
#define HOME_ACTOR

#include <../../include/types.h>
#include <Stepper.h>

class HomeActor : public SliderStateActor
{
public:
  HomeActor(Stepper *tracking, int homePin);
  void step(SliderState* state);
private:
  bool isHomed();
  Stepper *tracking;
  int homePin;
};

#endif
