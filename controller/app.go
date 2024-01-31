package main

import (
	"context"
	"log"
	"main/core"
	"main/device/mock"
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
	a.device = mock.New()
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
