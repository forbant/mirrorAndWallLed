#include "web_server.h"
#include "led_controller.h"
#include "eeprom_manager.h"
#include <ESP8266WebServer.h>

ESP8266WebServer server(80);

void handleRoot();
void handleSetColor();
void handleSaveColors();

const char html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>LED Control</title>
    <style>
        body {
            font-family: 'Arial', sans-serif;
            background: linear-gradient(135deg, #fc8ec5, #439ffa);
            margin: 0;
            padding: 0;
            display: flex;
            justify-content: center;
            align-items: flex-start;
            height: 100vh;
        }
        .container {
            background: white;
            border-radius: 10px;
            padding: 20px;
            box-shadow: 0 4px 20px rgba(0, 0, 0, 0.2);
            width: 350px;
            text-align: center;
            margin: 30px;
        }
        h1 {
            margin-bottom: 20px;
            color: #333;
        }
        .strip {
            background: #f3eefc;
            box-shadow: 0 4px 10px rgba(0, 0, 0, 0.1);
            display: flex;
            justify-content: space-between;
            align-items: center;
            width: 100%;
            margin-bottom: 10px;
            padding: 10px;
            border-radius: 6px;
            box-sizing: border-box;
        }
        .strip h2 {
            color: #666;
            margin-right: 10px;
        }
        .color-picker-container {
            display: flex;
            align-items: center;
            margin-left: auto;
        }
        .apply-button-container {
            margin-top: 30px;
            text-align: center;
        }
        .apply-button {
            background-color: #FF416C;
            border: none;
            color: white;
            padding: 12px 18px;
            border-radius: 6px;
            cursor: pointer;
            width: 100%;
            margin-bottom: 15px;
        }
        .apply-button:hover {
            background-color: #FF4B2B;
        }
        .checkbox-container {
            margin-bottom: 20px;
            text-align: left;
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>LED Control</h1>

        <!-- Checkbox to toggle one color mode -->
        <div class="checkbox-container">
            <label>
                <input type="checkbox" id="useOneColor" onchange="toggleOneColor()"> Use one color for both
            </label>
        </div>

        <!-- Strip 1 (MIRROR or MIRROR AND WALL) -->
        <div class="strip" id="strip1">
            <h2 id="mirrorLabel">MIRROR</h2>
            <div class="color-picker-container">
                <input type="color" value="000000" id="color1" class="color-picker">
            </div>
        </div>

        <!-- Strip 2 (CORNER) -->
        <div class="strip" id="strip2">
            <h2>CORNER</h2>
            <div class="color-picker-container">
                <input type="color" value="000000" id="color2" class="color-picker">
            </div>
        </div>

        <div class="apply-button-container">
            <button class="apply-button" onclick="applyColors()">Apply</button>
            <button class="apply-button" onclick="saveColorsToEEPROM()">Save to EEPROM</button>
        </div>
    </div>

    <script>
        // Function to toggle between one color picker and two
        function toggleOneColor() {
            var useOneColor = document.getElementById('useOneColor').checked;
            var mirrorLabel = document.getElementById('mirrorLabel');
            var strip2 = document.getElementById('strip2');

            if (useOneColor) {
                // Merge MIRROR and CORNER
                mirrorLabel.textContent = "MIRROR AND WALL";
                strip2.style.display = 'none'; // Hide the second strip
            } else {
                // Show both MIRROR and CORNER separately
                mirrorLabel.textContent = "MIRROR";
                strip2.style.display = 'flex'; // Show the second strip
            }
        }

        // Function to apply the color to the UI
        function applyColors() {
            var color1 = document.getElementById('color1').value.substring(1);
            var color2 = document.getElementById('color2').value.substring(1);
            var xhr = new XMLHttpRequest();
            xhr.open('GET', '/setcolor?color=' + color1 + '&color2=' + color2, true);
            xhr.send();
        }

        // Function to save the colors to EEPROM on the ESP8266
        function saveColorsToEEPROM() {
            var color1 = document.getElementById('color1').value.substring(1);
            var color2 = document.getElementById('color2').value.substring(1);

            var xhr = new XMLHttpRequest();
            xhr.open('GET', '/savecolors?color1=' + color1 + '&color2=' + color2, true);
            xhr.send();
        }
    </script>
</body>
</html>
)rawliteral";

const char pageNotFound[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>404 - Broken Neon</title>
    <style>
        body {
            background-color: black;
            color: #fff;
            text-align: center;
            font-family: 'Lucida Console', monospace;
            margin: 0;
        }
        h1 {
            font-size: 80px;
            text-shadow: 0 0 20px #FF416C;
            text-shadow: 1px 1px 2px red, 0 0 1em #FF416C, 0 0 0.2em pink;
            display: flex;
            justify-content: center;
            align-items: center;
        }
        p {
            font-size: 25px;
            margin-top: 20px;
        }
        .main {
          padding: 10px;
        }
        /* Neon flicker for all characters except the last */
        .neon {
            animation: flicker 1.5s infinite;
        }

        /* Broken flicker effect for last '4' */
        .broken {
            animation: flicker-broken 2s infinite;
            transform: rotate(15deg); /* Slightly rotated to look like it's falling off */
            display: inline-block;
        }

        @keyframes flicker {
            0%, 19%, 21%, 23%, 25%, 54%, 56%, 100% { opacity: 1; }
            20%, 24%, 55% { opacity: 0.2; }
        }

        @keyframes flicker-broken {
            0%, 50% { opacity: 0.8; }
            40%, 60%, 70%, 90% { opacity: 0.3; }
            100% { opacity: 0.1; }
        }

        button {
            padding: 15px 25px;
            background-color: #ff416c;
            border: none;
            color: white;
            border-radius: 5px;
            cursor: pointer;
            margin-top: 30px;
        }

        button:hover {
            background-color: #ff4b2b;
        }
    </style>
</head>
<body>
    <div class="main">
        <h1>
          <span class="neon">4</span>
          <span class="broken">0</span>
          <span class="neon">4</span>
        </h1>
        <h1>
          <span class="neon">E</span>
          <span class="neon">R</span>
          <span class="neon">R</span>
          <span class="neon">O</span>
          <span class="broken">Я</span>
        </h1>
        <p>404 - The neon sign says 'Error'...</p>
        <p> but we already knew that.</p>
        <button onclick="window.location.href='/';">Go Back Home</button>
    </div>
</body>
</html>
)rawliteral";

void setupWebServer() {
  server.on("/", handleRoot);
  server.on("/setcolor", handleSetColor);
  server.on("/savecolors", HTTP_GET, handleSaveColors);
  server.onNotFound(handleNotFound);
  server.begin();
}

void handleWebServer() {
  server.handleClient();
}

void handleRoot() {
  CHSV color_mirror = CHSV(hue_mirror, saturation_mirror, value_mirror);
  CHSV color_wall = CHSV(hue_wall, saturation_wall, value_wall);

  uint32_t u_color_mirror = CRGBtoUInt32(hsv2rgb_spectrum(color_mirror));
  uint32_t u_color_wall = CRGBtoUInt32(hsv2rgb_spectrum(color_wall));
  
  String html_copy = FPSTR(html);

  char color1_hex[8], color2_hex[8];
  snprintf(color1_hex, sizeof(color1_hex), "#%06X", u_color_mirror);
  snprintf(color2_hex, sizeof(color2_hex), "#%06X", u_color_wall);

  html_copy.replace("value=\"#000000\" id=\"color1\"", "value=\"" + String(color1_hex) + "\" id=\"color1\"");
  html_copy.replace("value=\"#000000\" id=\"color2\"", "value=\"" + String(color2_hex) + "\" id=\"color2\"");
  
  server.send(200, "text/html", html_copy);
}

void handleNotFound() {
  String message = "<html><body><h1>404 Not Found</h1>";
  message += "<p>You tried to access: " + server.uri() + "</p>";
  message += "</body></html>";

  server.send(404, "text/html", message);
  // server.send(404, "text/html", pageNotFound);
}

void handleSetColor() {
  String colorStr1 = server.arg("color1");
  String colorStr2 = server.arg("color2");

  uint32_t u_color1 = strtoul(colorStr1.c_str(), NULL, 16);
  uint32_t u_color2 = strtoul(colorStr2.c_str(), NULL, 16);

  setColorToStrip(1, u_color1);
  setColorToStrip(2, u_color2);

  server.send(200, "text/plain", "OK");
}

void handleSaveColors() {
  String colorStr1 = server.arg("color1");
  String colorStr2 = server.arg("color2");

  uint32_t u_color1 = strtoul(colorStr1.c_str(), NULL, 16);
  uint32_t u_color2 = strtoul(colorStr2.c_str(), NULL, 16);

  saveToEEPROM(u_color1, u_color2);

  server.send(200, "text/plain", "OK");
}