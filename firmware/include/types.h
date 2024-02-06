#ifndef SLIDER_STATE_ACTOR
#define SLIDER_STATE_ACTOR
#include <Arduino.h>

enum SliderMode
{
  HOMING = 0,
  IDLE = 1,
  PROGRAM = 2
};

#define N_SLIDER_STATES 3

typedef struct {
  float trackingMps;
  float panningRpm;
} SlideParams;

typedef struct {
  SlideParams params; //8
  uint32_t activeProgram; //4
  SliderMode mode; //4
  float percentComplete; //4
  long lastProgramTime; //8
} SliderState;

class SliderStateActor
{
public:
  virtual void step(SliderState* state) = 0;
};

#endif
