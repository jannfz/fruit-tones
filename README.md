# 🎶 Fruit Tones

An educational game that demonstrates human-computer interaction and embedded
networking using a custom keyboard and fruit emojis.

This project uses:
- An **ESP8266/ESP32** to serve a responsive web interface
  (HTML/CSS/JavaScript)
- An **Arduino Uno** to manage game logic, scoring, and player input
- A lightweight **serial protocol** to communicate between the two
  microcontrollers

## 🎮 How It Works

- The ESP hosts a webpage that displays fruit emojis and feedback.
- The Arduino receives commands (e.g., `LISTEN`, `PLAY`, `FREEPLAY`) from the
  ESP.
- The Arduino responds with game events (`NOTE`, `RESULT`, etc.).
- Real-time updates are sent to the web UI using JavaScript polling.

## 🚀 Getting Started

### Prerequisites

- ESP8266 or ESP32 board (e.g., NodeMCU or ESP32 DevKit)
- Arduino Uno
- USB serial connections to your development PC
- `arduino-cli` (or Arduino IDE)

### Build & Flash

1. Clone this repository:

   ```bash
   git clone https://github.com/your-username/fruit-keyboard-game.git
   cd fruit-keyboard-game
   ````

2. Edit your Wi-Fi credentials:

   Create a file named `secrets.h` in the ESP firmware directory:

   ```cpp
   // secrets.h
   #define WIFI_SSID     "your-ssid"
   #define WIFI_PASSWORD "your-password"
   ```

3. Compile and flash both:

   * ESP firmware: upload via USB using `arduino-cli` or PlatformIO.
   * Arduino Uno sketch: upload via USB as well.

## 📂 Folder Structure

```
fruit-keyboard-game/
├── uno/             # Arduino Uno code (game logic)
├── esp/             # ESP8266/ESP32 code (web server + WiFi)
├── web/             # HTML/CSS/JS for UI
├── build/           # Ignored folder for build artifacts
├── .gitignore
└── README.md
```

## ⚠️ Notes

* `secrets.h` is intentionally **not included in version control** for security.

## 🧾 License

MIT License — see [LICENSE](./LICENSE) for details.

