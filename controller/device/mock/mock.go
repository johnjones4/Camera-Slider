package mock

import (
	"main/core"
	"time"
)

type MockDevice struct {
	state    core.SliderState
	lastSend time.Time
}

func New() *MockDevice {
	d := &MockDevice{
		state: core.SliderState{
			Mode: core.SliderModeIdle,
		},
	}
	return d
}

func (d *MockDevice) SendParams(params core.SliderParams) error {
	d.state.Params = params
	d.lastSend = time.Now()
	d.state.ActiveProgram = true
	d.state.Mode = core.SliderModeProgram
	return nil
}

func (d *MockDevice) SubscribeToState() (chan core.SliderState, chan error) {
	stateChan := make(chan core.SliderState)
	errChan := make(chan error)
	go func() {
		ticker := time.NewTicker(time.Millisecond * 100)
		for {
			<-ticker.C
			limit := time.Duration(0.6/d.state.Params.TrackingMps) * time.Second
			elapsed := time.Since(d.lastSend)
			d.state.PercentComplete = float32(elapsed) / float32(limit)
			if d.state.PercentComplete >= 1 {
				d.state.Params = core.SliderParams{}
				d.state.ActiveProgram = false
				d.state.Mode = core.SliderModeIdle
				d.state.PercentComplete = 0
				d.state.LastProgramTime = int32(elapsed.Milliseconds())
			}
			stateChan <- d.state
		}
	}()
	return stateChan, errChan
}

func (d *MockDevice) Connect() chan error {
	ch := make(chan error)
	go func() {
		time.Sleep(time.Second * 5)
		ch <- nil
	}()
	return ch
}
