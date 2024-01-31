package mock

import (
	"log"
	"main/core"
	"sync/atomic"
	"time"
)

type MockDevice struct {
	status     core.SliderStatus
	lastSend   time.Time
	connecting atomic.Bool
	connected  atomic.Bool
}

func New() *MockDevice {
	d := &MockDevice{
		status: core.SliderStatus{
			State: core.SliderStateIdle,
		},
	}
	return d
}

func (d *MockDevice) SendParams(params core.SliderParams) error {
	d.status.Params = params
	d.lastSend = time.Now()
	d.status.ActiveProgram = true
	d.status.State = core.SliderStateProgram
	return nil
}

func (d *MockDevice) GetStatus() (core.SliderStatus, error) {
	if d.status.Params.TrackingMps > 0 {
		limit := time.Duration(1.0/d.status.Params.TrackingMps) * time.Second
		elapsed := time.Since(d.lastSend)
		d.status.PercentComplete = float32(elapsed) / float32(limit)
		log.Println(elapsed, limit, d.status.PercentComplete)
		if d.status.PercentComplete >= 1 {
			d.status.Params = core.SliderParams{}
			d.status.ActiveProgram = false
			d.status.State = core.SliderStateIdle
			d.status.PercentComplete = 0
		}
	}
	return d.status, nil
}

func (d *MockDevice) Connect() error {
	if !d.connected.Load() {
		return nil
	}
	if d.connecting.Swap(true) {
		return nil
	}
	time.Sleep(time.Second * 5)
	d.connecting.Store(false)
	d.connected.Store(true)
	return nil
}

func (d *MockDevice) Connected() bool {
	return d.connected.Load()
}
