# 🖥️ ESP32 Desktop Development GadgetGizmo

This sketch implements a **state machine** for an ESP32 development board with:
- A **Mushroom button** to change modes (states).
- **Toggle switches** to control LEDs.
- **PWM-controlled LEDs** that can turn on/off or run a "breathing" (fade) effect.

Each press of the Mushroom Button cycles to the next state. On state change, all LEDs turn off for 1 second before continuing.

---

## 📜 Features
- **Debounced Mushroom Button** → increments the state machine.
- **State Machine Architecture** → easy to extend with new states.
- **PWM LED Control** → breathing animation & toggle-based control.
- **Serial Debugging** → prints state changes only once per press.

---

## 🔄 State Machine
The system cycles through these states:

- **State 0: Idle**  
  🌱 All LEDs "breathe" together (fade in/out).

- **State 1: Toggle Mode 1**  
  🎚️ Each toggle switch directly controls one LED (ON/OFF).

- **State 2: Toggle Mode 2**  
  🌬️ Each toggle switch enables breathing mode for its LED.

---

## 🛠️ Functions Overview

### `setup()`
- Initializes serial, button pins, toggles, and LEDs.
- Attaches each LED to a PWM channel.

---

### `loop()`
```
while (true):
handleMushroom() // check if button pressed
runStateMachine() // run the active state
```

---

### `handleMushroom()`
- Reads mushroom button with debounce.
- On **press detected**:
  - Increments state.
  - Prints new state to Serial.
  - Calls `resetLeds()` → all LEDs off for 1s.

```
if button changed:
if debounce passed:
if pressed:
sequenceState++
wrap if beyond max
print state
resetLeds()
```


---

### `resetLeds()`
- Turns all LEDs off.
- Waits 1 second (blocking).  
Used when **changing states**.

---

### `allLedsOff()`
- Helper to set **all LEDs duty = 0**.

---

### `ledBreathe(pinIndex)`
- Controls the "breathing" effect (fade in/out).
- Adjusts brightness every ~15ms.
- Used in Idle and Toggle Mode 2.

```
if time passed:
breathValue += direction
if out of bounds:
reverse direction
ledcWrite(pin, breathValue)
```


---

### `stateIdle()`
- All LEDs breathe in sync.

---

### `stateToggle1()`
- Each toggle switch sets LED **fully ON** (when LOW) or **OFF** (when HIGH).

---

### `stateToggle2()`
- Each toggle switch enables breathing for its LED (LOW = breathe, HIGH = off).

---

### `runStateMachine()`
- Selects which state function to run based on `sequenceState`.

```
switch (sequenceState):
case 0: stateIdle()
case 1: stateToggle1()
case 2: stateToggle2()
default: do nothing
```


---

## 🚀 Extending the Sketch
- Add a new state:
  1. Write a new `stateXYZ()` function.
  2. Add a new `case` in `runStateMachine()`.
  3. Adjust the wrap condition in `handleMushroom()`.

This keeps the code **modular** and easy to grow.

---

## 🧭 Debugging
- All state changes are printed as:
`>> State changed to X`

- No continuous prints (avoids spamming Serial Monitor).

---
