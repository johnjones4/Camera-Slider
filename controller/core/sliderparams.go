package core

import (
	"bytes"
	"encoding/binary"
)

type SliderParams struct {
	TrackingMps float32 `json:"trackingMps"`
	PanningRpm  float32 `json:"panningRpm"`
}

func (p SliderParams) Bytes() ([]byte, error) {
	var buf bytes.Buffer

	buf.WriteByte(0xA4)

	err := binary.Write(&buf, binary.LittleEndian, p.TrackingMps)
	if err != nil {
		return nil, err
	}

	err = binary.Write(&buf, binary.LittleEndian, p.PanningRpm)
	if err != nil {
		return nil, err
	}

	return buf.Bytes(), nil
}
