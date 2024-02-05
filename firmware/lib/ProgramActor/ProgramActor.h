#ifndef PROGRAM_ACTOR
#define PROGRAM_ACTOR

#include "../../include/SliderStateActor.h"
#include "Stepper.h"

class ProgramActor : public SliderStateActor
{
public:
  ProgramActor(Stepper *tracking, Stepper *panning);
  void step(SharedState* state);
private:
  Stepper *tracking;
  Stepper *panning;
  unsigned long trackingStepRate = 0;
  unsigned long lastTrackingStep = 0;
  unsigned long panningStepRate = 0;
  unsigned long lastPanningStep = 0;
  unsigned long trackingSteps = 0;
  unsigned long programStart = 0;
  void recalculateStepRates(SharedState* state);
};

#endif
