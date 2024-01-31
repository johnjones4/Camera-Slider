```go
package main

import (
	"bytes"
	"encoding/binary"
	"log"
	"time"

	"tinygo.org/x/bluetooth"
)

const (
	serviceUUIDStr        = "A7F7A363-1642-475A-8252-8BCE4C4289E9"
	characteristicUUIDStr = "314A2B2A-EB2C-4D05-87CD-475DBA39BC0F"
)

type sliderParams struct {
	trackingMps float32
	panningRpm  float32
}

func main() {
	adapter := bluetooth.DefaultAdapter
	// Enable adapter
	err := adapter.Enable()
	if err != nil {
		panic(err)
	}

	scanResultChan := make(chan bluetooth.ScanResult)

	serviceUUID, err := bluetooth.ParseUUID(serviceUUIDStr)
	if err != nil {
		panic(err)
	}

	characteristicUUID, err := bluetooth.ParseUUID(characteristicUUIDStr)
	if err != nil {
		panic(err)
	}

	// Start scanning and define callback for scan results
	err = adapter.Scan(func(adapter *bluetooth.Adapter, device bluetooth.ScanResult) {
		log.Println(device.LocalName())
		if device.LocalName() == "Camera Slider" && device.HasServiceUUID(serviceUUID) {
			adapter.StopScan()
			scanResultChan <- device
		}
	})
	if err != nil {
		panic(err)
	}

	scanResult := <-scanResultChan
	device, err := adapter.Connect(scanResult.Address, bluetooth.ConnectionParams{})
	if err != nil {
		panic(err)
	}

	srvcs, err := device.DiscoverServices([]bluetooth.UUID{serviceUUID})
	if err != nil {
		panic(err)
	}

	if len(srvcs) != 1 {
		panic("service not found")
	}

	chars, err := srvcs[0].DiscoverCharacteristics([]bluetooth.UUID{characteristicUUID})
	if err != nil {
		panic(err)
	}

	if len(chars) != 1 {
		panic("characteristic not found")
	}

	message, err := sliderParams{
		trackingMps: 0.1,
		panningRpm:  60,
	}.bytes()
	if err != nil {
		panic(err)
	}

	_, err = chars[0].WriteWithoutResponse(message)
	if err != nil {
		panic(err)
	}
	time.Sleep(time.Second * 5)

	log.Println("done")
}

func (p sliderParams) bytes() ([]byte, error) {
	var buf bytes.Buffer

	buf.WriteByte(0xA4)

	err := binary.Write(&buf, binary.LittleEndian, p.trackingMps)
	if err != nil {
		return nil, err
	}

	err = binary.Write(&buf, binary.LittleEndian, p.panningRpm)
	if err != nil {
		return nil, err
	}

	return buf.Bytes(), nil
}

```
