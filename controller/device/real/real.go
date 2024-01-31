package real

import (
	"errors"
	"log"
	"main/core"
	"sync"

	"tinygo.org/x/bluetooth"
)

const (
	serviceUUIDStr               = "A7F7A363-1642-475A-8252-8BCE4C4289E9"
	programCharacteristicUUIDStr = "314A2B2A-EB2C-4D05-87CD-475DBA39BC0F"
	statusCharacteristicUUIDStr  = "9C282BA8-2BCE-4DF7-AA80-ED014C471B21"
)

type RealDevice struct {
	programCharacteristic *bluetooth.DeviceCharacteristic
	statusCharacteristic  *bluetooth.DeviceCharacteristic
	characteristicLock    sync.Mutex
}

func New() *RealDevice {
	d := &RealDevice{}
	return d
}

func (d *RealDevice) SendParams(params core.SliderParams) error {
	message, err := params.Bytes()
	if err != nil {
		return nil
	}

	_, err = d.programCharacteristic.WriteWithoutResponse(message)
	if err != nil {
		return err
	}

	return nil
}

func (d *RealDevice) GetStatus() (core.SliderStatus, error) {
	buffer := make([]byte, 512)
	length, err := d.statusCharacteristic.Read(buffer)
	if err != nil {
		return core.SliderStatus{}, err
	}
	trueBuffer := buffer[:length]
	return core.SliderStatusFromBytes(trueBuffer)
}

func (d *RealDevice) Connect() error {
	if d.Connected() {
		return nil
	}

	adapter := bluetooth.DefaultAdapter
	err := adapter.Enable()
	if err != nil {
		return err
	}

	scanResultChan := make(chan bluetooth.ScanResult)

	serviceUUID, err := bluetooth.ParseUUID(serviceUUIDStr)
	if err != nil {
		return err
	}

	programCharacteristicUUID, err := bluetooth.ParseUUID(programCharacteristicUUIDStr)
	if err != nil {
		return err
	}

	statusCharacteristicUUID, err := bluetooth.ParseUUID(statusCharacteristicUUIDStr)
	if err != nil {
		return err
	}

	err = adapter.Scan(func(adapter *bluetooth.Adapter, device bluetooth.ScanResult) {
		log.Println(device.LocalName())
		if device.LocalName() == "Camera Slider" && device.HasServiceUUID(serviceUUID) {
			adapter.StopScan()
			scanResultChan <- device
		}
	})
	if err != nil {
		return err
	}

	scanResult := <-scanResultChan
	device, err := adapter.Connect(scanResult.Address, bluetooth.ConnectionParams{})
	if err != nil {
		return err
	}

	srvcs, err := device.DiscoverServices([]bluetooth.UUID{serviceUUID})
	if err != nil {
		return err
	}

	if len(srvcs) != 1 {
		return errors.New("service not found")
	}

	chars, err := srvcs[0].DiscoverCharacteristics([]bluetooth.UUID{programCharacteristicUUID, statusCharacteristicUUID})
	if err != nil {
		return err
	}

	if len(chars) != 2 {
		return errors.New("characteristics not found")
	}

	d.characteristicLock.Lock()
	for _, char := range chars {
		switch char.UUID().String() {
		case programCharacteristicUUID.String():
			d.programCharacteristic = &char
		case statusCharacteristicUUID.String():
			d.statusCharacteristic = &char
		}
	}
	d.characteristicLock.Unlock()

	return nil
}

func (d *RealDevice) Connected() bool {
	d.characteristicLock.Lock()
	defer d.characteristicLock.Unlock()
	return d.programCharacteristic != nil && d.statusCharacteristic != nil
}
