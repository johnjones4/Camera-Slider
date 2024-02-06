package core

type Device interface {
	SendParams(params SliderParams) error
	SubscribeToState() (chan SliderState, chan error)
	Connect() chan error
}
