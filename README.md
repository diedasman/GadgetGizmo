# ⚡ GadgetGizmo  

The **GadgetGizmo** is a customizable **desktop ESP32 development module** wrapped in a neat enclosure.  
It’s designed for makers, tinkerers, and embedded developers who want a hands-on way to test out ESP32 GPIOs,  
firmware features, and creative LED/button interactions — all without messy breadboards.  

---

## ✨ Features  

- 🟢 **4x LEDs** (Green + Blue) for status, PWM fading, or sequencing  
- 🎚 **4x Toggle Switches** for interactive GPIO input testing  
- 🔴 **Mushroom Push Button** for triggering state machines, puzzles, or LED effects  
- 💡 **2x Halo LED Buttons** (1x latching, 1x non-latching) for extra flair and input control  
- 🧩 **Modular firmware approach** – experiment with different sketches and LED effects  
- 📦 Fully enclosed, giving you a professional, desktop-friendly dev tool  

---

## 📍 Pinout  

The GadgetGizmo pinout is fixed to simplify firmware development.  

| Component        | GPIO | Notes                  |
|------------------|------|------------------------|
| **MUSHROOM_BUTT** | 35   | Push button (input only) |
| **TOGGLE_1**      | 33   | Controls **LED_GRN_1** (GPIO 14) |
| **TOGGLE_2**      | 32   | Controls **LED_BLU_1** (GPIO 27) |
| **TOGGLE_3**      | 26   | Controls **LED_GRN_2** (GPIO 13) |
| **TOGGLE_4**      | 25   | Controls **LED_BLU_2** (GPIO 12) |
| **LED_GRN_1**     | 14   | Green LED above Toggle 1 |
| **LED_BLU_1**     | 27   | Blue LED above Toggle 2 |
| **LED_GRN_2**     | 13   | Green LED above Toggle 3 |
| **LED_BLU_2**     | 12   | Blue LED above Toggle 4 |

---

## 📂 Repository Structure  

This repo is the **main project development folder**.  
Here you’ll find (as development progresses):  

- 📜 **Firmware sketches** (`/firmware/`) – multiple flavors of code for the device  
- 🧾 **BOMs (Bill of Materials)** – parts lists for building your own unit  
- 🖼 **Schematics** – wiring diagrams and reference designs  
- 📝 **Documentation** – usage guides and example workflows  

---

## 🚀 Development Notes  

- All code is written in **Arduino IDE** (ESP32 board support required)  
- Multiple firmware versions will be provided:  
  - 🔄 **Basic I/O Tester** – LEDs follow toggles and button presses  
  - 🌈 **PWM Fader** – LEDs fade in/out with mushroom button sequences  
  - 🕹 **Puzzle/Game Modes** – treat toggles + LEDs like a mini puzzle box  
  - 🧪 **Experimental builds** – trying out new ESP32 features (Wi-Fi, sensors, etc.)  

---

## 🛠 Future Plans  

- 🔌 Add support for external modules (I²C sensors, displays, etc.)  
- 🌍 Optional Wi-Fi / MQTT firmware for IoT-style usage  
- 🎨 More creative LED animations and “mini-games”  
- 📖 Tutorials for extending the device into your own projects  

---

## 🤝 Contributing  

Got an idea for a firmware variant? PRs and issues are welcome!  
Feel free to fork this repo and experiment with your own code.  

---

## ⚡ Quick Start  

1. Clone this repo  
2. Open Arduino IDE and select your ESP32 board under **Tools → Board**  
3. Upload one of the provided sketches to your device  
4. Flip toggles, press buttons, and enjoy the blinkenlights ✨  

---

## 📜 License  

This project will be licensed under an open-source license (to be decided).  

---
