const char webpage[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
    <head>
        <title>ESP32 WebRadio</title>
        <meta charset="UTF-8">
        <meta name="color-scheme" content="dark light">
        <meta name="viewport" content="width=device-width, initial-scale=1.0, user-scalable=no"/>
        <script>
function changeChannel(element, fieldName) {
    var urlField = document.getElementById(fieldName);

    if(element.value == 'custom') {
        urlField.style.display = 'flex';
    }
    else {
        urlField.style.display = 'none';
        runCommand('POST', '/channel', {url: element.value});
    }
}

function startAlarm(button) {
    var form = document.getElementById('alarm-form');

    if(button.innerHTML == 'Start') {
        var data = {
            url: form['channel'].value,
            time: form['time'].value + 'Z',
            snooze: parseInt(form['snooze'].value)
        };
        runCommand('POST', '/alarm', data);
        button.innerHTML = 'Snooze';
    }
    else if(button.innerHTML == 'Snooze') {
        runCommand('POST', '/snooze');
    }
}

function stopAlarm(button) {
    var start = document.getElementById('alarm-form')['start'];

    start.innerHTML = 'Start';
    runCommand('POST', '/stop-alarm');
}

function runCommand(method, endpoint, params, callback) {
    var url = new URL(endpoint, window.location.href);
    for (var key in params) {
        url.searchParams.set(key, params[key]);
    }

    var request = new XMLHttpRequest();
    request.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200 && callback) {
            callback(this.responseText);
        }
    };
    request.ontimeout = function() {
        console.log('[Error] Timeout');
    };
    request.open(method, url, true);
    request.send();
}
        </script>
        <style>
@media (prefers-color-scheme: light) {
    body {
        --dark-background: #f7f7f7;
        --background: #ffffff;
        --light-background: #f0f0f0;
        --highlight-background: #e0e0e0;
        --accent-color: #40AA66;
        --highlight-accent-color: #40CC77;
        --alert-color: #A00000;
        --dark-text-color: #404040;
        --text-color: #000000;
        --highlight-text-color: #ffffff;
        --border-radius: 15px;
    }
}

@media (prefers-color-scheme: dark) {
    body {
        --dark-background: #171717;
        --background: #272727;
        --light-background: #373737;
        --highlight-background: #474747;
        --accent-color: #307744;
        --highlight-accent-color: #309955;
        --alert-color: #A00000;
        --dark-text-color: #d0d0d0;
        --text-color: #ffffff;
        --highlight-text-color: #ffffff;
        --border-radius: 15px;
    }
}

* {
    box-sizing: border-box;
    margin: 0;
    padding: 0;
}

::placeholder {
    color: var(--dark-text-color);
    font-weight: normal;
    opacity: 1;
}

:-ms-input-placeholder {
    color: var(--dark-text-color);
    font-weight: normal;
}

::-webkit-input-placeholder {
    color: var(--dark-text-color);
    font-weight: normal;
}

a {
    color: inherit;
    cursor: pointer;
    text-decoration: none;
}

a:hover {
    color: inherit;
    outline: none;
}

button, input[type=submit], input[type=reset] {
    background-color: var(--accent-color);
    border: none;
    border-radius: var(--border-radius);
    color: var(--highlight-text-color);
    cursor: pointer;
    font-size: 18px;
    margin: 5px 0;
    padding: 8px;
    transition: background-color 0.3s;
    width: 100%;
}

button:hover, input[type=submit]:hover, input[type=reset]:hover {
    background-color: var(--highlight-accent-color);
    outline: none;
}

body {
    background-color: var(--dark-background);
    color: var(--text-color);
    font-family: Helvetica, Arial, sans-serif;
    margin: 0;
    padding: 0;
    transition: background-color,color 0.3s;
}

br {
    height: 2px;
    margin: 10px 0;
}

h1 {
    font-size: 30px;
    font-weight: normal;
    margin: 2px 10px;
    text-align: left;
}

h2 {
    font-size: 26px;
    font-weight: 500;
}

header {
    background-color: var(--background);
    box-shadow: 0 1px 4px 0 rgba(0, 0, 0, 0.15);
    height: 45px;
    overflow: hidden;
    position: fixed;
    top: 0;
    text-align: center;
    width: 100%;
    z-index: 2;
}

hr {
    background-color: var(--light-background);
    border: none;
    height: 2px;
    margin: 10px 0;
}

input[type=text], input[type=password], input[type=number],
input[type=date], input[type=datetime-local],
select {
    background-color: var(--light-background);
    border: none;
    border-radius: var(--border-radius);
    color: var(--text-color);
    font-family: inherit;
    font-size: 18px;
    margin: 5px 0;
    padding: 8px;
    width: 100%;
}

input[type=text]:focus, input[type=password]:focus, input[type=number]:focus,
input[type=date]:focus, input[type=datetime-local]:focus,
input[type=text]:hover, input[type=password]:hover, input[type=number]:hover,
input[type=date]:hover, input[type=datetime-local]:hover,
select:focus, select:hover {
    background-color: var(--highlight-background);
    outline: none;
}

input[type=range] {
    appearance: none;
    background-color: var(--highlight-background);
    border-radius: 4px;
    height: 8px;
    margin: 10px 0;
    outline: none;
}

input[type=range]::-webkit-slider-thumb {
  appearance: none;
  border-radius: 50%;
  background-color: var(--accent-color);
  cursor: pointer;
  height: 25px;
  width: 25px;
}

input[type=range]::-webkit-slider-thumb:hover {
    background-color: var(--highlight-accent-color);
}

input[type=range]::-moz-range-thumb {
    border: none;
    border-radius: 50%;
    background-color: var(--accent-color);
    cursor: pointer;
    height: 25px;
    width: 25px;
}

input[type=range]::-moz-range-thumb:hover {
    background-color: var(--highlight-accent-color);
}

label {
    color: var(--dark-text-color);
    font-size: 16px;
    width: 100%;
}

option {
    background-color: var(--light-background);
    border: none;
    color: var(--text-color);
    cursor: pointer;
    outline: none;
}

option:active, option:hover {
    background-color: var(--highlight-background);
    border: none;
    color: var(--text-color);
    outline: none;
}

select {
    cursor: pointer;
}

.card {
    background-color: var(--background);
    border-radius: var(--border-radius);
    display: flex;
    flex-direction: column;
    margin: 15px auto;
    max-width: 1200px;
    padding: 20px;
    width: 100%;
}

.content {
    background-color: var(--dark-background);
    display: block;
    height: 100%;
    padding: 45px 15px 0;
    width: 100%;
}

.column {
    display: flex;
    flex-direction: column;
}

.flex {
    flex: 100%;
}

.hidden {
    display: none;
}

.row {
    display: flex;
    flex-direction: row;
}

.separator {
    height: 100%;
    width: 20px;
}
        </style>
    </head>
    <body>
        <header class="row">
            <h1 class="flex">ESP32 WebRadio</h1>
        </header>
        <div class="content">
            <div class="card">
                <form id="settings-form" class="column" onsubmit="return false;">
                    <h2>Radio Settings</h2>
                    <hr>
                    <label for="channel">Radio Channel:</label>
                    <select name="channel" onchange="changeChannel(this, 'radio-url');">
                        <option value="http://liveradio.swr.de/sw282p3/swr1bw">SWR1</option>
                        <option value="http://liveradio.swr.de/sw282p3/swr3" selected>SWR3</option>
                        <option value="http://liveradio.swr.de/sw282p3/swr4bw">SWR4</option>
                        <option value="http://stream.antenne1.de/a1stg/livestream2.mp3">Antenne 1</option>
                        <option value="http://stream.antenne.de/antenne/stream/mp3">Antenne Bayern</option>
                        <option value="http://streams.bigfm.de/bigfm-deutschland-128-mp3">BigFM</option>
                        <option value="https://liveradio.swr.de/d9zadj3/dasding/">Dasding</option>
                        <option value="https://streams.br.de/bayern3_2.m3u">Bayern 3</option>
                        <option value="https://stream.srg-ssr.ch/rsp/mp3_128.m3u">Swiss Pop</option>
                        <option value="https://streams.80s80s.de/web/mp3-128/streams.80s80s.de/">80s 80s</option>
                        <option value="http://nrj.de/stuttgart">Energy Stuttgart</option>
                        <option value="http://addrad.io/4454xb3">107.7</option>
                        <option value="custom">Custom</option>
                    </select>
                    <div class="column hidden" id="radio-url">
                        <label for="url">Streaming URL:</label>
                        <input type="text" name="url" placeholder="Custom URL" onchange="runCommand('POST', '/channel', {url: this.value});">
                    </div>
                    <br>
                    <div class="column">
                        <label for="volume">Volume:</label>
                        <input type="range" name="volume" min="0" step="1" max="100" value="80" onchange="runCommand('POST', '/volume', {volume: parseInt(this.value)});">
                    </div>
                    <br>
                    <hr>
                    <div class="row">
                        <button onclick="runCommand('POST', '/stop');">Stop</button>
                        <div class="separator"></div>
                        <button onclick="runCommand('POST', '/play');">Play</button>
                    </div>
                </form>
            </div>
            <div class="card">
                <form id="alarm-form" class="column" onsubmit="return false">
                    <h2>Alarm Clock</h2>
                    <hr>
                    <label for="channel">Radio Channel:</label>
                    <select name="channel">
                        <option value="http://liveradio.swr.de/sw282p3/swr1bw">SWR1</option>
                        <option value="http://liveradio.swr.de/sw282p3/swr3" selected>SWR3</option>
                        <option value="http://liveradio.swr.de/sw282p3/swr4bw">SWR4</option>
                        <option value="http://stream.antenne1.de/a1stg/livestream2.mp3">Antenne 1</option>
                        <option value="http://stream.antenne.de/antenne/stream/mp3">Antenne Bayern</option>
                        <option value="http://streams.bigfm.de/bigfm-deutschland-128-mp3">BigFM</option>
                        <option value="https://liveradio.swr.de/d9zadj3/dasding/">Dasding</option>
                        <option value="https://streams.br.de/bayern3_2.m3u">Bayern 3</option>
                        <option value="https://stream.srg-ssr.ch/rsp/mp3_128.m3u">Swiss Pop</option>
                        <option value="https://streams.80s80s.de/web/mp3-128/streams.80s80s.de/">80s 80s</option>
                        <option value="http://nrj.de/stuttgart">Energy Stuttgart</option>
                        <option value="http://addrad.io/4454xb3">107.7</option>
                    </select>
                    <br>
                    <div class="column">
                        <label for="time">Alarm Time:</label>
                        <input type="datetime-local" name="time" placeholder="Alarm">
                    </div>
                    <div class="column">
                        <label for="snooze">Snooze Time:</label>
                        <input type="number" name="snooze" min="0" step="1" max="20" value="10" placeholder="Minutes">
                    </div>
                    <hr>
                    <div class="row">
                        <button name="stop" onclick="stopAlarm(this);">Stop</button>
                        <div class="separator"></div>
                        <button name="start" onclick="startAlarm(this);">Start</button>
                    </div>
                </form>
            </div>
        </div>
    </body>
</html>
)=====";
