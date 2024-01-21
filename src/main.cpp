#include <Arduino.h>
#include <SliderStateMachine.h>
#include <Stepper.h>
#include <HomeActor.h>
#include <IdleActor.h>
#include <consts.h>

SliderStateMachine ssm;
Stepper tracking(200, TRACKING_PIN_1, TRACKING_PIN_2, TRACKING_PIN_3, TRACKING_PIN_4);
Stepper panning(200, PANNING_PIN_1, PANNING_PIN_2, PANNING_PIN_3, PANNING_PIN_4);
HomeActor home(&tracking, HOME_PIN);
IdleActor idle(PERIPHERAL_NAME, SERVICE_UUID, CHARACTERISTIC_UUID);

void setup() {
  Serial.begin(9600);
  Serial.println("Starting");
  idle.start();
  ssm.registerActor(IDLE, &idle);
  ssm.registerActor(HOMING, &home);
  Serial.println("Ready");
}

void loop() {
  ssm.step();
}
