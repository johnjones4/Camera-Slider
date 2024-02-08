#ifndef BLUETOOTH_MANAGER
#define BLUETOOTH_MANAGER

#include <string.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <../../include/types.h>

class BluetoothManager : BLEServerCallbacks
{
public:
  BluetoothManager(std::string peripheralName, std::string serviceUUID, std::string programCharacteristicUUID, std::string stateCharacteristicUUID);
  void start();
  bool readSliderParams(SliderParams *params);
  void updateState(SliderState state);
	void onConnect(BLEServer* pServer, esp_ble_gatts_cb_param_t *param);
	void onDisconnect(BLEServer* pServer, esp_ble_gatts_cb_param_t *param);
  void onMtuChanged(BLEServer* pServer, esp_ble_gatts_cb_param_t* param);
private:
  std::string peripheralName;
  std::string serviceUUID;
  std::string programCharacteristicUUID;
  std::string stateCharacteristicUUID;
  BLEServer *server;
  BLEService *service;
  BLECharacteristic *programCharacteristic;
  BLECharacteristic * stateCharacteristic;
};

#endif
