#include <WiFi.h>
#include <WebServer.h>

// Replace with your network credentials
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

WebServer server(80);  // Create a web server on port 80

// HTML & CSS for the web page
const char* htmlContent = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body { 
      font-family: Arial, sans-serif; 
      text-align: center; 
      margin: 50px;
      background-color: #f0f0f0;
    }
    .container {
      background-color: white;
      padding: 20px;
      border-radius: 10px;
      box-shadow: 0 0 10px rgba(0,0,0,0.1);
      max-width: 400px;
      margin: 0 auto;
    }
    h1 { color: #333; }
    .button {
      background-color: #4CAF50;
      border: none;
      color: white;
      padding: 15px 32px;
      text-align: center;
      text-decoration: none;
      display: inline-block;
      font-size: 16px;
      margin: 10px;
      cursor: pointer;
      border-radius: 5px;
      transition: background-color 0.3s;
    }
    .button-off {
      background-color: #f44336;
    }
    .button-stop {
      background-color: #ff9800;
    }
    .button-start {
      background-color: #2196F3;
    }
    .button:hover {
      opacity: 0.8;
    }
    .status {
      margin: 20px 0;
      padding: 10px;
      border-radius: 5px;
      background-color: #e7f3fe;
      border-left: 6px solid #2196F3;
    }
  </style>
</head>
<body>
  <div class="container">
    <h1>ESP32-S3 Control Panel</h1>
    
    <div class="status">
      <p>Built-in LED: <span id="ledState">%LED_STATE%</span></p>
      <p>IP Address: <span id="ip">%IP_ADDRESS%</span></p>
      <p>Board: <span id="board">ESP32-S3</span></p>
      <p>Server Status: <span id="serverStatus">RUNNING</span></p>
    </div>
    
    <a href="/led/on" class="button">Turn LED ON</a>
    <a href="/led/off" class="button button-off">Turn LED OFF</a>
    <br>
    <a href="/stop" class="button button-stop">Stop Server</a>
    <a href="/start" class="button button-start">Start Server</a>
    <br>
    <a href="/info" class="button" style="background-color: #9c27b0;">Refresh Info</a>
  </div>
</body>
</html>
)rawliteral";

// Variables to track LED state
bool ledState = false;
bool serverRunning = true;

// Function prototypes
String processor(const String& var);
void handleRoot();
void handleLEDOn();
void handleLEDOff();
void handleInfo();
void handleStopServer();
void handleStartServer();
void handleNotFound();

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);  // Turn off LED initially

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());

  // Set up server routes
  server.on("/", handleRoot);
  server.on("/led/on", handleLEDOn);
  server.on("/led/off", handleLEDOff);
  server.on("/info", handleInfo);
  server.on("/stop", handleStopServer);
  server.on("/start", handleStartServer);
  server.onNotFound(handleNotFound);

  // Start server
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  if (serverRunning) {
    server.handleClient();
  }
}

// Process the HTML template by replacing placeholders
String processor(const String& var) {
  if (var == "LED_STATE") {
    return ledState ? "ON" : "OFF";
  } else if (var == "IP_ADDRESS") {
    return WiFi.localIP().toString();
  }
  return String();
}

// Handle root URL
void handleRoot() {
  String html = htmlContent;
  html.replace("%LED_STATE%", processor("LED_STATE"));
  html.replace("%IP_ADDRESS%", processor("IP_ADDRESS"));
  server.send(200, "text/html", html);
}

// Turn LED on
void handleLEDOn() {
  digitalWrite(LED_BUILTIN, HIGH);
  ledState = true;
  Serial.println("LED turned ON");
  server.sendHeader("Location", "/");
  server.send(303);
}

// Turn LED off
void handleLEDOff() {
  digitalWrite(LED_BUILTIN, LOW);
  ledState = false;
  Serial.println("LED turned OFF");
  server.sendHeader("Location", "/");
  server.send(303);
}

// Stop the server
void handleStopServer() {
  serverRunning = false;
  Serial.println("HTTP server stopped");
  server.send(200, "text/plain", "Server stopped. Cannot restart via web until physically reset.");
}

// Start the server (this won't work after stop because server.handleClient() is not called)
void handleStartServer() {
  serverRunning = true;
  Serial.println("HTTP server started");
  server.send(200, "text/plain", "Server started");
}

// Return system information as JSON
void handleInfo() {
  String json = "{";
  json += "\"led\":" + String(ledState ? "true" : "false") + ",";
  json += "\"ip\":\"" + WiFi.localIP().toString() + "\",";
  json += "\"board\":\"ESP32-S3\",";
  json += "\"server_running\":" + String(serverRunning ? "true" : "false");
  json += "}";
  
  server.send(200, "application/json", json);
}

// Handle 404 errors
void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  
  server.send(404, "text/plain", message);
}

//http://[ESP_IP]/stop - stops the server

//http://[ESP_IP]/start - starts the server