#define DEBUG

#define HOME_PIN 14

#define PERIPHERAL_NAME     "Camera Slider"
#define SERVICE_UUID        "A7F7A363-1642-475A-8252-8BCE4C4289E9"
#define CHARACTERISTIC_UUID "314A2B2A-EB2C-4D05-87CD-475DBA39BC0F"

#define TRACKING_PIN_1 2
#define TRACKING_PIN_2 4 
#define TRACKING_PIN_3 5
#define TRACKING_PIN_4 18

#define PANNING_PIN_1 19
#define PANNING_PIN_2 21
#define PANNING_PIN_3 22
#define PANNING_PIN_4 23

#define MESSAGE_START 0xA4
#define MESSAGE_BODY_LENGTH 8

#define STEPS_PER_ROTATION 200
#define SHAFT_CIRCUMFRENCE 0.03455752
#define SHAFT_LENGTH 1
#define ONE_MINUTE_MICROS 60000000
#define MAX_TRACKING_STEPS (SHAFT_LENGTH / SHAFT_CIRCUMFRENCE) * STEPS_PER_ROTATION




/*
  SilentStepStick TMC2208/TMC2209 Example
  Rsense: 0.11 Ohm

  Other examples/libraries can be found here:
  https://github.com/teemuatlut/TMCStepper
  https://github.com/trinamic/TMC-API
  https://github.com/manoukianv/TMC2208Pilot

  Example source code free to use.
  Further information: https://learn.watterott.com/license/
*/

#include <Arduino.h>

// Note: You also have to connect GND, 5V/VIO and VM.
//       A connection diagram can be found in the schematics.
#define EN_PIN    5 //enable (CFG6)
#define STEP_PIN  4 //step
#define DIR_PIN   2 //direction

#define EN_PIN_1    22 //enable (CFG6)
#define STEP_PIN_1  21 //step
#define DIR_PIN_1   22 //direction

void setup()
{
  //set pin modes
  pinMode(EN_PIN, OUTPUT);
  digitalWrite(EN_PIN, HIGH); //deactivate driver (LOW active)
  pinMode(DIR_PIN, OUTPUT);
  
  digitalWrite(DIR_PIN, LOW); //LOW or HIGH
  pinMode(STEP_PIN, OUTPUT);
  digitalWrite(STEP_PIN, LOW);

  digitalWrite(EN_PIN, LOW); //activate driver

  pinMode(EN_PIN_1, OUTPUT);
  digitalWrite(EN_PIN_1, HIGH); //deactivate driver (LOW active)
  pinMode(DIR_PIN_1, OUTPUT);
  
  digitalWrite(DIR_PIN_1, LOW); //LOW or HIGH
  pinMode(STEP_PIN_1, OUTPUT);
  digitalWrite(STEP_PIN_1, LOW);

  digitalWrite(EN_PIN_1, LOW); //activate driver
  //LED BUILT_IN is GPIO 33


  Serial.begin(9600);
  Serial.println("Booting");
}

void loop()
{
 
  //make steps
  digitalWrite(STEP_PIN, HIGH);
  delay(2);
  digitalWrite(STEP_PIN, LOW);
  delay(2);

  digitalWrite(STEP_PIN_1, HIGH);
  delay(2);
  digitalWrite(STEP_PIN_1, LOW);
  delay(2);

/*
  digitalWrite(33, LOW); //Turn on
  delay (1000); //Wait 1 sec
  digitalWrite(33, HIGH); //Turn off
  delay (1000); //Wait 1 sec
*/
}