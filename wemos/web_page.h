#ifndef WEB_PAGE_H
#define WEB_PAGE_H

const char index_html[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Wemos Box</title>
    <style>
    html, body {
        font-family: 'Courier New', monospace;
        background-color: #0D0208;
        height: 100%;
        margin: 0;
        overflow: hidden;
    }

    body {
        display: flex;
        flex-direction: column;
    }

    h1 {
        color: #03A062;
    }
    h2 {
        color: #03A062;
        margin-top: 20px;
    }
    button {
        padding: 10px 20px;
        margin: 5px;
        font-size: 14px;
        cursor: pointer;
        border: none;
        border-radius: 4px;
        background-color: #4CAF50;
        color: white;
        transition: background-color 0.3s;
    }
    button:hover {
        background-color: #45a049;
    }
    button:active {
        background-color: #3d8b40;
    }
    .controls, .logs {
        flex: 1;
        min-height: 0;
        border-style: solid;
        border-color: #02683f;
        background-color: #0D0208;
        padding: 20px;
        border-radius: 8px;
        display: flex;
        flex-direction: column;
    }

    .log-messages {
        flex: 1;
        overflow-y: auto;
        border: 1px solid #03A062;
        border-radius: 8px;
        padding: 10px;
        background-color: #0D0208;
        font-size: 12px;
        color: #03A062;
    }
    </style>
</head>
<body>

    <div class="controls">
        <h1>Wemos Box</h1>

        <button id="start-scan">Start Scan</button>
        <button id="stop-scan">Stop Scan</button>
        <button id="repeat-signal">Repeat Last Signal</button>
    </div>

    <div class="logs">
        <h1>Logs</h1>
        <div class="log-messages" id="log-messages">
            <p>No logs yet. Waiting for commands...</p>
        </div>
    </div>

    <script>
        const startScanBtn = document.getElementById('start-scan');
        const stopScanBtn = document.getElementById('stop-scan');
        const repeatSignalBtn = document.getElementById('repeat-signal');
        const logMessages = document.getElementById('log-messages');

        function addLog(message, type) {
            type = type || 'info';
            const timestamp = new Date().toLocaleTimeString();
            let color = '';
            if (type === 'error') color = 'color: red;';
            else if (type === 'success') color = 'color: #03A062;';
            logMessages.innerHTML += '<p style="' + color + '">[' + timestamp + '] ' + message + '</p>';
            logMessages.scrollTop = logMessages.scrollHeight;
        }

        startScanBtn.addEventListener('click', function() {
            addLog('Starting IR scan...');
            fetch('/toggleIR')
                .then(function(response) { return response.text(); })
                .then(function(text) {
                    addLog(text, 'success');
                })
                .catch(function(error) {
                    addLog('Error: ' + error, 'error');
                });
        });

        stopScanBtn.addEventListener('click', function() {
            addLog('Stopping IR scan...');
            fetch('/toggleIR')
                .then(function(response) { return response.text(); })
                .then(function(text) {
                    addLog(text, 'success');
                })
                .catch(function(error) {
                    addLog('Error: ' + error, 'error');
                });
        });

        repeatSignalBtn.addEventListener('click', function() {
            addLog('Attempting to repeat last IR signal...');
            fetch('/repeatSignal')
                .then(function(response) { return response.text(); })
                .then(function(text) {
                    addLog(text, text.indexOf('✓') >= 0 ? 'success' : 'error');
                })
                .catch(function(error) {
                    addLog('Error: ' + error, 'error');
                });
        });

        setInterval(function() {
            fetch('/status')
                .then(function(response) { return response.json(); })
                .then(function(data) {
                    console.log('Status:', data);
                })
                .catch(function(error) {
                    console.error('Status error:', error);
                });
        }, 3000);

        window.addEventListener('load', function() {
            addLog('Web interface loaded.');
            fetch('/status')
                .then(function(response) { return response.json(); })
                .then(function(data) {
                    addLog('Connected to device!', 'success');
                })
                .catch(function(error) {
                    addLog('Could not connect to device.', 'error');
                });
        });
    </script>
</body>
</html>

)=====";

#endif