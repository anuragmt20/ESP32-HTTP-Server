# ESP32 HTTP Server Project

A complete ESP32-based web server project that allows you to control the built-in LED and monitor device status through a web interface.

## 📋 Project Overview
This project turns your ESP32 into a web server that:

- Hosts a responsive web control panel  
- Allows control of the built-in LED via HTTP  
- Displays system information (IP address, board type, status)  
- Provides a RESTful API endpoint for machine-readable data  
- Includes server start/stop functionality  

## 🛠 Hardware Requirements
- ESP32 development board (tested with ESP32-S3)  
- USB cable for programming and power  
- WiFi network access  

## 📁 Project Structure
```text
ESP32-HTTP-Server/
│
├── src/
│   └── ESP32_HTTP_Server.ino   # Main Arduino sketch
│
├── docs/
│   └── wiring.md               # Connection guide
│
├── README.md                   # This file
└── LICENSE                     # MIT License
```

## 🚀 Installation & Setup
1. Install Arduino IDE with ESP32 support:
   - Add ESP32 board manager URL:  
     `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`
   - Install **ESP32 by Espressif Systems** from Boards Manager

2. Select the correct board:
   - **Tools → Board → ESP32 Arduino → ESP32S3 Dev Module**
   - Select the appropriate USB port

3. Configure WiFi credentials:
   - Edit the `ssid` and `password` variables in the code with your network details

4. Upload the code to your ESP32

## 🔧 Example Code
The full Arduino sketch is inside [`src/ESP32_HTTP_Server.ino`](src/ESP32_HTTP_Server.ino).

## 📖 Usage Guide
1. Upload the code to your ESP32  
2. Open Serial Monitor at 115200 baud  
3. Copy the displayed IP address into a browser  
4. Use the control panel to:
   - Turn the LED on/off  
   - Check device status  
   - Stop/start the web server  
   - View system information  

## 📄 Wiring Guide
See [docs/wiring.md](docs/wiring.md) for details.  

## 📸 Example Output
```text
Connecting to WiFi...
Connected! IP address: 192.168.1.100
HTTP server started
LED turned ON
LED turned OFF
HTTP server stopped
```

## 🌟 Advanced Features
- OTA updates with `ArduinoOTA`  
- WebSocket support for real-time updates  
- SPIFFS for serving static files  

## 🤝 Contributing
Feel free to contribute by:  
- Reporting issues  
- Suggesting new features  
- Submitting pull requests  
- Improving documentation  

## 📜 License
This project is licensed under the MIT License - see the LICENSE file for details.  

## 🙏 Acknowledgments
- Espressif Systems for ESP32 hardware and software  
- Arduino community for libraries and examples  
- Contributors who improve this project  
