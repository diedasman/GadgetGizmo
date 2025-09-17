/*  [ ESP32 DESKTOP DEVELOPMENT GADGEDGIZMO ]
---------------------------------------------
--------------- [ PINOUT ] ------------------
MUSHROOM_BUTT  35
TOGGLE_1       33
TOGGLE_2       32
TOGGLE_3       26
TOGGLE_4       25

LED_BLU_1      27
LED_BLU_2      12
LED_GRN_1      14
LED_GRN_2      13
*/

/* ================ [ PINOUT ] ===================== */
#define MUSHROOM_BUTT 35
#define TOGGLE_1 33
#define TOGGLE_2 32
#define TOGGLE_3 26
#define TOGGLE_4 25

#define LED_BLU_1 27
#define LED_BLU_2 12
#define LED_GRN_1 14
#define LED_GRN_2 13

/* ================ [ CONFIG ] ===================== */
const unsigned long debounceDelay = 50;  // ms - mushroom debounce

// Mushroom Button State variables
int mushState = HIGH;
int lastMushState = HIGH;
unsigned long lastDebounceTime = 0;

int sequenceState = 0;  // 0..N (0 = idle)

// PWM settings
const uint32_t PWM_FREQ = 5000;
const uint8_t PWM_RES = 8;  // 8-bit -> duty 0..255

// Define pwmPins in this order: GRN1, GRN2, BLU1, BLU2
const int pwmPins[4] = { LED_GRN_1, LED_GRN_2, LED_BLU_1, LED_BLU_2 };
const int togglePins[4] = { TOGGLE_1, TOGGLE_2, TOGGLE_3, TOGGLE_4 };

// Time tracking
unsigned long lastBreathTime = 0;
int breathValue = 0;
int breathDir = 1;

/* ================ [ HELPER FUNCTIONS ] ===================== */
// Turn off all LEDs
void allLedsOff() {
  for (int i = 0; i < 4; i++) ledcWrite(pwmPins[i], 0);
}

// Simple blocking LED reset delay
void resetLeds() {
  allLedsOff();
  delay(1000);  // 1 second off before next state logic
}

// Breathing animation
void ledBreathe(int pinIndex) {
  unsigned long now = millis();
  if (now - lastBreathTime > 15) {  // adjust speed
    breathValue += breathDir * 5;
    if (breathValue >= 255 || breathValue <= 0) {
      breathDir = -breathDir;
      breathValue = constrain(breathValue, 0, 255);
    }
    lastBreathTime = now;
  }
  ledcWrite(pwmPins[pinIndex], breathValue);
}

/* ================ [ STATE FUNCTIONS ] ===================== */
// State 0: Idle - all LEDs breathe
void stateIdle() {
  for (int i = 0; i < 4; i++) {
    ledBreathe(i);
  }
}

// State 1: Toggle Mode 1 - switches ON/OFF LEDs
void stateToggle1() {
  for (int i = 0; i < 4; i++) {
    int tState = digitalRead(togglePins[i]);
    ledcWrite(pwmPins[i], (tState == LOW) ? 255 : 0);
  }
}

// State 2: Toggle Mode 2 - switches control breathing LEDs
void stateToggle2() {
  for (int i = 0; i < 4; i++) {
    int tState = digitalRead(togglePins[i]);
    if (tState == LOW) {
      ledBreathe(i);
    } else {
      ledcWrite(pwmPins[i], 0);
    }
  }
}

/* ================ [ STATE MACHINE ] ===================== */
void runStateMachine() {
  switch (sequenceState) {
    case 0: stateIdle(); break;
    case 1: stateToggle1(); break;
    case 2: stateToggle2(); break;
    default: break;
  }
}

/* ================ [ MUSHROOM HANDLER ] ===================== */
void handleMushroom() {
  int reading = digitalRead(MUSHROOM_BUTT);

  if (reading != lastMushState) lastDebounceTime = millis();

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != mushState) {
      mushState = reading;
      if (mushState == LOW) {  // on press
        sequenceState++;
        if (sequenceState > 2) sequenceState = 0;  // wrap around
        Serial.printf(">> State changed to %d\n", sequenceState);
        resetLeds();
      }
    }
  }
  lastMushState = reading;
}

/* ================ [ SETUP ] ===================== */
void setup() {
  Serial.begin(115200);
  Serial.println("\n=== ESP32 DESKTOP DEVELOPMENT GADGEDGIZMO - Ready ===");

  pinMode(MUSHROOM_BUTT, INPUT_PULLUP);
  for (int i = 0; i < 4; i++) pinMode(togglePins[i], INPUT_PULLUP);

  for (int i = 0; i < 4; i++) {
    pinMode(pwmPins[i], OUTPUT);
    bool ok = ledcAttach(pwmPins[i], PWM_FREQ, PWM_RES);
    if (!ok) {
      Serial.printf("Warning: ledcAttach failed for pin %d\n", pwmPins[i]);
    } else {
      Serial.printf("ledcAttach OK for pin %d (freq=%lu res=%u)\n",
                    pwmPins[i], (unsigned long)PWM_FREQ, PWM_RES);
    }
    ledcWrite(pwmPins[i], 0);
  }

  Serial.println("System initialized.");
}

/* ================ [ LOOP ] ===================== */
void loop() {
  handleMushroom();
  runStateMachine();
}
