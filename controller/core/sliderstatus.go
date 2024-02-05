package core

import (
	"bytes"
	"encoding/binary"
)

type SliderState int32

const (
	SliderStateHoming  = 0
	SliderStateIdle    = 1
	SliderStateProgram = 2
)

type SliderStatus struct {
	Params          SliderParams `json:"params"`
	ActiveProgram   bool         `json:"activeProgram"`
	State           SliderState  `json:"state"`
	PercentComplete float32      `json:"percentComplete"`
}

func SliderStatusFromBytes(message []byte) (SliderStatus, error) {
	var s SliderStatus
	buffer := bytes.NewBuffer(message)

	err := binary.Read(buffer, binary.LittleEndian, &s.Params.TrackingMps)
	if err != nil {
		return SliderStatus{}, err
	}

	err = binary.Read(buffer, binary.LittleEndian, &s.Params.PanningRpm)
	if err != nil {
		return SliderStatus{}, err
	}

	var activeProgram uint32
	err = binary.Read(buffer, binary.LittleEndian, &activeProgram)
	if err != nil {
		return SliderStatus{}, err
	}
	s.ActiveProgram = activeProgram == 1

	var state int32
	err = binary.Read(buffer, binary.LittleEndian, &state)
	if err != nil {
		return SliderStatus{}, err
	}
	s.State = SliderState(state)

	err = binary.Read(buffer, binary.LittleEndian, &s.PercentComplete)
	if err != nil {
		return SliderStatus{}, err
	}

	return s, nil
}
