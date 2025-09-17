# 🛠 GadgetGizmo Template Sketch

This is a **starting template sketch** for the **GadgetGizmo ESP32 desktop development module**.  
It provides the **basic pinout, structure, and configuration** so you can start creating new firmware builds quickly.

---

## ⚡ Overview

The GadgetGizmo template sketch includes:

- Predefined **hardware pinout** for LEDs, toggle switches, and mushroom button.  
- A section for **global configuration variables** (e.g., debounce delay).  
- Structured **`setup()`** and **`loop()`** functions ready for your code.  
- Serial initialization for debugging and logging.

The sketch is designed to **serve as a foundation** for:

- LED sequences (PWM fading, toggles, puzzle games)  
- Toggle switch tests  
- Button-triggered state machines  
- Rapid prototyping of firmware features

---

## 📍 Pinout

The hardware pinout is fixed for consistency across sketches:

| Component        | GPIO | Notes |
|------------------|------|-------|
| **MUSHROOM_BUTT** | 35   | Push button (input_pullup) |
| **TOGGLE_1**      | 33   | User input switch |
| **TOGGLE_2**      | 32   | User input switch |
| **TOGGLE_3**      | 26   | User input switch |
| **TOGGLE_4**      | 25   | User input switch |
| **LED_BLU_1**     | 27   | Blue LED above Toggle 2 |
| **LED_BLU_2**     | 12   | Blue LED above Toggle 4 |
| **LED_GRN_1**     | 14   | Green LED above Toggle 1 |
| **LED_GRN_2**     | 13   | Green LED above Toggle 3 |

> This ensures that any new sketch uses the same wiring as previous GadgetGizmo builds.

---

## 📝 Usage

1. **Open the template sketch** in the Arduino IDE.  
2. **Add global configuration variables** under the `[CONFIG]` section if needed.  
3. **Initialize hardware** in the `setup()` function:  
   ```
   pinMode(TOGGLE_1, INPUT_PULLUP);
   pinMode(LED_GRN_1, OUTPUT);
   // Repeat for all switches and LEDs
   ```
4. Write your main logic in the loop() function. Examples:
* Read toggle states and control corresponding LEDs.
* Create button-triggered state machines.
* Implement LED animations with PWM fading.

5. Use Serial for debugging:
```
Serial.print("Toggle 1 state: "); 
Serial.println(digitalRead(TOGGLE_1));
```

## 🧩 Recommended Sections in Sketch

* PINOUT – Defines all pins for buttons, toggles, and LEDs.
* CONFIG – Global variables like debounce timings, PWM settings, or mode state.
* SETUP – Initialize Serial, configure pins, attach PWM channels.
* LOOP – Main code loop for toggle reading, button state machines, LED control.

## 💡 Tips

* Use INPUT_PULLUP for toggle and mushroom button pins to simplify wiring.
* Keep the pinout consistent to avoid re-wiring for different sketches.
* Add functions for LED patterns to keep loop() clean.
* Start small: test one LED and one toggle before adding sequences or state machines.

## 🚀 Next Steps

* Implement a simple toggle → LED direct control sketch.
* Add a mushroom button state machine for sequence control.
* Expand with PWM LED fading and puzzle modes.
* Maintain separate sketches for different firmware features in /firmware/.
