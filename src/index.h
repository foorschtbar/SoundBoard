
#include <Arduino.h>
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>ESP32-HUB75</title>
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
  .topnav {
    overflow: hidden;
    background-color: #143642;
    position: fixed;
    left: 0;
    right: 0;
    top: 0;
    height: 75px;
  }
  #status_status {
    font-size: 1rem;
    color: #fff;
  }
  .topnav > div > svg {
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
    background-color: #F8F7F9;;
    box-shadow: 2px 2px 12px 1px rgba(140,140,140,.5);
    padding: 20px;
    margin: 30px;
    border-radius: 5px;
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
    -webkit-tap-highlight-color: rgba(0,0,0,0);
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

  #loader_msg {
    padding-top: 20px;
  }

  label {
    display: block;
    margin-bottom: 5px;
    text-align: left;
    font-weight: bold;
  }

  input[type=text], input[type=password] {
    width: 100%;
    padding: 8px;
    margin-bottom: 10px; 
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
  .table > div {
      display: table-row;
  }
  .table > div > div {
      display: table-cell;
  }
  .table > div > div:first-child {
      font-weight: bold;
      text-align: left;
  }
  .table > div > div:last-child {
      text-align: right;

  }
</style>
<title>ESP Web Server</title>
<meta name="viewport" content="width=device-width, initial-scale=1">
<link rel="icon" type="image/svg+xml" href="data:image/svg+xml,<svg xmlns=&quot;http://www.w3.org/2000/svg&quot; height=&quot;1em&quot; viewBox=&quot;0 0 512 512&quot;><!--! Font Awesome Free 6.4.2 by @fontawesome - https://fontawesome.com License - https://fontawesome.com/license (Commercial License) Copyright 2023 Fonticons, Inc. --><path d=&quot;M96 256H128V512H0V352H32V320H64V288H96V256zM512 352V512H384V256H416V288H448V320H480V352H512zM320 64H352V448H320V416H192V448H160V64H192V32H224V0H288V32H320V64zM288 128H224V192H288V128z&quot;/></svg>">
</head>
<body>
  <div class="topnav">
    <h1>ESP32-HUB75 (<span id="status_hostname_header"></span>)</h1>
     <div><svg xmlns="http://www.w3.org/2000/svg" height="1em" viewBox="0 0 512 512"><!--! Font Awesome Free 6.4.2 by @fontawesome - https://fontawesome.com License - https://fontawesome.com/license (Commercial License) Copyright 2023 Fonticons, Inc. --><style>svg{fill:#ffffff}</style><path d="M256 512A256 256 0 1 0 256 0a256 256 0 1 0 0 512zM216 336h24V272H216c-13.3 0-24-10.7-24-24s10.7-24 24-24h48c13.3 0 24 10.7 24 24v88h8c13.3 0 24 10.7 24 24s-10.7 24-24 24H216c-13.3 0-24-10.7-24-24s10.7-24 24-24zm40-208a32 32 0 1 1 0 64 32 32 0 1 1 0-64z"></path></svg><span id="status_status"></span></div>
  </div>
  <div class="content">
   <div class="card">
      <h2>System</h2>
      <div id="status_sysinfo"></div>
    </div>
     <div class="card">
      <h2>Brightness</h2>
      <input type="range" id="status_brightness" min="0" max="255" step="5" value="0" oninput="sliderChange()">
    </div>
    <div class="card">
      <h2>Test Screens</h2>
      <p><button id="screenTest" class="button" data-type="screen">Boot</button><button id="screenColorTest" class="button" data-type="screen">Color Test</button><button id="screenColorRed" class="button" data-type="screen">Red</button><button id="screenColorGreen" class="button" data-type="screen">Green</button><button id="screenColorBlue" class="button" data-type="screen">Blue</button><button id="screenColorWhite" class="button" data-type="screen">White</button><button id="screenColorBlack" class="button" data-type="screen">Black</button></p>
    </div>
    <!--<div class="card">
      <h2>GIFs</h2>
      <div id="status_gifs"></div>
    </div>-->
    <div class="card">
      <h2>Filesystem</h2>
      <p><div id="status_fs_info"></div></p>
      <p><div id="status_fs"></div></p>
      <p>
        <form method="POST" action="/upload" enctype="multipart/form-data">
          <input type="file" name="data" id="fileupload" required/>
          <input type="button" class="button" value="Upload GIF" onclick="document.getElementById('fileupload').click();" />
          <input type="submit" class="button" name="upload" value="Upload" style="display:none;" title="Upload File">
          <input type="button" class="button showLoader" value="Save settings" data-type="fssave" />
          <input type="button" class="button" value="Start playlist" data-type="playlist" />
        </form>
      </p>
    </div>
    <div class="card">
      <h2>Maintenance</h2>
      <p><button id="reboot" class="button confirm showLoader" data-type="cmd">Reboot</button></p>
      <p><button id="format" class="button confirm showLoader" data-type="cmd">Format</button></p>
      <p>
        <form method='POST' action='/update' enctype='multipart/form-data'>
          <input type='file' name='update' id='fwupdate' required>
          <input type="button" class="button" value="Update Firmware" onclick="document.getElementById('fwupdate').click();" />
          <input type='submit' class="button" value='Run Update' style="display:none;">
        </form>
      </p>
    </div>
    <div class="card">
      <h2>Settings</h2>
      <form id="settings">
      <p><label for="ssid">SSID</label><input type='text' name='ssid' id="status_ssid" autocomplete='off' placeholder='SSID'></p>
      <p><label for="psk">PSK</label><input type='password' name='psk' id="status_psk" autocomplete='off' placeholder='PSK'></p>
      <p><label for="hostname">Hostname</label><input type='text' name='hostname' id="status_hostname" autocomplete='off' placeholder='Hostname' ></p>
      <p><label for="min_gif_time">Min. GIF playback time</label><input type='text' name='min_gif_time' id="status_min_gif_time" autocomplete='off' placeholder='Min. GIF playback time'></p>
      <p><label for="min_gif_time">Autoplay delay</label><input type='text' name='autoplay_delay' id="status_autoplay_delay" autocomplete='off' placeholder='Autoplay delay'></p>
      <p><input type="button" data-type="settingssave" class="button confirm showLoader" value='Save and Reboot'></p>
      </form>
    </div>
  </div>
  <div id="overlay" class="overlay"></div>
  <div id="loader" class="loader"><div class="lds-ring"><div></div><div></div><div></div><div></div></div><div id="loader_msg">Loading...</div></div>
<script>
  var gateway = `ws://${window.location.hostname}/ws`;
  var websocket;
  var noReconnect = false;

  document.addEventListener("DOMContentLoaded", function () {
    initWebSocket();
    initButtons();
    initPostHook();
  });

  function initWebSocket() {
    console.log('Trying to open a WebSocket connection...');
    websocket = new WebSocket(gateway);
    websocket.onopen    = onOpen;
    websocket.onclose   = onClose;
    websocket.onmessage = onMessage;
  }
  function onOpen(event) {
    console.log('Connection opened');
    getData();
  }
  function onClose(event) {
    console.log('Connection closed');
    if(!noReconnect) {
      timer = setTimeout(initWebSocket, 5000);
    }
  }
  function onMessage(event) {
    try {
      var response = JSON.parse(event.data);
      if(processResponse(response)) {
        hideLoader()
      }
    } catch (error) {
      console.log("Error processing message: ", error.message);
    }

  }

  function initButtons() {
    btns = document.getElementsByClassName("card");
    for (var i = 0; i < btns.length; i++) {
        btns[i].addEventListener("click", function (event) {
          if (event.target.classList.contains('button')) {
            // console.log(event)
            var type = event.target.getAttribute("data-type")
            var data = event.target.id
            var dosend = true
            if(event.target.classList.contains("confirm")) {
              dosend = confirm("Are you sure?")
            }
            if(dosend) {
              if(event.target.classList.contains("showLoader")) {
                showLoader()
              }
              switch(type) {
                case "playlist":
                  var playlist = []
                  var checkboxes = document.getElementsByName("cb_playlist")
                  for (var i = 0; i < checkboxes.length; i++) {
                    if(checkboxes[i].checked) {
                      playlist.push(checkboxes[i].value)
                    }
                  }
                  websocket.send('{"cmd": "playlist", "playlist": "' + playlist.join(",") + '"}');
                break;
                case "fssave":
                  var playlist = []
                  var autoplay = []
                  var checkboxes = document.getElementsByName("cb_playlist")
                  for (var i = 0; i < checkboxes.length; i++) {
                    if(checkboxes[i].checked) {
                      playlist.push(checkboxes[i].value)
                    }
                  }
                  checkboxes = document.getElementsByName("cb_autoplay")
                  for (var i = 0; i < checkboxes.length; i++) {
                    if(checkboxes[i].checked) {
                      autoplay.push(checkboxes[i].value)
                    }
                  }
                  //console.log(playlist)
                  //console.log(autoplay)
                  websocket.send('{"settings":{"playlist": "' + playlist.join(",") + '", "autoplay": "' + autoplay.join(",") + '"}}');
                break;
                case "settingssave":
                  var form = document.getElementById('settings');
                  var data = {settings: {}, reboot: true};
                  // Loop through all input elements in the form
                  for (var i = 0; i < form.elements.length; i++) {
                    var element = form.elements[i];

                    // Check if the element is an input
                    if (element.tagName === 'INPUT' && (element.type === 'text' || element.type === 'password')) {
                      // Add key-value pair to the array
                      data.settings[element.name] = element.value;
                    }
                  }

                  var json = JSON.stringify(data)
                  console.log(json)
                  
                  websocket.send(json);
                break;
                default:
                   websocket.send('{"'+ type +'": "' + data + '"}')
                break;
              }
            }
          } else if (event.target.type === 'checkbox' && event.target.classList.contains('group')) {
            var name = event.target.name;
            var checkboxes = document.querySelectorAll('input[name="'+name+'"]')
            for (var i = 0; i < checkboxes.length; i++) {
              if(checkboxes[i] !== event.target) {
                checkboxes[i].checked = false
              }
            }
          }

        });
    }
  }

  function initPostHook() {
    var elements = document.querySelectorAll("input[type='file']")
    for (var i = 0; i < elements.length; i++) {
      elements[i].addEventListener("change", function (event) {
        const file = event.target.files[0];
        //console.log(event.target)

        if (!file) {
          alert("Please select a file to upload.");
          return;
        }

        const xhr = new XMLHttpRequest();
        xhr.open("POST", event.target.closest('form').getAttribute("action"), true);

        xhr.upload.onprogress = function (e) {
          if (e.lengthComputable) {
            const percentComplete = (e.loaded / e.total) * 100;
            //console.log(`Upload Progress: ${Math.round(percentComplete)}%`);
            document.getElementById("loader_msg").innerHTML = `Upload Progress: ${Math.round(percentComplete)}%`;

            if(percentComplete >= 100 && event.target.id === "fwupdate") {
              document.getElementById("loader_msg").innerHTML = "Flashing firmware...";
            }
          }
        };

        xhr.onreadystatechange = function () {
          if (xhr.readyState === 4 && xhr.status === 200) {
            var jsonResponse = JSON.parse(xhr.responseText);
            console.log("Upload Complete");
            if(jsonResponse.hasOwnProperty("success")) {
              if(jsonResponse.success) {
                if(jsonResponse.hasOwnProperty("refresh")) {
                    if(event.target.id === "fwupdate") {
                      document.getElementById("loader_msg").innerHTML = "Flashing complete, rebooting...";
                    } else {
                      document.getElementById("loader_msg").innerHTML = "Upload complete, rebooting...";
                    }

                    // reload page
                    setTimeout(function() {
                      window.location.reload(true);
                    }, jsonResponse.refresh*1000);
                } else {
                  document.getElementById("loader_msg").innerHTML = "Upload complete";
                  getData();
                }
              } else {
                document.getElementById("loader_msg").innerHTML = "Upload failed";
              }
            } else {
              document.getElementById("loader_msg").innerHTML = "Upload failed";
            }
          }
        };

        const formData = new FormData();
        formData.append("file", file);

        showLoader()
        xhr.send(formData);

        
      });
    }
  }

  function showLoader() {
    document.getElementById('loader_msg').innerHTML = "Loading..."
    document.getElementById('overlay').style.display = 'block';
    document.getElementById('loader').style.display = 'block';
  }

  function hideLoader() {
    document.getElementById('overlay').style.display = 'none';
    document.getElementById('loader').style.display = 'none';
  }

  function sliderChange() {
    websocket.send('{"brightness": "' + document.getElementById("status_brightness").value + '"}');
  }

  function getData() {
    showLoader()
    websocket.send('{"getData": true}')
  }

  function processResponse(response) {
    hideloader = true
    for (var key in response) {
      if (response.hasOwnProperty(key)) {
        if(key === "alert") {
          if(document.getElementById("loader").style.display === "block") {
            document.getElementById("loader_msg").innerHTML = response[key];
          } else {
            alert(response[key])
          }
          continue;
        }
        if(key === "refresh") {
          if(response[key] > 0) {
            setTimeout(function() {
              window.location.reload(true);
            }, response[key]*1000);
            hideloader = false
            break;
          }
        }
        var elementId = "status_" + key;
        var element = document.getElementById(elementId);
        //console.log("Processing " + key, response[key])
        switch(key) {
          case "fs":

            // file list
            var html = '<div class="table">'
            for (var i = 0; i < response[key].length; i++) {
              var isGif = response[key][i].name.toLowerCase().endsWith(".gif")
              var isAutoplay = response.autoplay.split(",").includes(response[key][i].name)
              var isPlaylist = response.playlist.split(",").includes(response[key][i].name)
              if(i == 0) {
                html += '<div>';
                html += '<div>Name</div>';
                html += '<div>Boot</div>';
                html += '<div>List</div>';
                html += '<div>Size</div>';
                html += '<div>Actions</div>';
                html += '</div>';
              }
              html += '<div>';
              html += '<div>' + response[key][i].name + '</div>';
               if(isGif) {
                html += '<div><input type="checkbox" name="cb_autoplay" '+ (isAutoplay ? "checked" : "")+' value="'+response[key][i].name+'"></div>';
                html += '<div><input type="checkbox" name="cb_playlist" '+ (isPlaylist ? "checked" : "")+' value="'+response[key][i].name+'"></div>';
                
              } else {
                html += '<div></div>';
                html += '<div></div>';
              }
              html += '<div>' + formatBytes(response[key][i].size) + '</div>';
              html += '<div>';
              if(isGif) {
                html += '<button id="' + response[key][i].name + '" class="button small" data-type="gif">Play</button>';
              }
              html += '<button id="' + response[key][i].name + '" class="button small red confirm showLoader" data-type="delete">Delete</button>';
              html += '</div>';
              html += '</div>';
            }
            html += "</div>"
            element.innerHTML = html
            break;

          case "sysinfo":
            var html = '<div class="table">'
            for (var entry in response[key]) {
              value = response[key][entry]
              switch(entry) {
                case "Free Heap":
                case "Free PSRAM":
                case "Flash Chip Size":
                  value = formatBytes(value)
                  break;
                  case "CPU Freq.":
                    value = value + " MHz"
                    break;
              }
              html += '<div>';
               html += '<div>' + entry + '</div>';
              html += '<div>' + value + '</div>';
              html += '</div>';
            }
            html += "</div>"
            element.innerHTML = html
          break;
          default:
            if (element) {
              if (element.tagName === "DIV" || element.tagName === "SPAN") {
                element.innerText = response[key];
              } else if (element.tagName === "INPUT") {
                element.value = response[key];
              }
            }
          break;
        }
      }
    }
    return hideloader
  }

  function formatBytes(bytes, decimals = 2) {
    if (bytes === 0) return '0 Bytes';

    const k = 1024;
    const sizes = ['Bytes', 'KB', 'MB', 'GB', 'TB'];

    const i = parseInt(Math.floor(Math.log(bytes) / Math.log(k)));

    return Math.ceil(parseFloat((bytes / Math.pow(k, i)).toFixed(decimals))) + ' ' + sizes[i];
  }

</script>
</body>
</html>)rawliteral";