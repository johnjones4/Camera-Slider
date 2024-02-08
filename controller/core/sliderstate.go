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
	Params             SliderParams
	ActiveProgram      bool
	Mode               SliderMode
	PercentComplete    float32
	LastProgramTime    int32
	LastEffectiveSpeed SliderSpeed
}

func SliderStateFromBytes(message []byte) (SliderState, error) {
	var s SliderState
	buffer := bytes.NewBuffer(message)

	err := binary.Read(buffer, binary.LittleEndian, &s.Params.Speed.TrackingMps)
	if err != nil {
		return SliderState{}, err
	}

	err = binary.Read(buffer, binary.LittleEndian, &s.Params.Speed.PanningRpm)
	if err != nil {
		return SliderState{}, err
	}

	var rd uint32 = 0
	err = binary.Read(buffer, binary.LittleEndian, &rd)
	if err != nil {
		return SliderState{}, err
	}
	if rd == 1 {
		s.Params.RotationDirection = true
	} else {
		s.Params.RotationDirection = false
	}

	err = binary.Read(buffer, binary.LittleEndian, &s.Params.PercentDistance)
	if err != nil {
		return SliderState{}, err
	}

	var ap uint32 = 0
	err = binary.Read(buffer, binary.LittleEndian, &ap)
	if err != nil {
		return SliderState{}, err
	}
	if ap == 1 {
		s.ActiveProgram = true
	} else {
		s.ActiveProgram = false
	}

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

	err = binary.Read(buffer, binary.LittleEndian, &s.LastEffectiveSpeed.TrackingMps)
	if err != nil {
		return SliderState{}, err
	}

	err = binary.Read(buffer, binary.LittleEndian, &s.LastEffectiveSpeed.PanningRpm)
	if err != nil {
		return SliderState{}, err
	}

	return s, nil
}
