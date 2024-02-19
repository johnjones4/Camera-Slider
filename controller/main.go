package main

import (
	"flag"
	"main/core"
	"main/device/mock"
	"main/device/real"
	"os"
	"time"

	"github.com/schollz/progressbar/v3"
	"github.com/sirupsen/logrus"
)

func main() {
	trackingSpeed := flag.Float64("t", 0.05, "Tracking speed (MPS)")
	panningSpeed := flag.Float64("p", 0.5, "Panning speed (RPM)")
	direction := flag.Bool("ccw", false, "Direction (true == CCW)")
	trackingDistance := flag.Float64("td", 1, "Tracking distance (0 - 1)")
	useMock := flag.Bool("mock", false, "Mock devie")
	logLevel := flag.String("loglevel", logrus.InfoLevel.String(), "Log level")
	flag.Parse()

	log := logrus.New()

	level, err := logrus.ParseLevel(*logLevel)
	if err != nil {
		log.Panic(err)
	}
	log.SetLevel(level)

	log.Info("Beginning program:")
	log.Infof("Tracking speed: %f mps", *trackingSpeed)
	log.Infof("Rotation speed: %f rpm", *panningSpeed)

	var device core.Device
	if *useMock {
		device = mock.New()
	} else {
		device = real.New(log)
	}

	spinner := progressbar.NewOptions(-1,
		progressbar.OptionSetDescription("Connecting"),
	)

	connectChan := device.Connect()
	ticker := time.NewTicker(time.Millisecond * 100)

connecting:
	for {
		select {
		case err := <-connectChan:
			if err != nil {
				log.Panic(err)
			}
			break connecting
		case <-ticker.C:
			err := spinner.RenderBlank()
			if err != nil {
				log.Panic(err)
			}
		}
	}

	err = device.SendParams(core.SliderParams{
		Speed: core.SliderSpeed{
			TrackingMps: float32(*trackingSpeed),
			PanningRpm:  float32(*panningSpeed),
		},
		RotationDirection: *direction,
		PercentDistance:   float32(*trackingDistance),
	})
	if err != nil {
		log.Panic(err)
	}

	spinner.Clear()

	bar := progressbar.NewOptions(100,
		progressbar.OptionSetDescription("Running"),
	)

	stateChan, errChan := device.SubscribeToState()
	started := false
	for {
		select {
		case state := <-stateChan:
			if !started && state.ActiveProgram {
				started = true
			}
			pcnt := int(100 * state.PercentComplete)
			bar.Set(pcnt)
			if started && !state.ActiveProgram {
				bar.Clear()
				log.Infof("Completed program in %f seconds at %f mps / %f rpm\n", float64(state.LastProgramTime)/1000.0, state.LastEffectiveSpeed.TrackingMps, state.LastEffectiveSpeed.PanningRpm)
				os.Exit(0)
			}
		case err := <-errChan:
			log.Panic(err)
		}
	}
}
