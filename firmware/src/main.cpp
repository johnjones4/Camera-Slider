#include <Arduino.h>
#include <SliderStateMachine.h>
#include <Stepper.h>
#include <HomeActor.h>
#include <IdleActor.h>
#include <consts.h>
#include <ProgramActor.h>
#include <BluetoothManager.h>

BluetoothManager btm(PERIPHERAL_NAME, SERVICE_UUID, PROGRAM_CHARACTERISTIC_UUID, STATUS_CHARACTERISTIC_UUID);
SliderStateMachine ssm(&btm);
Stepper tracking(TRACKING_PIN_EN, TRACKING_PIN_STEP, TRACKING_PIN_DIR);
Stepper panning(PANNING_PIN_EN, PANNING_PIN_STEP, PANNING_PIN_DIR);
HomeActor home(&tracking, HOME_PIN);
IdleActor idle(&btm);
ProgramActor program(&tracking, &panning);

void setup() {
  Serial.begin(9600);
  Serial.println("Starting");
  tracking.start();
  panning.start();
  btm.start();
  ssm.registerActor(IDLE, &idle);
  ssm.registerActor(HOMING, &home);
  ssm.registerActor(PROGRAM, &program);
  Serial.println("Ready");
}

void loop() {
  ssm.step();
}
