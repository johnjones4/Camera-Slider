package core

import (
	"bytes"
	"encoding/binary"
)

type SliderSpeed struct {
	TrackingMps float32
	PanningRpm  float32
}

type SliderParams struct {
	Speed             SliderSpeed
	RotationDirection bool
	PercentDistance   float32
}

func (p SliderParams) Bytes() ([]byte, error) {
	var buf bytes.Buffer

	buf.WriteByte(0xA4)

	err := binary.Write(&buf, binary.LittleEndian, p.Speed.TrackingMps)
	if err != nil {
		return nil, err
	}

	err = binary.Write(&buf, binary.LittleEndian, p.Speed.PanningRpm)
	if err != nil {
		return nil, err
	}

	err = binary.Write(&buf, binary.LittleEndian, p.PercentDistance)
	if err != nil {
		return nil, err
	}

	var d uint32 = 0
	if p.RotationDirection {
		d = 1
	}
	err = binary.Write(&buf, binary.LittleEndian, d)
	if err != nil {
		return nil, err
	}

	return buf.Bytes(), nil
}
