#include "IdleActor.h"
#include <Arduino.h>
#include <../../include/consts.h>

typedef union {
  uint8_t bytes[MESSAGE_BODY_LENGTH];
  SlideParams params;
} SlideParamMessage;

IdleActor::IdleActor(std::string peripheralName, std::string serviceUUID, std::string characteristicUUID)
{
  this->peripheralName = peripheralName;
  this->serviceUUID = serviceUUID;
  this->characteristicUUID = characteristicUUID;
}

void IdleActor::start()
{
  BLEDevice::init(this->peripheralName);
  this->pServer = BLEDevice::createServer();
  this->pService = this->pServer->createService(this->serviceUUID);
  this->pCharacteristic = pService->createCharacteristic(
                                         this->characteristicUUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE |
                                         BLECharacteristic::PROPERTY_WRITE_NR
                                       );
  this->pService->start();
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(this->serviceUUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
}

void IdleActor::step(SharedState* state)
{
  if (!state->activeProgram && this->readBLE(state))
  {
#ifdef DEBUG
    Serial.println("starting program");
#endif
    state->state = PROGRAM;
  }
}

bool IdleActor::readBLE(SharedState* state) {
  std::string value = this->pCharacteristic->getValue();
  const char* bytes = value.c_str();
  int l = value.length();
  if (l != MESSAGE_BODY_LENGTH+1 || bytes[0] != MESSAGE_START) {
    return false;
  }

  SlideParamMessage msg;
  for (int i = 0; i < MESSAGE_BODY_LENGTH; i++)
  {
    msg.bytes[i] = value[i + 1];
  }
  
  state->params = msg.params;
  state->activeProgram = true;

#ifdef DEBUG
  Serial.printf("Panning RPM: %f\nTracking mps: %f\n", state->params.panningRpm, state->params.trackingMps);
#endif

  this->pCharacteristic->setValue("");

  return true;
}