#ifndef IDLE_ACTOR
#define IDLE_ACTOR

#include <../../include/SliderStateActor.h>
#include <string.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

class IdleActor : public SliderStateActor
{
public:
  IdleActor(std::string peripheralName, std::string serviceUUID, std::string characteristicUUID);
  void step(SharedState* state);
  void start();
private:
  std::string peripheralName;
  std::string serviceUUID;
  std::string characteristicUUID;
  BLEServer *pServer;
  BLEService *pService;
  BLECharacteristic *pCharacteristic;
  bool readBLE(SharedState* state);
};

#endif
