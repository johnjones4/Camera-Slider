#ifndef BLUETOOTH_MANAGER
#define BLUETOOTH_MANAGER

#include <string.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <../../include/SliderStateActor.h>

class BluetoothManager
{
public:
  BluetoothManager(std::string peripheralName, std::string serviceUUID, std::string programCharacteristicUUID, std::string statusCharacteristicUUID);
  void start();
  bool readSliderParams(SlideParams *params);
  void updateState(SharedState state);
private:
  std::string peripheralName;
  std::string serviceUUID;
  std::string programCharacteristicUUID;
  std::string statusCharacteristicUUID;
  std::string lastParamValue = "";
  BLEServer *server;
  BLEService *service;
  BLECharacteristic *programCharacteristic;
  BLECharacteristic * statusCharacteristic;
};

#endif
