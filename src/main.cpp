#include <Arduino.h>
#include <SliderStateMachine.h>
#include <Stepper.h>
#include <HomeActor.h>
#include <IdleActor.h>

#define HOME_PIN 14

#define PERIPHERAL_NAME     "Camera Slider"
#define SERVICE_UUID        "A7F7A363-1642-475A-8252-8BCE4C4289E9"
#define CHARACTERISTIC_UUID "314A2B2A-EB2C-4D05-87CD-475DBA39BC0F"

SliderStateMachine ssm;
Stepper tracking(200, 2, 4, 5, 18);
Stepper panning(200, 19, 21, 22, 23);
HomeActor home(&tracking, HOME_PIN);
IdleActor idle(PERIPHERAL_NAME, SERVICE_UUID, CHARACTERISTIC_UUID);

void setup() {
  Serial.begin(9600);
  idle.start();
  ssm.registerActor(IDLE, &idle);
  ssm.registerActor(HOMING, &home);
}

void loop() {
  ssm.step();
}
