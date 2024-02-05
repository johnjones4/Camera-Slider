package main

import (
	"context"
	"log"
	"main/core"
	"main/device/mock"
	"main/device/real"
	"os"
)

// App struct
type App struct {
	ctx    context.Context
	device core.Device
}

// NewApp creates a new App application struct
func NewApp() *App {
	return &App{}
}

// startup is called when the app starts. The context is saved
// so we can call the runtime methods
func (a *App) startup(ctx context.Context) {
	if os.Getenv("MOCK") != "" {
		log.Println("Using mock device")
		a.device = mock.New()
	} else {
		log.Println("Using real device")
		a.device = real.New()
	}
	a.ctx = ctx
}

func (a *App) Connect() error {
	return a.device.Connect()
}

func (a *App) Connected() bool {
	return a.device.Connected()
}

func (a *App) SendParams(params core.SliderParams) error {
	log.Println(params)
	return a.device.SendParams(params)
}

func (a *App) GetStatus() (core.SliderStatus, error) {
	status, err := a.device.GetStatus()
	log.Println(status, err)
	return status, err
}
