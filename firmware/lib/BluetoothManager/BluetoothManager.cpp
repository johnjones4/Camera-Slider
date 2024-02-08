#include "BluetoothManager.h"
#include <../../include/consts.h>
#include <Arduino.h>

typedef union {
  uint8_t bytes[PARAMS_MESSAGE_BODY_LENGTH];
  SliderParams params;
} SlideParamMessage;

typedef union {
  uint8_t bytes[STATUS_MESSAGE_BODY_LENGTH];
  SliderState state;
} SlideStatusMessage;

BluetoothManager::BluetoothManager(std::string peripheralName, std::string serviceUUID, std::string programCharacteristicUUID, std::string stateCharacteristicUUID)
{
  this->peripheralName = peripheralName;
  this->serviceUUID = serviceUUID;
  this->programCharacteristicUUID = programCharacteristicUUID;
  this->stateCharacteristicUUID = stateCharacteristicUUID;
}

void BluetoothManager::start()
{
  BLEDevice::init(this->peripheralName);
  this->server = BLEDevice::createServer();
  this->server->setCallbacks(this);
  this->service = this->server->createService(this->serviceUUID);
  this->programCharacteristic = service->createCharacteristic(
                                         this->programCharacteristicUUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE |
                                         BLECharacteristic::PROPERTY_WRITE_NR
                                       );
  this->stateCharacteristic = service->createCharacteristic(
                                         this->stateCharacteristicUUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_NOTIFY
                                       );
  this->service->start();
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(this->serviceUUID);
  BLEDevice::startAdvertising();
}

bool BluetoothManager::readSliderParams(SliderParams *params)
{
  std::string value = this->programCharacteristic->getValue();
  const char* bytes = value.c_str();
  int l = value.length();
  if (l != PARAMS_MESSAGE_BODY_LENGTH+1 || bytes[0] != MESSAGE_START) {
    return false;
  }
  this->programCharacteristic->setValue("");

  SlideParamMessage msg;
  for (int i = 0; i < PARAMS_MESSAGE_BODY_LENGTH; i++)
  {
    msg.bytes[i] = value[i + 1];
  }
  
  *params = msg.params;

#ifdef DEBUG
  Serial.printf("Panning RPM: %f\nTracking mps: %f\nDirection: %d\nPercent:%f\n", msg.params.speed.panningRpm, msg.params.speed.trackingMps, msg.params.rotationDirection, msg.params.percentDistance);
#endif

  return true;
}

void BluetoothManager::updateState(SliderState state)
{
  SlideStatusMessage msg;
  msg.state = state;
  this->stateCharacteristic->setValue(msg.bytes, STATUS_MESSAGE_BODY_LENGTH);
  this->stateCharacteristic->notify(true);
}

void BluetoothManager::onConnect(BLEServer* pServer, esp_ble_gatts_cb_param_t *param)
{
#ifdef DEBUG
  Serial.println("client connected");
#endif
}

void BluetoothManager::onDisconnect(BLEServer* pServer, esp_ble_gatts_cb_param_t *param)
{
#ifdef DEBUG
  Serial.println("client disconnected");
#endif
  BLEDevice::startAdvertising();
}

void BluetoothManager::onMtuChanged(BLEServer* pServer, esp_ble_gatts_cb_param_t* param)
{
#ifdef DEBUG
  Serial.println("client mtu changed");
#endif
}
