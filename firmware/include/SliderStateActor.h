#ifndef SLIDER_STATE_ACTOR
#define SLIDER_STATE_ACTOR

enum SliderState
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
  short activeProgram; //2
  SliderState state; //4
  float percentComplete; //4
} SharedState;

class SliderStateActor
{
public:
  virtual void step(SharedState* state) = 0;
};

#endif
