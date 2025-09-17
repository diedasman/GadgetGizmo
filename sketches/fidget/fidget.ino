/* 
  ESP32 - pin-based LEDC API version (uses ledcAttach + ledcWrite)
  Uses your exact pinout:
    MUSHROOM_BUTT  35
    TOGGLE_1       33  -> LED_GRN_1 (14)
    TOGGLE_2       32  -> LED_BLU_1 (27)
    TOGGLE_3       26  -> LED_GRN_2 (13)
    TOGGLE_4       25  -> LED_BLU_2 (12)

    LED_BLU_1      27
    LED_BLU_2      12
    LED_GRN_1      14
    LED_GRN_2      13

  - Uses ledcAttach(pin, freq, resolution) and ledcWrite(pin, duty)
  - Non-blocking sequences (millis-based)
  - Toggles override sequences immediately (full on/off)
  - sequenceRepeats controls how many cycles per press
  - Make sure you have current-limiting resistors on LEDs (220-330Ω)
*/

#include <Arduino.h>

// ===== Pinout =====
#define MUSHROOM_BUTT  35
#define TOGGLE_1       33
#define TOGGLE_2       32
#define TOGGLE_3       26
#define TOGGLE_4       25

#define LED_BLU_1      27
#define LED_BLU_2      12
#define LED_GRN_1      14
#define LED_GRN_2      13

// ===== CONFIG =====
const unsigned long debounceDelay = 50;   // ms - mushroom debounce
const int sequenceRepeats = 2;            // how many times each sequence runs per press
const unsigned long SEQ1_PERIOD = 2000;   // ms per cycle for sequence 1 (up+down)
const unsigned long SEQ2_PERIOD = 2000;   // ms per cycle for sequence 2
const unsigned long SEQ3_PERIOD = 2000;   // ms per cycle for sequence 3
const unsigned long SEQ4_PERIOD = 2500;   // ms per cycle for sequence 4

// PWM settings (used by ledcAttach)
const uint32_t PWM_FREQ = 5000;
const uint8_t  PWM_RES  = 8;    // 8-bit -> duty 0..255

// ===== State =====
int mushState = HIGH;
int lastMushState = HIGH;
unsigned long lastDebounceTime = 0;

// Define pwmPins in this order: GRN1, GRN2, BLU1, BLU2
const int pwmPins[4] = { LED_GRN_1, LED_GRN_2, LED_BLU_1, LED_BLU_2 };

// toggle pins and mapping to pwm index
const int togglePins[4] = { TOGGLE_1, TOGGLE_2, TOGGLE_3, TOGGLE_4 };
// mapping: TOGGLE_1->GRN1(index0), TOGGLE_2->BLU1(index2), TOGGLE_3->GRN2(index1), TOGGLE_4->BLU2(index3)
const int toggleToPwmIndex[4] = { 0, 2, 1, 3 };

int prevToggleReading[4] = { HIGH, HIGH, HIGH, HIGH };
bool toggleOverride[4] = { false, false, false, false };

// Sequence engine
bool sequenceActive = false;
int sequenceState = 0;            // 0..4 (0 = idle)
unsigned long sequenceStartMs = 0;
unsigned long sequencePeriod = 1000;

// ===== Helper functions =====
int triangleDuty(unsigned long pos, unsigned long period) {
  // Returns 0..255 triangle (up then down) for position pos in [0..period)
  if (period == 0) return 0;
  unsigned long half = period / 2;
  if (pos < half) {
    // rising
    return (int) map(pos, 0UL, half, 0, 255);
  } else {
    // falling
    return (int) map(pos - half, 0UL, half, 255, 0);
  }
}

// safe pin-based PWM write wrapper (uses new API ledcWrite(pin, duty))
void pwmWritePin(int pin, uint8_t duty) {
  // duty 0..255
  ledcWrite(pin, duty);
}

// ===== Setup =====
void setup() {
  Serial.begin(115200);
  delay(50);
  Serial.println("\n=== ESP32 PWM (ledcAttach + ledcWrite) - Ready ===");

  pinMode(MUSHROOM_BUTT, INPUT_PULLUP);
  for (int i=0;i<4;i++) pinMode(togglePins[i], INPUT_PULLUP);

  // ensure LED pins are outputs, then attach LEDC via pin-based API
  for (int i=0;i<4;i++) {
    pinMode(pwmPins[i], OUTPUT);
    bool ok = ledcAttach(pwmPins[i], PWM_FREQ, PWM_RES); // pin-based attach
    if (!ok) {
      Serial.printf("Warning: ledcAttach failed for pin %d\n", pwmPins[i]);
    } else {
      Serial.printf("ledcAttach OK for pin %d (freq=%lu res=%u)\n",
                    pwmPins[i], (unsigned long)PWM_FREQ, PWM_RES);
    }
    ledcWrite(pwmPins[i], 0); // ensure off
  }

  // read initial toggle states
  for (int i=0;i<4;i++) {
    prevToggleReading[i] = digitalRead(togglePins[i]);
    if (prevToggleReading[i] == LOW) {
      toggleOverride[i] = true;
      pwmWritePin(pwmPins[toggleToPwmIndex[i]], 255);
      Serial.printf("Initial: Toggle %d ON -> override ON\n", i+1);
    }
  }

  Serial.println("System initialized. State = 0 (idle).");
}

// ===== Main loop =====
void loop() {
  handleToggles();     // always check toggles first (low latency)
  handleMushroom();    // check mushroom press
  if (sequenceActive) runSequence(); // non-blocking sequence update
}

// ===== Toggle logic (instant override) =====
void handleToggles() {
  for (int i=0;i<4;i++) {
    int r = digitalRead(togglePins[i]);
    if (r != prevToggleReading[i]) {
      prevToggleReading[i] = r;
      int pwmIndex = toggleToPwmIndex[i];
      if (r == LOW) {
        toggleOverride[i] = true;
        pwmWritePin(pwmPins[pwmIndex], 255); // full on
        Serial.printf("Toggle %d ON -> LED full ON (pin %d)\n", i+1, pwmPins[pwmIndex]);
      } else {
        toggleOverride[i] = false;
        pwmWritePin(pwmPins[pwmIndex], 0);   // off
        Serial.printf("Toggle %d OFF -> LED OFF (pin %d)\n", i+1, pwmPins[pwmIndex]);
      }
    }
  }
}

// ===== Mushroom (debounced) =====
void handleMushroom() {
  int reading = digitalRead(MUSHROOM_BUTT);
  if (reading != lastMushState) lastDebounceTime = millis();
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != mushState) {
      mushState = reading;
      if (mushState == LOW) { // on press
        sequenceState++;
        if (sequenceState > 4) sequenceState = 0;
        startSequence(sequenceState);
        Serial.printf("Mushroom pressed -> new state %d\n", sequenceState);
      }
    }
  }
  lastMushState = reading;
}

// ===== Start sequence (non-blocking) =====
void startSequence(int state) {
  if (state == 0) {
    // idle -> cancel sequence and clear non-overridden LEDs
    sequenceActive = false;
    for (int i=0;i<4;i++) if (!toggleOverride[i]) pwmWritePin(pwmPins[i], 0);
    Serial.println("State 0: idle (sequence cancelled).");
    return;
  }

  sequenceActive = true;
  sequenceState = state;
  sequenceStartMs = millis();

  switch (state) {
    case 1: sequencePeriod = SEQ1_PERIOD; break;
    case 2: sequencePeriod = SEQ2_PERIOD; break;
    case 3: sequencePeriod = SEQ3_PERIOD; break;
    case 4: sequencePeriod = SEQ4_PERIOD; break;
    default: sequencePeriod = SEQ1_PERIOD; break;
  }

  Serial.printf("Starting sequence %d (period %lums) x %d repeats\n",
                state, sequencePeriod, sequenceRepeats);
}

// ===== Run sequence each loop (non-blocking) =====
void runSequence() {
  unsigned long now = millis();
  unsigned long elapsed = now - sequenceStartMs;

  unsigned long cyclesCompleted = elapsed / sequencePeriod;
  if ((int)cyclesCompleted >= sequenceRepeats) {
    // finished
    sequenceActive = false;
    for (int i=0;i<4;i++) {
      // if toggle overrides, leave it; otherwise turn off
      bool overridden = false;
      for (int t=0;t<4;t++) if (toggleToPwmIndex[t] == i && toggleOverride[t]) overridden = true;
      if (!overridden) pwmWritePin(pwmPins[i], 0);
    }
    Serial.println("Sequence finished (auto-stop).");
    return;
  }

  unsigned long pos = elapsed % sequencePeriod; // position in current cycle

  uint8_t duty[4] = {0,0,0,0}; // GRN1,GRN2,BLU1,BLU2

  switch (sequenceState) {
    case 1: { // breathing greens (GRN1 and GRN2 offset by half cycle)
      duty[0] = (uint8_t) triangleDuty(pos, sequencePeriod);
      unsigned long pos2 = (pos + (sequencePeriod/2)) % sequencePeriod;
      duty[1] = (uint8_t) triangleDuty(pos2, sequencePeriod);
      duty[2] = 0; duty[3] = 0;
      break;
    }

    case 2: { // opposite fade GRN1 <-> BLU1
      uint8_t g = (uint8_t) triangleDuty(pos, sequencePeriod);
      duty[0] = g;      // GRN1
      duty[2] = 255 - g; // BLU1
      duty[1] = 0; duty[3] = 0;
      break;
    }

    case 3: { // diagonals crossfade: (GRN1+BLU2) vs (GRN2+BLU1)
      uint8_t v = (uint8_t) triangleDuty(pos, sequencePeriod);
      duty[0] = v;      // GRN1
      duty[3] = v;      // BLU2
      duty[1] = 255 - v; // GRN2
      duty[2] = 255 - v; // BLU1
      break;
    }

    case 4: { // all fade in, hold, fade out
      unsigned long fadeIn = sequencePeriod/3;
      unsigned long hold   = sequencePeriod/3;
      unsigned long fadeOut= sequencePeriod - fadeIn - hold;
      if (pos < fadeIn) {
        uint8_t x = (uint8_t) map(pos, 0UL, fadeIn, 0, 255);
        duty[0]=duty[1]=duty[2]=duty[3]=x;
      } else if (pos < (fadeIn + hold)) {
        duty[0]=duty[1]=duty[2]=duty[3]=255;
      } else {
        unsigned long p = pos - (fadeIn + hold);
        uint8_t x = (uint8_t) map(p, 0UL, fadeOut, 255, 0);
        duty[0]=duty[1]=duty[2]=duty[3]=x;
      }
      break;
    }

    default:
      duty[0]=duty[1]=duty[2]=duty[3]=0;
      break;
  }

  // Write duties but respect overrides
  for (int i=0;i<4;i++) {
    bool overridden = false;
    for (int t=0;t<4;t++) {
      if (toggleToPwmIndex[t] == i && toggleOverride[t]) { overridden = true; break; }
    }
    if (!overridden) {
      pwmWritePin(pwmPins[i], duty[i]);
    }
  }
}
