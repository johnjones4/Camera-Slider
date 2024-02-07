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

    bool completedProgram = this->trackingSteps >= MAX_TRACKING_STEPS;

    if (completedProgram && this->panningSteps == this->totalPanningSteps) {
      state->lastProgramTime = millis()-this->programStart;
      float seconds = ((float)state->lastProgramTime/1000.0);
      state->lastEffectiveSpeed.trackingMps = ((float)SHAFT_LENGTH_MILLIS/1000.0) / seconds;
      state->lastEffectiveSpeed.panningRpm = ((float)this->totalPanningSteps / ((float)STEPS_PER_ROTATION * (float)PANNING_GEAR_RATIO)) / (seconds/60.0);
  #ifdef DEBUG
      Serial.printf("Completed %d steps in %d milliseconds\n", this->trackingSteps, state->lastProgramTime);
  #endif
    }
    if (completedProgram && this->panningSteps <= 0) {
      state->mode = HOMING;
      state->activeProgram = false;
      state->percentComplete = 0;
      this->panningStepRate = 0;
      this->trackingStepRate = 0;
      this->lastPanningStep = 0;
      this->lastTrackingStep = 0;
      this->trackingSteps= 0;
      this->programStart = 0;
      this->panningSteps = 0;
      this->totalPanningSteps = 0;
      return;
    } else if (completedProgram && this->panningSteps <= this->totalPanningSteps) {
      this->panning->step(true);
      this->panningSteps--;
      delayMicroseconds(STEP_WAIT_MICROS);
    } else {
      state->percentComplete = (float)this->trackingSteps / (float)(MAX_TRACKING_STEPS);
 
      long panningStepWaited = micros() - this->lastPanningStep;
      if (panningStepWaited >= STEP_WAIT_MICROS && panningStepWaited >= this->panningStepRate) 
      {
        this->lastPanningStep = micros();
        this->panning->step(false);
        this->panningSteps++;
        this->totalPanningSteps++;
      }

      long trackingStepWaited = micros() - this->lastTrackingStep;
      if (trackingStepWaited >= STEP_WAIT_MICROS && trackingStepWaited >= this->trackingStepRate) 
      {
        this->lastTrackingStep = micros();
        this->tracking->step(false);
        this->trackingSteps++;
      }
    }
  }
}

void ProgramActor::recalculateStepRates(SliderState* state)
{
  if (this->panningStepRate == 0 && this->trackingStepRate == 0)
  {
    this->programStart = millis();
    
    long panningStepsPerMinute = STEPS_PER_ROTATION * PANNING_GEAR_RATIO * state->params.panningRpm;
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
