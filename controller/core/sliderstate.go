package core

import (
	"bytes"
	"encoding/binary"
)

type SliderMode int32

const (
	SliderModeHoming  SliderMode = 0
	SliderModeIdle    SliderMode = 1
	SliderModeProgram SliderMode = 2
)

type SliderState struct {
	Params                     SliderParams
	ActiveProgram              bool
	Mode                       SliderMode
	PercentComplete            float32
	LastProgramTime            int32
	LastEffectiveTrackingSpeed float32
}

func SliderStateFromBytes(message []byte) (SliderState, error) {
	var s SliderState
	buffer := bytes.NewBuffer(message)

	err := binary.Read(buffer, binary.LittleEndian, &s.Params.TrackingMps)
	if err != nil {
		return SliderState{}, err
	}

	err = binary.Read(buffer, binary.LittleEndian, &s.Params.PanningRpm)
	if err != nil {
		return SliderState{}, err
	}

	var activeProgram uint32
	err = binary.Read(buffer, binary.LittleEndian, &activeProgram)
	if err != nil {
		return SliderState{}, err
	}
	s.ActiveProgram = activeProgram == 1

	var mode int32
	err = binary.Read(buffer, binary.LittleEndian, &mode)
	if err != nil {
		return SliderState{}, err
	}
	s.Mode = SliderMode(mode)

	err = binary.Read(buffer, binary.LittleEndian, &s.PercentComplete)
	if err != nil {
		return SliderState{}, err
	}

	err = binary.Read(buffer, binary.LittleEndian, &s.LastProgramTime)
	if err != nil {
		return SliderState{}, err
	}

	err = binary.Read(buffer, binary.LittleEndian, &s.LastEffectiveTrackingSpeed)
	if err != nil {
		return SliderState{}, err
	}

	return s, nil
}
