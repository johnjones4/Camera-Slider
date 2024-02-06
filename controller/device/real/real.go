package real

import (
	"errors"
	"main/core"
	"sync"
	"time"

	"github.com/sirupsen/logrus"
	"tinygo.org/x/bluetooth"
)

const (
	serviceUUIDStr               = "A7F7A363-1642-475A-8252-8BCE4C4289E9"
	programCharacteristicUUIDStr = "314A2B2A-EB2C-4D05-87CD-475DBA39BC0F"
	stateCharacteristicUUIDStr   = "9C282BA8-2BCE-4DF7-AA80-ED014C471B21"
)

type stateSubscriber struct {
	state chan core.SliderState
	err   chan error
}
type RealDevice struct {
	programCharacteristic *bluetooth.DeviceCharacteristic
	stateCharacteristic   *bluetooth.DeviceCharacteristic
	characteristicLock    sync.Mutex
	log                   logrus.FieldLogger
	subscribers           []stateSubscriber
	subscribersLock       sync.Mutex
}

func New(log logrus.FieldLogger) *RealDevice {
	d := &RealDevice{
		log:         log,
		subscribers: make([]stateSubscriber, 0),
	}
	return d
}

func (d *RealDevice) SendParams(params core.SliderParams) error {
	message, err := params.Bytes()
	if err != nil {
		return nil
	}

	d.log.Debugf("sending %d bytes", len(message))

	_, err = d.programCharacteristic.WriteWithoutResponse(message)
	if err != nil {
		return err
	}
	time.Sleep(time.Second)

	return nil
}

func (d *RealDevice) newStateData(buf []byte) {
	d.log.Debugf("Received %d state bytes", len(buf))
	state, err := core.SliderStateFromBytes(buf)
	d.subscribersLock.Lock()
	for _, s := range d.subscribers {
		if err != nil {
			s.err <- err
		} else {
			s.state <- state
		}
	}
	d.subscribersLock.Unlock()
}

func (d *RealDevice) SubscribeToState() (chan core.SliderState, chan error) {
	s := stateSubscriber{
		state: make(chan core.SliderState),
		err:   make(chan error),
	}
	d.subscribersLock.Lock()
	d.subscribers = append(d.subscribers, s)
	d.subscribersLock.Unlock()
	return s.state, s.err
}

func (d *RealDevice) Connect() chan error {
	ch := make(chan error)
	go func() {
		d.log.Debug("Connecting")

		adapter := bluetooth.DefaultAdapter
		err := adapter.Enable()
		if err != nil {
			ch <- err
			return
		}

		scanResultChan := make(chan bluetooth.ScanResult)

		serviceUUID, err := bluetooth.ParseUUID(serviceUUIDStr)
		if err != nil {
			ch <- err
			return
		}

		programCharacteristicUUID, err := bluetooth.ParseUUID(programCharacteristicUUIDStr)
		if err != nil {
			ch <- err
			return
		}

		stateCharacteristicUUID, err := bluetooth.ParseUUID(stateCharacteristicUUIDStr)
		if err != nil {
			ch <- err
			return
		}

		err = adapter.Scan(func(adapter *bluetooth.Adapter, device bluetooth.ScanResult) {
			if device.LocalName() != "" {
				d.log.Debug(device.LocalName())
			}
			if device.LocalName() == "Camera Slider" && device.HasServiceUUID(serviceUUID) {
				d.log.Debug("Found device")
				adapter.StopScan()
				scanResultChan <- device
			}
		})
		if err != nil {
			ch <- err
			return
		}

		scanResult := <-scanResultChan
		device, err := adapter.Connect(scanResult.Address, bluetooth.ConnectionParams{})
		if err != nil {
			ch <- err
			return
		}

		d.log.Debug("Connected to device")

		srvcs, err := device.DiscoverServices([]bluetooth.UUID{serviceUUID})
		if err != nil {
			ch <- err
			return
		}

		if len(srvcs) != 1 {
			ch <- errors.New("service not found")
			return
		}

		d.log.Debug("Loaded service")

		chars, err := srvcs[0].DiscoverCharacteristics([]bluetooth.UUID{programCharacteristicUUID})
		if err != nil {
			ch <- err
			return
		}

		if len(chars) != 1 {
			ch <- errors.New("program characteristic not found")
			return
		}

		d.characteristicLock.Lock()

		d.programCharacteristic = &chars[0]

		chars, err = srvcs[0].DiscoverCharacteristics([]bluetooth.UUID{stateCharacteristicUUID})
		if err != nil {
			ch <- err
			return
		}

		if len(chars) != 1 {
			ch <- errors.New("program characteristic not found")
			return
		}

		d.stateCharacteristic = &chars[0]
		d.stateCharacteristic.EnableNotifications(d.newStateData)

		d.characteristicLock.Unlock()

		d.log.Debug("Loaded characteristics")

		d.log.Debug("Done connecting")

		ch <- nil
	}()
	return ch
}
