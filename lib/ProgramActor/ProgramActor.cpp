#include "ProgramActor.h"
#include "Arduino.h"

#define STEPS_PER_ROTATION 200
#define SHAFT_CIRCUMFRENCE 0.03455752
#define SHAFT_LENGTH 3
#define ONE_MINUTE_MICROS 60000000
#define MAX_TRACKING_STEPS (2.0 / SHAFT_CIRCUMFRENCE) * STEPS_PER_ROTATION

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
        state->state = IDLE;
        state->activeProgram = false;
        return;
      }
    }
  } else {
    state->state = IDLE;
    this->panningStepRate = 0;
    this->trackingStepRate = 0;
    this->lastPanningStep = 0;
    this->lastTrackingStep = 0;
  }
}

void ProgramActor::recalculateStepRates(SharedState* state)
{
  if (this->panningStepRate == 0 && this->trackingStepRate == 0)
  {
    unsigned long panningStepsPerMinute = (unsigned long)(STEPS_PER_ROTATION * state->params.panningRpm);
    this->panningStepRate = panningStepsPerMinute / ONE_MINUTE_MICROS;

    unsigned long metersPerMinute = state->params.trackingMps * 60;
    unsigned long rpm = metersPerMinute / SHAFT_CIRCUMFRENCE;
    unsigned long trackingStepsPerMinute = (unsigned long)(STEPS_PER_ROTATION * rpm);
    this->trackingStepRate = trackingStepsPerMinute / ONE_MINUTE_MICROS;

    Serial.printf("recalulating speeds:\nPanning Steps: %d\nTracking steps: %d\n", this->panningStepRate, this->trackingStepRate);
  }
}
