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
} SliderSpeed;
  
typedef struct {
  SliderSpeed speed; // 8
  bool rotationDirection; // 1
  float percentDistance; // 4
} SliderParams;

typedef struct {
  SliderParams params; // 13
  bool activeProgram; // 1
  SliderMode mode; // 4
  float percentComplete; // 4
  long lastProgramTime; // 8
  SliderSpeed lastEffectiveSpeed; // 8
} SliderState;

class SliderStateActor
{
public:
  virtual void step(SliderState* state) = 0;
};

#endif
