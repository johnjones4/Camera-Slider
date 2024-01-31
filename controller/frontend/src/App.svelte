<script lang="ts">
  import camera from './assets/images/camera.svg'
  import {Connected, Connect, SendParams, GetStatus} from '../wailsjs/go/main/App.js'
  import {core} from '../wailsjs/go/models';

  const modeMap = {
    0: 'Homing',
    1: 'Idle',
    2: 'Program'
  }

  let tracking: number = 0.01
  let panning: number = 0.01
  let programRunning: boolean
  let pcnt: number
  let connecting: boolean
  let mode = 0

  $: formEnabled = !programRunning && !connecting
  $: submitEnabled = !programRunning && !connecting && tracking > 0 && panning > 0

  function updateConnectionStatus() {
    connecting = true
    programRunning = false
    Connected()
      .then(connected => {
        if (connected) {
          connecting = false
        }
        return Connect()
      })
      .then(() => {
        connecting = false
        updateStatus()
      })
      .catch(e => alert(e))
  }

  function updateStatus() {
    console.log("getting status")
    GetStatus()
      .then(status => {
        console.log(status)
        programRunning = status.activeProgram
        pcnt = status.percentComplete
        mode = status.state
        if (status.state === 0 || status.state === 2) {
          setTimeout(() => updateStatus(), 100)
        }
      })
      .catch(e => alert(e))
  }

  function formSubmitted(e: Event): boolean {
    e.preventDefault()
    if (tracking > 0 && panning > 0) {
      const payload = new core.SliderParams({
        trackingMps: tracking as number,
        panningRpm: panning as number
      })
      SendParams(payload)
        .then(() => updateStatus())
        .catch(e => alert(e))
    }
    return false
  }

  updateConnectionStatus()
</script>

<main>
  <div class={["status", programRunning ? 'enabled' : 'disabled'].join(' ')}>
    <div class="slider">
      <img class="camera" style={`left: ${pcnt * 100}%`} src={camera} alt="Camera" />
    </div>
    <div class="pcnt">
      {`${(pcnt * 100).toFixed(0)}%`} | {modeMap[mode]}
    </div>
  </div>
  <div class={["controls", formEnabled ? 'enabled' : 'disabled'].join(' ')}>
    <form on:submit={formSubmitted}>
      <div class="fields">
        <div class="input-group">
          <label for="tracking">Tracking Speed</label>
          <input bind:value={tracking} type="number" step="0.0001" id="tracking" class="input" disabled={!formEnabled} />
        </div>
        <div class="input-group">
          <label for="panning">Panning Speed</label>
          <input bind:value={panning} type="number" step="0.0001"  id="panning" class="input" disabled={!formEnabled} />
        </div>
      </div>
      <button class="start" disabled={!submitEnabled}>Start</button>
    </form>
  </div>
  {#if connecting}
    <div class="connecting">
      <div>Connecting ...</div>
    </div>
  {/if}
</main>

<style>
main {
  display: flex;
  flex-direction: column;
  justify-content: center;
  height: 100%;
}

.status.disabled {
  opacity: 0.25;
}

.status .slider {
  position: relative;
  margin: 0 auto;
  border-bottom: solid 1px #C8FF00;
  height: 100px;
  width: 60%;
}

.status .slider .camera {
  width: 100px;
  height: 100px;
  position: absolute;
  transform: translateX(-50%);
}

.status .pcnt {
  padding: 0.5em;
}

.controls {
  margin: 2em auto 0 auto;
  width: 60%;
}

.controls.disabled {
  opacity: 0.25;
}

.controls .fields {
  display: flex;
  flex-direction: row;
}

.controls .fields .input-group {
  flex-grow: 1;
  display: flex;
  flex-direction: column;
  text-align: left;
  margin: 0 1em;
}

.controls .fields .input-group label {
  margin: 0 0 0.5em 0;
}

.controls .fields .input-group input {
  font-size: 18px;
  border-radius: 4px;
  outline: none;
  border: solid 1px #C8FF00;
  background: none;
  color: white;
  height: 2em;
  width: 100%;
  padding: 10px;
}

.controls .start {
  display: block;
  width: 100%;
  margin: 1em 0 0 0;
  background: #C8FF00;
  border: none;
  outline: none;
  border-radius: 0.5em;
  padding: 0.5em 0;
  color: #1C0B19;
  font-size: 1.5em;
}

.controls .start:disabled {
  opacity: 0.5;
}

.connecting {
  position: fixed;
  top: 0;
  bottom: 0;
  left: 0;
  right: 0;
  background-color: rgba(0,0,0,0.75);
  color: white;
  display: flex;
  justify-content: center;
  flex-direction: column;
}
  
</style>
