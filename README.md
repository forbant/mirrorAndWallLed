# Smart LED Mirror Controller 🚨

A web-controlled LED system for mirrors and ambient wall lighting, built with ESP8266. Control your lighting through an intuitive web interface or physical button.
If you want to use ESP-07S see https://www.letscontrolit.com/forum/viewtopic.php?p=14993#p14993

## System Overview

```
┌─────────────────┐      ┌──────────────────┐
│    ESP8266      │      │   Web Interface  │
│   Controller    │<────>│  Color Control   │
└────────┬────────┘      └──────────────────┘
         │
         ├────────┐
         │        │
         ▼        ▼
┌─────────────┐  ┌─────────────┐
│ Mirror LEDs │  │  Wall LEDs  │
│    (330)    │  │    (146)    │
└─────────────┘  └─────────────┘
```

## ✨ Features

- **Dual LED Strip Control**: Independently control mirror (330 LEDs) and wall (146 LEDs) strips
- **Web Interface**: Intuitive color picker interface accessible via WiFi
- **Physical Control**: Button interface for mode switching and color cycling
- **Multiple Modes**:
  - Mirror Only
  - Wall Only
  - Both Synchronized
- **State Persistence**: Saves settings to EEPROM
- **Captive Portal**: Easy web interface access through DNS redirection

## 🛠️ Hardware Requirements

- ESP8266 board (NodeMCU or similar)
- WS2812B LED strips:
  - 330 LEDs for mirror
  - 146 LEDs for wall
- Momentary push button
- 5V power supply (rated for total LED current)
- Level shifter (3.3V to 5V) for LED data line

## Circuit Diagram

```
┌──────────────────────────────────────┐
│              ESP8266                 │
│                                      │
│  GPIO5    GPIO13    GPIO14    GND   │
└───┬────────┬─────────┬──────────┬───┘
    │        │         │          │
    │        │         │          │
┌───┴───┐    │         │          │
│Button │    │         │          │
└───┬───┘    │         │          │
    │        │         │          │
    └────────┴─────────┴──────────┘
             │         │
             ▼         ▼
        Wall LEDs  Mirror LEDs
```

## 📡 Network Configuration

The device creates its own WiFi network:

```
SSID: ESPap
Password: thereisnospoon
IP Address: 192.168.4.1
```

## 🚀 Installation

1. **Required Libraries**

   ```
   ESP8266WiFi
   ESP8266WebServer
   DNSServer
   FastLED
   ```

2. **Wiring**

   ```
   ESP8266 GPIO13 -> Wall LED Strip Data
   ESP8266 GPIO14 -> Mirror LED Strip Data
   ESP8266 GPIO5  -> Button
   ```

3. **Upload Process**
   - Clone repository
   - Open in Arduino IDE
   - Select ESP8266 board
   - Upload sketch

## 💡 Usage

### Web Interface

1. Connect to `ESPap` WiFi network
2. Navigate to `http://192.168.4.1`
3. Control panel features:
   ```
   ┌────────────────────────────┐
   │      LED Controller        │
   ├────────────────────────────┤
   │ ☐ Use one color for both   │
   ├────────────────────────────┤
   │ MIRROR                     │
   │ [    Color Picker 1  🔳]   │
   ├────────────────────────────┤
   │ WALL                       │
   │ [    Color Picker 2  🔳]   │
   ├────────────────────────────┤
   │ [  ✅   Apply Colors   ]   │
   │ [  💾  Save to EEPROM  ]   │
   └────────────────────────────┘
   ```

### Button Controls

```
Short Press: Cycle modes (Mirror → Both → Wall)
Long Press: Cycle colors
Release Long Press: Save state
```

## 📁 Project Structure

```
LED_Controller/
│
├── main.cpp           # Program entry point
├── button_handler.*   # Button input handling
├── eeprom_manager.*   # Settings storage
├── led_controller.*   # LED strip control
├── web_server.*       # Web interface
└── wifi_manager.*     # WiFi and DNS setup
```

## ⚡ Power Calculations

```
LED Count:
  Mirror: 330
  Wall: 146
  Total: 476

Current Draw:
  Per LED: 60mA (full white)
  Total: 476 × 60mA = 28.56A

Power Supply:
  Voltage: 5V
  Minimum Rating: 30A (150W)
```

## 🤝 Contributing

1. Fork repository
2. Create feature branch
3. Commit changes
4. Push to branch
5. Open Pull Request

## 📝 License

MIT License - See LICENSE file

## ⚠️ Safety Notes

- Ensure proper power supply ventilation
- Connect power to both side of LED strips
- Use appropriate power wire gauge
  ```
  Recommended for 30A:
  - Main Power: 10 AWG
  - LED Strips: 18 AWG
  ```
- Install appropriate fuse protection
- Monitor power supply temperature

## 🐛 Known Issues

1. FastLED requires multiple `show()` calls
2. Web interface state updates require manual refresh

## 🔄 Planned Features

- [ ] OTA(over-the-air) update capability
- [ ] Animation patterns
- [ ] Brightness control
- [ ] Home automation integration
- [ ] Mobile app

## 📞 Support

Report issues via GitHub Issues page

---

Created with 💡 for the DIY community
