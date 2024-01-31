package core

type Device interface {
	SendParams(params SliderParams) error
	GetStatus() (SliderStatus, error)
	Connected() bool
	Connect() error
}
