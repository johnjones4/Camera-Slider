#include "BluetoothManager.h"
#include <../../include/consts.h>
#include <Arduino.h>

typedef union {
  uint8_t bytes[PARAMS_MESSAGE_BODY_LENGTH];
  SlideParams params;
} SlideParamMessage;

typedef union {
  uint8_t bytes[STATUS_MESSAGE_BODY_LENGTH];
  SharedState state;
} SlideStatusMessage;

BluetoothManager::BluetoothManager(std::string peripheralName, std::string serviceUUID, std::string programCharacteristicUUID, std::string statusCharacteristicUUID)
{
  this->peripheralName = peripheralName;
  this->serviceUUID = serviceUUID;
  this->programCharacteristicUUID = programCharacteristicUUID;
  this->statusCharacteristicUUID = statusCharacteristicUUID;
}

void BluetoothManager::start()
{
  BLEDevice::init(this->peripheralName);
  this->server = BLEDevice::createServer();
  this->service = this->server->createService(this->serviceUUID);
  this->programCharacteristic = service->createCharacteristic(
                                         this->programCharacteristicUUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE |
                                         BLECharacteristic::PROPERTY_WRITE_NR
                                       );
  this->statusCharacteristic = service->createCharacteristic(
                                         this->statusCharacteristicUUID,
                                         BLECharacteristic::PROPERTY_READ
                                       );
  this->service->start();
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(this->serviceUUID);
  BLEDevice::startAdvertising();
}

bool BluetoothManager::readSliderParams(SlideParams *params)
{
  std::string value = this->programCharacteristic->getValue();
  const char* bytes = value.c_str();
  int l = value.length();
  if (l != PARAMS_MESSAGE_BODY_LENGTH+1 || bytes[0] != MESSAGE_START || value.compare(lastParamValue) == 0) {
    return false;
  }
  lastParamValue = value;
  this->programCharacteristic->setValue("");

  SlideParamMessage msg;
  for (int i = 0; i < PARAMS_MESSAGE_BODY_LENGTH; i++)
  {
    msg.bytes[i] = value[i + 1];
  }
  
  *params = msg.params;

#ifdef DEBUG
  Serial.printf("Panning RPM: %f\nTracking mps: %f\n", msg.params.panningRpm, msg.params.trackingMps);
#endif

  return true;
}

void BluetoothManager::updateState(SharedState state)
{
  SlideStatusMessage msg;
  msg.state = state;
  this->statusCharacteristic->setValue(msg.bytes, STATUS_MESSAGE_BODY_LENGTH);
}
