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
  SlideParams params;
  bool activeProgram;
  SliderState state;
} SharedState;

class SliderStateActor
{
public:
  void step(SharedState* state);
};

#endif
