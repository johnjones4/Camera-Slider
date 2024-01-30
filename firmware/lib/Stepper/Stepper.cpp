#include "Arduino.h"
#include "Stepper.h"

Stepper::Stepper(int enPin, int stepPin, int dirPin)
{
  this->enPin = enPin;
  this->stepPin = stepPin;
  this->dirPin = dirPin;
}

void Stepper::start()
{
  pinMode(enPin, OUTPUT);
  digitalWrite(enPin, HIGH); //deactivate driver (LOW active)
  pinMode(dirPin, OUTPUT);
  
  digitalWrite(dirPin, LOW); //LOW or HIGH
  pinMode(stepPin, OUTPUT);
  digitalWrite(stepPin, LOW);

  digitalWrite(enPin, LOW); //activate driver
}

void Stepper::step(bool forward)
{
  digitalWrite(dirPin, forward ? LOW : HIGH);
  digitalWrite(stepPin, HIGH);
  delay(1);
  digitalWrite(stepPin, LOW);
  delay(1);
}
