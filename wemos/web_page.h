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

    button, input {
        padding: 10px 20px;
        margin-top: 20px;
        font-size: 14px;
        cursor: pointer;
        border: none;
        border-radius: 4px;
        background-color: #03A062;
        color: white;
        transition: background-color 0.3s;
    }

    .controller-button, .log-button {
      margin-top: 0px;
      margin-bottom: 10px;
      padding: 10px 20px;
      font-size: 16px;
      cursor: pointer;
      border: 1px solid #03A062;
      background-color: transparent;
      width: 50%;
    }

    .controller, .log-messages {
        flex: 1;
        overflow-y: auto;
        border: 1px solid #03A062;
        border-radius: 8px;
        padding: 10px;
        background-color: #0D0208;
        font-size: 12px;
        color: #03A062;
    }

    .controller *, .log-messages * {
        margin: 0px;
    }

    .hiden {
        display: none;
    }

    .button-active {
      background-color: #03A062;
    }

    .buttons-div {
      display: flex;
    }

    .status-indicator {
        padding: 10px 20px;
        cursor: pointer;
        border-radius: 4px;
        background-color: #4c4c4c;
        color: white;
        transition: background-color 0.3s;
    }

    .status-on {
        background-color: #03A062;
    }

    .status-off {
        background-color: #bd0000;
    }

    .controls, .bottom {
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

    </style>
</head>
<body>

    <div class="controls">
        <h1>Wemos Toolbox</h1>
        <button id="toggleIR" class="status-indicator">Toggle IR</button>
        <div>
            <form id="sendIRForm">
            <button id="sendIRBtn" type="submit">Send IR</button>
            <input type="text" name="protocol" placeholder="protocol" id="protocolIR">
            <input type="text" name="address" placeholder="address" id="addressIR">
            <input type="text" name="command" placeholder="command" id="commandIR">
            <input type="text" name="repeats" placeholder="repeats" id="repeatsIR">
            </form>
        </div>
    </div>

    <div class="bottom">
        <div class="buttons-div">
            <button class="log-button button-active" id="log-button">Logs</button>
            <button class="controller-button" id="controller-button">Controller</button>
        </div>
        <div class="controller hiden" id="controller"></div>
        <div class="log-messages" id="log-messages"></div>
    </div>

    <script>
        const toggleIRBtn = document.getElementById('toggleIR');

        const repeatSignalBtn = document.getElementById('repeat-signal');

        const controllerBtn = document.getElementById('controller-button');
        const controller = document.getElementById('controller');

        const sendIRBtn = document.getElementById('sendIRBtn');
        const sendIRForm = document.getElementById('sendIRForm');

        const logMessagesBtn = document.getElementById('log-button');
        const logMessages = document.getElementById('log-messages');

        function updateLog() {
          fetch('/log')
                .then(response => response.text())
                .then(data => {
                    console.log(data);
                    logMessages.innerHTML = '';

                    data.split('\n').forEach(line => {
                        if (line.trim() !== '') {
                            logMessages.innerHTML += `<p>${line}</p>`;
                        }
                    });
                    logMessages.scrollTop = logMessages.scrollHeight;
                })
                .catch(function(error) {
                    console.error('Could not connect to device.', 'error');
                });
        }

        function updateStatus(data) {
            if (data.IRReceiveEnabled) {
                toggleIRBtn.classList.remove('status-off');
                toggleIRBtn.classList.add('status-on');
            } else {
                toggleIRBtn.classList.remove('status-on');
                toggleIRBtn.classList.add('status-off');
            }
        }

        logMessagesBtn.addEventListener('click', function() {
          controller.classList.add("hiden");
          logMessages.classList.remove("hiden");
          controllerBtn.classList.remove("button-active")
          logMessagesBtn.classList.add("button-active");

          updateLog();
        });

        controllerBtn.addEventListener('click', function() {
          logMessages.classList.add("hiden");
          controller.classList.remove("hiden");
          logMessagesBtn.classList.remove("button-active");
          controllerBtn.classList.add("button-active");
        });

        toggleIRBtn.addEventListener('click', function() {
            fetch('/toggleIR')
                .then(response => response.text())
                .then(text => {
                    updateLog(text)

                    return fetch('/status');
                })
                .then(response => response.json())
                .then(data => {
                    updateStatus(data);
                })
                .catch(error => {
                    console.error('Error: ' + error, 'error');
                });
        });

        sendIRForm.addEventListener('submit', function(e) {
            e.preventDefault();

            fetch('/sendIR', {
                method: 'POST',
                body: new FormData(sendIRForm)
            })

            updateLog();
        });

        setInterval(function() {
            fetch('/status')
                .then(function(response) { return response.json(); })
                .then(function(data) {
                    updateStatus(data);
                })
                .catch(function(error) {
                    console.error('Status error:', error);
                });
        }, 3000);

        window.addEventListener('load', function() {
            fetch('/status')
                .then(function(response) { return response.json(); })
                .then(function(data) {
                    console.log('Connected to device!', 'success');
                    updateStatus(data);
                })
                .catch(function(error) {
                    console.error('Could not connect to device.', 'error');
                });
        });
    </script>
</body>
</html>


)=====";

#endif