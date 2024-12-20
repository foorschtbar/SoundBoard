// WARNING! Do not modify this file, it is auto-generated from index.html at build!
const char index_html[] PROGMEM = R"=====(<!DOCTYPE HTML>
<html>

<head>
    <title>SoundBoard</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
        html {
            font-family: Arial, Helvetica, sans-serif;
            text-align: center;
        }

        h1 {
            font-size: 1.8rem;
            color: white;
            margin: 10px 0 0 0;
        }

        h2 {
            font-size: 1.5rem;
            font-weight: bold;
            color: #143642;
            margin: 0 0 10px 0;
        }

        h3 {
            font-size: 1.1rem;
            font-weight: bold;
            margin: 10px 0;
            text-align: left;
        }

        .topnav {
            overflow: hidden;
            background-color: #143642;
            position: fixed;
            left: 0;
            right: 0;
            top: 0;
            height: 75px;
        }

        #json_status {
            font-size: 1rem;
            color: #fff;
        }

        .topnav>div>svg {
            padding-right: 5px;
            position: relative;
            top: 3px;
        }

        body {
            margin: 0;
        }

        .content {
            max-width: 600px;
            margin: 0 auto;
            padding-top: 70px;
        }

        .card {
            background-color: #F8F7F9;
            ;
            box-shadow: 2px 2px 12px 1px rgba(140, 140, 140, .5);
            padding: 20px;
            margin: 30px;
            border-radius: 5px;

            &.hidden {
                display: none;
            }

            .hover {
                background-color: #f0f0f0d7;
                cursor: pointer;
            }
        }

        .button {
            padding: 8px 46px;
            font-size: 16px;
            text-align: center;
            outline: none;
            color: #fff;
            background-color: #0f8b8d;
            border: none;
            border-radius: 5px;
            -webkit-touch-callout: none;
            -webkit-user-select: none;
            -khtml-user-select: none;
            -moz-user-select: none;
            -ms-user-select: none;
            user-select: none;
            -webkit-tap-highlight-color: rgba(0, 0, 0, 0);
            margin: 5px 0 0 5px;
        }

        .button:hover {
            background-color: #13adb0
        }

        .button:active {
            background-color: #0f8b8d;
            box-shadow: 2 2px #CDCDCD;
        }

        .button.small {
            padding: 2px 10px;
            font-size: 12px;
        }

        .button.red {
            background-color: #d9534f;
        }

        .overlay {
            position: fixed;
            top: 0;
            left: 0;
            right: 0;
            bottom: 0;
            background-color: rgba(0, 0, 0, 0.5);
            display: none;
            z-index: 1;
            display: block;
        }

        .loader {
            position: fixed;
            top: 50%;
            left: 50%;
            transform: translate(-50%, -50%);
            background-color: #fff;
            padding: 20px;
            border-radius: 10px;
            text-align: center;
            display: block;
            z-index: 2;
        }

        input[type=file] {
            display: none;
        }

        #loader_msg,
        #loader_close {
            padding-top: 20px;
        }

        .row {
            text-align: left;
            border-left: 3px solid #ccc;
            padding: 2px 0 2px 5px;

        }

        label {
            margin-bottom: 2px;
            font-weight: bold;
            display: block;
        }

        input[type=text],
        input[type=password] {
            width: 100%;
            padding: 3px;
            box-sizing: border-box;
        }

        .lds-ring {
            display: inline-block;
            position: relative;
            width: 80px;
            height: 80px;
        }

        .lds-ring div {
            box-sizing: border-box;
            display: block;
            position: absolute;
            width: 64px;
            height: 64px;
            margin: 8px;
            border: 8px solid #000;
            border-radius: 50%;
            animation: lds-ring 1.2s cubic-bezier(0.5, 0, 0.5, 1) infinite;
            border-color: #000 transparent transparent transparent;
        }

        .lds-ring div:nth-child(1) {
            animation-delay: -0.45s;
        }

        .lds-ring div:nth-child(2) {
            animation-delay: -0.3s;
        }

        .lds-ring div:nth-child(3) {
            animation-delay: -0.15s;
        }

        @keyframes lds-ring {
            0% {
                transform: rotate(0deg);
            }

            100% {
                transform: rotate(360deg);
            }
        }

        .table {
            display: table;
            width: 100%;
            text-align: center;
        }

        .table>div {
            display: table-row;
        }

        .table>div>div {
            display: table-cell;
        }

        .table>div>div:first-child {
            font-weight: bold;
            text-align: left;
        }

        .table>div>div:last-child {
            text-align: right;

        }

        footer {

            a,
            a:visited {
                color: #7a7a7a;
            }

            a:hover {
                text-decoration: none;
            }
        }
    </style>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="icon" type="image/svg+xml"
        href="data:image/svg+xml;base64,PD94bWwgdmVyc2lvbj0iMS4wIiA/Pjxzdmcgdmlld0JveD0iMCAwIDUxMiA1MTIiIHhtbG5zPSJodHRwOi8vd3d3LnczLm9yZy8yMDAwL3N2ZyI+PHBhdGggZD0iTTUxMS4xIDM2Ny4xYzAgNDQuMTgtNDIuOTggODAtOTUuMSA4MHMtOTUuMS0zNS44Mi05NS4xLTc5LjFjMC00NC4xOCA0Mi45OC03OS4xIDk1LjEtNzkuMWMxMS4yOCAwIDIxLjk1IDEuOTIgMzIuMDEgNC44OThWMTQ4LjFMMTkyIDIyNGwtLjAwMjMgMjA4LjFDMTkxLjEgNDc2LjIgMTQ5IDUxMiA5NS4xIDUxMlMwIDQ3Ni4yIDAgNDMyYzAtNDQuMTggNDIuOTgtNzkuMSA5NS4xLTc5LjFjMTEuMjggMCAyMS45NSAxLjkyIDMyIDQuODk4VjEyNi41YzAtMTIuOTcgMTAuMDYtMjYuNjMgMjIuNDEtMzAuNTJsMzE5LjEtOTQuNDlDNDcyLjEgLjY2MTUgNDc3LjMgMCA0ODAgMGMxNy42NiAwIDMxLjk3IDE0LjM0IDMyIDMxLjk5TDUxMS4xIDM2Ny4xeiIvPjwvc3ZnPg==">
</head>

<body>
    <div class="topnav">
        <h1>SoundBoard WebUI (<span id="json_hostname_header"></span>)</h1>
        <div><svg xmlns="http://www.w3.org/2000/svg" height="1em"
                viewBox="0 0 512 512"><!--! Font Awesome Free 6.4.2 by @fontawesome - https://fontawesome.com License - https://fontawesome.com/license (Commercial License) Copyright 2023 Fonticons, Inc. -->
                <style>
                    svg {
                        fill: #ffffff
                    }
                </style>
                <path
                    d="M256 512A256 256 0 1 0 256 0a256 256 0 1 0 0 512zM216 336h24V272H216c-13.3 0-24-10.7-24-24s10.7-24 24-24h48c13.3 0 24 10.7 24 24v88h8c13.3 0 24 10.7 24 24s-10.7 24-24 24H216c-13.3 0-24-10.7-24-24s10.7-24 24-24zm40-208a32 32 0 1 1 0 64 32 32 0 1 1 0-64z">
                </path>
            </svg><span id="json_status"></span></div>
    </div>
    <div class="content">
        <div class="card">
            <h2>Login</h2>
            <form action="/auth">
                <div class="row">
                    <label for="username">Username</label>
                    <input type="text" name="username" autocomplete="off" placeholder="Username">
                </div>
                <div class="row">
                    <label for="password">Password</label>
                    <input type="password" name="password" autocomplete="off" placeholder="Password">
                </div>
                <input type="button" data-cmd="login" class="button" value='Login'>
            </form>
        </div>
        <div class="card hidden">
            <h2>System</h2>
            <div id="json_sysinfo"></div>
        </div>
        <div class="card hidden">
            <h2>Volume</h2>
            <input type="range" id="json_cur_volume" name="volume" min="0" max="21" step="1" value="0">
            <h2>Balance</h2>
            <input type="range" id="json_cur_balance" name="balance" min="-16" max="16" step="1" value="0">
        </div>
        <div class="card hidden">
            <h2>Filesystem (external)</h2>
            <p>
            <div id="json_fs_info"></div>
            </p>
            <div id="json_fs"></div>
            <p>
            <form method="POST" action="/upload" enctype="multipart/form-data">
                <input type="file" name="data" id="fileupload" required />
                <input type="button" class="button" value="Upload"
                    onclick="document.getElementById('fileupload').click();" />
                <input type="submit" class="button" name="upload" value="Upload" style="display:none;"
                    title="Upload File">
            </form>
            </p>
        </div>
        <div class="card hidden">
            <h2>Text-to-Speech</h2>
            <p>
                <label for="tts_text">Text</label>
                <input type="text" name="tts_text" id="json_tts_text" autocomplete="off" placeholder="Text to speak">
                <select id="json_tts_lang">
                    <option value="de">German</option>
                    <option value="en">English</option>
                </select>
            </p>
            <p>
                <button class="button" data-cmd="tts">Speak</button>
            </p>
        </div>
        <div class="card hidden">
            <h2>Maintenance</h2>
            <p><button class="button confirm showLoader" data-cmd="cmd" data-value="reboot">Reboot</button></p>
            <p><button class="button confirm showLoader" data-cmd="cmd" data-value="reset">Reset Settings</button></p>
            <p>
            <form method='POST' action='/update' enctype='multipart/form-data'>
                <input type='file' name='update' id='fwupdate' required>
                <input type="button" class="button" value="Update Firmware"
                    onclick="document.getElementById('fwupdate').click();" />
                <input type='submit' class="button" value='Run Update' style="display:none;">
            </form>
            </p>
        </div>
        <div class="card hidden">
            <h2>Settings</h2>
            <form id="settings">
                <h3>Network</h3>
                <div class="row"><label for="ssid">SSID</label><input type='text' name='ssid' id="json_ssid"
                        autocomplete='off' placeholder='SSID'></div>
                <div class="row"><label for="psk">PSK</label><input type='password' name='psk' id="json_psk"
                        autocomplete='off' placeholder='PSK'></div>
                <div class="row"><label for="hostname">Hostname</label><input type='text' name='hostname'
                        id="json_hostname" autocomplete='off' placeholder='Hostname'></div>
                <h3>MQTT</h3>
                <div class="row"><label for="mqtt_broker">MQTT Broker</label><input type='text' name='mqtt_broker'
                        id="json_mqtt_broker" autocomplete='off' placeholder='MQTT Broker'></div>
                <div class="row"><label for="mqtt_port">MQTT Port</label><input type='text' name='mqtt_port'
                        id="json_mqtt_port" autocomplete='off' placeholder='MQTT Port'></div>
                <div class="row"><label for="mqtt_user">MQTT User</label><input type='text' name='mqtt_user'
                        id="json_mqtt_user" autocomplete='off' placeholder='MQTT User'></div>
                <div class="row"><label for="mqtt_pass">MQTT Pass</label><input type='password' name='mqtt_pass'
                        id="json_mqtt_pass" autocomplete='off' placeholder='MQTT Pass'></div>
                <div class="row"><label for="mqtt_prefix">MQTT Prefix</label><input type='text' name='mqtt_prefix'
                        id="json_mqtt_prefix" autocomplete='off' placeholder='MQTT Prefix'></div>
                <h3>General</h3>
                <div class="row"><label for="volume">Start volume</label><input type='text' name='volume'
                        id="json_start_volume" autocomplete='off' placeholder='Start volume'></div>
                <div class="row"><label for="balance">Start balance</label><input type='text' name='balance'
                        id="json_start_balance" autocomplete='off' placeholder='Start balance'></div>
                <br />
                <input type="button" data-cmd="settingssave" class="button confirm showLoader" value='Save and Reboot'>
            </form>
        </div>
    </div>
    <footer>
        <p>&copy; 2024 <a href="https://github.com/foorschtbar/">foorschtbar</a> - <a
                href="https://github.com/foorschtbar/SoundBoard">SoundBoard on GitHub</a></p>
    </footer>
    <div id="overlay" class="overlay" style="display: none;"></div>
    <div id="loader" class="loader" style="display: none;">
        <div class="lds-ring">
            <div></div>
            <div></div>
            <div></div>
            <div></div>
        </div>
        <div id="loader_msg">Loading...</div>
        <div id="loader_close" style="display:none">
            <button class="button">Close</button>
        </div>
    </div>
    <script>
        var modalLoader = false;
        const cookiename = "overwrite-host";
        let host = window.location.host;
        if (document.cookie.split(';').filter((item) => item.trim().startsWith(cookiename + '=')).length) {
            host = document.cookie.split(';').find(c => c.trim().startsWith(cookiename + '=')).split('=')[1];
            // replace url in form action with new host
            document.querySelectorAll('form').forEach((form) => {
                form.action = form.action.replace(window.location.host, host);
            });
        }
        const base_url = `http://${host}`;
        const gateway = `ws://${host}/ws`;
        let websocket;
        const noReconnect = false;

        document.addEventListener('DOMContentLoaded', () => {
            console.log('DOM loaded');
            initWebSocket();
            initButtons();
            initPostHook();
        });

        function initWebSocket() {
            console.log('Trying to open a WebSocket connection...');
            websocket = new WebSocket(gateway);
            websocket.onopen = onOpen;
            websocket.onclose = onClose;
            websocket.onmessage = onMessage;
        }
        function onOpen(event) {
            console.log('Connection opened');
            // getData();
        }
        function onClose(event) {
            console.log('Connection closed');
            if (!noReconnect) {
                setTimeout(initWebSocket, 5000);
            }
        }
        function onMessage(event) {
            const response = JSON.parse(event.data);
            if (processResponse(response)) {
                hideLoader();
            }
        }

        function initButtons() {
            var btns = document.getElementsByClassName('card');
            for (let i = 0; i < btns.length; i++) {
                btns[i].addEventListener('click', (event) => {
                    if (event.target.classList.contains('button')) {
                        //console.log(event)
                        const cmd = event.target.getAttribute('data-cmd');
                        const value = event.target.getAttribute("data-value")

                        // skip if no command is set
                        if (cmd == null) {
                            return;
                        }

                        let dosend = true;
                        if (event.target.classList.contains('confirm')) {
                            dosend = confirm('Are you sure?');
                        }
                        if (dosend) {
                            if (event.target.classList.contains('showLoader')) {
                                showLoader();
                            }
                            switch (cmd) {
                                case 'login':
                                    var form = event.target.closest('form');

                                    // do basic auth with username and password
                                    var username = form.elements['username'].value;
                                    var password = form.elements['password'].value;
                                    var base64 = btoa(unescape(encodeURIComponent(`${username}:${password}`)));
                                    var xhr = new XMLHttpRequest();
                                    xhr.open('GET', form.action, true);
                                    xhr.setRequestHeader('Authorization', `Basic ${base64}`);
                                    xhr.onreadystatechange = function () {
                                        if (xhr.readyState === 4 && xhr.status === 200) {
                                            console.log("Login successful");
                                            // open websocket
                                            websocket.send('{"getData": true}');
                                        } else if (xhr.readyState === 4 && xhr.status === 401) {
                                            alert('Login failed');
                                        }
                                    };
                                    xhr.send();

                                    break;
                                case 'settingssave':
                                    var form = document.getElementById('settings');
                                    var data = { settings: {}, reboot: true };
                                    // Loop through all input elements in the form
                                    for (let i = 0; i < form.elements.length; i++) {
                                        const element = form.elements[i];

                                        // Check if the element is an input
                                        if (element.tagName === 'INPUT' && (element.type === 'text' || element.type === 'password')) {
                                            // Add key-value pair to the array
                                            data.settings[element.name] = element.value;
                                        }
                                    }

                                    var json = JSON.stringify(data);
                                    console.log(json);
                                    modalLoader = true;
                                    websocket.send(json);
                                    break;
                                case 'tts':
                                    websocket.send(`{"tts": "${document.getElementById("json_tts_text").value}", "lang": "${document.getElementById('json_tts_lang').value}"}`);
                                    break;
                                default:
                                    websocket.send(`{"${cmd}": "${value}"}`);
                                    break;
                            }
                        }
                    } else if (event.target.type === 'checkbox' && event.target.classList.contains('group')) {
                        const { name } = event.target;
                        var checkboxes = document.querySelectorAll(`input[name="${name}"]`);
                        for (var i = 0; i < checkboxes.length; i++) {
                            if (checkboxes[i] !== event.target) {
                                checkboxes[i].checked = false;
                            }
                        }
                    }
                });
            }
            const dropZone = document.getElementById('json_fs');

            dropZone.addEventListener('dragstart', (e) => {
                e.dataTransfer.effectAllowed = 'copy';
            });

            dropZone.addEventListener('dragover', (e) => {
                e.preventDefault();
                dropZone.classList.add('hover');
                e.dataTransfer.dropEffect = 'copy';
            });

            // Remove highlight when the file is dragged out or dropped
            dropZone.addEventListener('dragleave', (e) => {
                dropZone.classList.remove('hover');
                e.dataTransfer.dropEffect = 'none';
            });

            dropZone.addEventListener('drop', (e) => {
                e.preventDefault();
                dropZone.classList.remove('hover');

                const files = e.dataTransfer.files;
                if (files.length > 1) {
                    alert('Please only drop one file at a time.');
                    return;
                } else {
                    document.getElementById('fileupload').files = files;
                    document.getElementById('fileupload').dispatchEvent(new Event('change'));
                }
            });


            var sliders = document.querySelectorAll('input[type="range"]');
            sliders.forEach((slider) => {
                slider.addEventListener('input', (event) => {
                    console.log(event.target);
                    const { name, value } = event.target;
                    websocket.send(`{${name}: ${value}}`);
                });
            });

            var loaderCloseBtn = document.querySelectorAll('#loader_close button');
            loaderCloseBtn.forEach((btn) => {
                btn.addEventListener('click', () => {
                    modalLoader = false;
                    hideLoader();
                });
            });
        }

        function initPostHook() {
            const elements = document.querySelectorAll("input[type='file']");
            for (let i = 0; i < elements.length; i++) {
                elements[i].addEventListener('change', (event) => {
                    const file = event.target.files[0];
                    // console.log(event.target)

                    if (!file) {
                        alert('Please select a file to upload.');
                        return;
                    }

                    // check file extension
                    if (event.target.id === 'fwupdate') {
                        const allowedExtensions = ['bin'];
                        const fileExtension = file.name.split('.').pop();
                        if (!allowedExtensions.includes(fileExtension)) {
                            alert('Invalid file type. Please select a valid firmware file.');
                            return;
                        }
                    } else {
                        const allowedExtensions = ['mp3', 'wav', 'ogg', 'flac'];
                        const fileExtension = file.name.split('.').pop();
                        if (!allowedExtensions.includes(fileExtension)) {
                            alert('Invalid file type. Please select a valid audio file.');
                            return;
                        }
                    }

                    const xhr = new XMLHttpRequest();
                    xhr.open('POST', event.target.closest('form').getAttribute('action'), true);

                    xhr.upload.onprogress = function (e) {
                        if (e.lengthComputable) {
                            const percentComplete = (e.loaded / e.total) * 100;
                            // console.log(`Upload Progress: ${Math.round(percentComplete)}%`);
                            document.getElementById('loader_msg').innerHTML = `Upload progress: ${Math.round(percentComplete)}%`;

                            if (percentComplete >= 100 && event.target.id === 'fwupdate') {
                                document.getElementById('loader_msg').innerHTML = 'Flashing firmware...';
                            }
                        }
                    };

                    xhr.onreadystatechange = function () {
                        if (xhr.readyState === 4 && xhr.status === 200) {
                            const response = JSON.parse(xhr.responseText);
                            let prefix = "Upload ";
                            if (event.target.id === 'fwupdate') {
                                prefix = "Flashing ";
                            }
                            if (response.success !== undefined) {
                                if (response.success) {
                                    if (response.refresh !== undefined) {
                                        document.getElementById('loader_msg').innerHTML = prefix + 'complete, rebooting...';
                                        // reload page
                                        setTimeout(() => {
                                            window.location.reload(true);
                                        }, response.refresh * 1000);
                                    } else {
                                        document.getElementById('loader_msg').innerHTML = prefix + 'complete';
                                        getData();
                                        modalLoader = false;
                                    }
                                } else {
                                    document.getElementById('loader_msg').innerHTML = prefix + 'failed';
                                    showLoaderCloseBtn();
                                }
                            } else {
                                document.getElementById('loader_msg').innerHTML = prefix + 'failed';
                                showLoaderCloseBtn();
                            }
                        }
                    };

                    const formData = new FormData();
                    formData.append('file', file);
                    showLoader();
                    modalLoader = true;
                    xhr.send(formData);
                });
            }
        }

        function showLoader() {
            document.getElementById('loader_msg').innerHTML = 'Loading...';
            document.getElementById('overlay').style.display = 'block';
            document.getElementById('loader').style.display = 'block';
            document.getElementById('loader_close').style.display = 'none';
            document.querySelector('.lds-ring').style.display = 'inline-block';
        }

        function hideLoader() {
            if (modalLoader) {
                return;
            } else {
                document.getElementById('overlay').style.display = 'none';
                document.getElementById('loader').style.display = 'none';
            }
        }

        function showLoaderCloseBtn() {
            document.getElementById('loader_close').style.display = 'block';
            document.querySelector('.lds-ring').style.display = 'none';
        }

        function getData() {
            showLoader();
            websocket.send('{"getData": true}');
        }

        function processResponse(response) {
            var hideloader = true;
            var html = '';
            for (const key in response) {
                if (response.hasOwnProperty(key)) {
                    if (key === 'alert') {
                        if (document.getElementById('loader').style.display === 'block') {
                            document.getElementById('loader_msg').innerHTML = response[key];
                        } else {
                            alert(response[key]);
                        }
                        continue;
                    }
                    if (key === 'refresh') {
                        if (response[key] > 0) {
                            setTimeout(() => {
                                window.location.reload(true);
                            }, response[key] * 1000);
                            hideloader = false;
                            break;
                        }
                    }
                    const elementId = `json_${key}`;
                    const element = document.getElementById(elementId);
                    // console.log("Processing " + key, response[key])
                    switch (key) {
                        case 'fs':

                            // file list
                            html = '<div class="table">';
                            for (let i = 0; i < response[key].length; i++) {
                                if (i == 0) {
                                    html += '<div>';
                                    html += '<div>Name</div>';
                                    html += '<div>Size</div>';
                                    html += '<div>Actions</div>';
                                    html += '</div>';
                                }
                                html += '<div>';
                                html += `<div>${response[key][i].name}</div>`;
                                html += `<div>${formatBytes(response[key][i].size)}</div>`;
                                html += '<div>';
                                html += `<button class="button small" data-cmd="play" data-value="${response[key][i].name}">Play</button>`;
                                html += `<button class="button small red confirm showLoader" data-cmd="delete" data-value="${response[key][i].name}">Delete</button>`;
                                html += '</div>';
                                html += '</div>';
                            }
                            html += '</div>';
                            element.innerHTML = html;
                            break;

                        case 'sysinfo':
                            html = '<div class="table">';
                            for (const entry in response[key]) {
                                var value = response[key][entry];
                                switch (entry) {
                                    case 'Free Heap':
                                    case 'Free PSRAM':
                                    case 'Flash Chip Size':
                                        value = formatBytes(value);
                                        break;
                                    case 'CPU Freq.':
                                        value = `${value} MHz`;
                                        break;
                                }
                                html += '<div>';
                                html += `<div>${entry}</div>`;
                                html += `<div>${value}</div>`;
                                html += '</div>';
                            }
                            html += '</div>';
                            element.innerHTML = html;
                            break;
                        default:
                            if (element) {
                                if (element.tagName === 'DIV' || element.tagName === 'SPAN') {
                                    element.innerText = response[key];
                                } else if (element.tagName === 'INPUT') {
                                    element.value = response[key];
                                }
                            }
                            break;
                    }
                }
            }
            return hideloader;
        }

        function formatBytes(bytes, decimals = 2) {
            bytes = parseInt(bytes);

            if (bytes === 0) return '0 Bytes';

            const k = 1024;
            const sizes = ['Bytes', 'KB', 'MB', 'GB', 'TB'];

            const i = (Math.floor(Math.log(bytes) / Math.log(k)));

            return `${Math.ceil(parseFloat((bytes / Math.pow(k, i)).toFixed(decimals)))} ${sizes[i]}`;
        }
    </script>
</body>

</html>)=====";