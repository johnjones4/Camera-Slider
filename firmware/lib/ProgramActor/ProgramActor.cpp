#include "ProgramActor.h"
#include "Arduino.h"
#include <../../include/consts.h>

ProgramActor::ProgramActor(Stepper *tracking, Stepper *panning)
{
  this->tracking = tracking;
  this->panning = panning;
}

void ProgramActor::step(SliderState* state)
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
      this->trackingSteps++;
      if (this->trackingSteps >= MAX_TRACKING_STEPS)
      {
        state->lastProgramTime = millis()-this->programStart;
#ifdef DEBUG
        Serial.printf("Completed %d steps in %d milliseconds\n", this->trackingSteps, state->lastProgramTime);
#endif
        state->mode = HOMING;
        state->activeProgram = false;
        state->percentComplete = 0;
        this->panningStepRate = 0;
        this->trackingStepRate = 0;
        this->lastPanningStep = 0;
        this->lastTrackingStep = 0;
        this->trackingSteps= 0;
        this->programStart = 0;
        return;
      }
    }
  }
}

void ProgramActor::recalculateStepRates(SliderState* state)
{
  if (this->panningStepRate == 0 && this->trackingStepRate == 0)
  {
    this->programStart = millis();
    
    long panningStepsPerMinute = STEPS_PER_ROTATION * state->params.panningRpm;
    this->panningStepRate = ONE_MINUTE_MICROS / panningStepsPerMinute;

    double millimetersPerSecond = state->params.trackingMps * ONE_METER_MILLIS;
    double stepsPerSecond = millimetersPerSecond / TRACKING_MILLIS_PER_STEP;
    this->trackingStepRate = ONE_SECOND_MICROS / stepsPerSecond;
#ifdef DEBUG
    Serial.printf("Panning steps per minute: %d\n", panningStepsPerMinute);
    Serial.printf("Tracking millimeters per second: %f\n", millimetersPerSecond);
    Serial.printf("Tracking steps per second: %f\n", stepsPerSecond);
    Serial.printf("recalulating speeds:\nPanning Steps: %d\nTracking steps: %d\n", this->panningStepRate, this->trackingStepRate);
#endif
  }
}
