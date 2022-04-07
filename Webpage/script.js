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