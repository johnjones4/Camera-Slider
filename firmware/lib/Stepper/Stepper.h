#ifndef STEPPER
#define STEPPER
class Stepper {
  public:
    Stepper(int enPin, int stepPin, int dirPin);
    void start();
    void step(bool forward);
  private:
    int enPin;
    int stepPin;
    int dirPin;
};

#endif

