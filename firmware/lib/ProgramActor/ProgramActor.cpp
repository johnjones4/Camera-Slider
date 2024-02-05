#include "ProgramActor.h"
#include "Arduino.h"
#include <../../include/consts.h>

ProgramActor::ProgramActor(Stepper *tracking, Stepper *panning)
{
  this->tracking = tracking;
  this->panning = panning;
}

void ProgramActor::step(SharedState* state)
{
  if (state->activeProgram)
  {
    recalculateStepRates(state);

    state->percentComplete = (float)this->trackingSteps / (float)(MAX_TRACKING_STEPS);
 
    if (micros() - this->lastPanningStep >= this->panningStepRate) 
    {
      this->lastPanningStep = micros();
      this->panning->step(false);
    }

    if (micros() - this->lastTrackingStep >= this->trackingStepRate) 
    {
      this->lastTrackingStep = micros();
      this->tracking->step(false);
      if (++this->trackingSteps >= MAX_TRACKING_STEPS)
      {
        state->state = HOMING;
        state->activeProgram = false;
        state->percentComplete = 0;
        this->panningStepRate = 0;
        this->trackingStepRate = 0;
        this->lastPanningStep = 0;
        this->lastTrackingStep = 0;
        this->trackingSteps= 0;
        return;
      }
    }
  }
}

void ProgramActor::recalculateStepRates(SharedState* state)
{
  if (this->panningStepRate == 0 && this->trackingStepRate == 0)
  {
    float panningStepsPerMinute = STEPS_PER_ROTATION * state->params.panningRpm;
    this->panningStepRate = ONE_MINUTE_MICROS / panningStepsPerMinute;

    float metersPerMinute = state->params.trackingMps * 60;
    float rpm = metersPerMinute / SHAFT_CIRCUMFRENCE;
    float trackingStepsPerMinute = STEPS_PER_ROTATION * rpm;
    this->trackingStepRate = ONE_MINUTE_MICROS / trackingStepsPerMinute;

#ifdef DEBUG
    Serial.printf("recalulating speeds:\nPanning Steps: %d\nTracking steps: %d\n", this->panningStepRate, this->trackingStepRate);
#endif
  }
}
