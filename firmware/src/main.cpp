#include <Arduino.h>
#include <SliderStateMachine.h>
#include <Stepper.h>
#include <HomeActor.h>
#include <IdleActor.h>
#include <consts.h>
#include <ProgramActor.h>

SliderStateMachine ssm;
Stepper tracking(TRACKING_PIN_EN, TRACKING_PIN_STEP, TRACKING_PIN_DIR);
Stepper panning(PANNING_PIN_EN, PANNING_PIN_STEP, PANNING_PIN_DIR);
HomeActor home(&tracking, HOME_PIN);
IdleActor idle(PERIPHERAL_NAME, SERVICE_UUID, CHARACTERISTIC_UUID);
ProgramActor program(&tracking, &panning);

void setup() {
  Serial.begin(9600);
  Serial.println("Starting");
  idle.start();
  ssm.registerActor(IDLE, &idle);
  ssm.registerActor(HOMING, &home);
  ssm.registerActor(PROGRAM, &program);
  Serial.println("Ready");
}

void loop() {
  ssm.step();
}
