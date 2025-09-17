/*  [ ESP32 DESKTOP DEVELOPMENT GADGEDGIZMO]
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

/* ================ [ PINOUT ] =====================
  Hardware pinout configuration.
*/
#define MUSHROOM_BUTT 35
#define TOGGLE_1 33
#define TOGGLE_2 32
#define TOGGLE_3 26
#define TOGGLE_4 25

#define LED_BLU_1 27
#define LED_BLU_2 12
#define LED_GRN_1 14
#define LED_GRN_2 13

/* ================ [ CONFIG ] =====================
  Add global variables here.
*/
const unsigned long debounceDelay = 50;  // ms - mushroom debounce

// Mushroom Button State variables
int mushState = HIGH;
int lastMushState = HIGH;
unsigned long lastDebounceTime = 0;

int sequenceState = 0;            // 0..4 (0 = idle)

// PWM settings (used by ledcAttach)
const uint32_t PWM_FREQ = 5000;
const uint8_t PWM_RES = 8;  // 8-bit -> duty 0..255

// Define pwmPins in this order: GRN1, GRN2, BLU1, BLU2
const int pwmPins[4] = { LED_GRN_1, LED_GRN_2, LED_BLU_1, LED_BLU_2 };

// toggle pins and mapping to pwm index
const int togglePins[4] = { TOGGLE_1, TOGGLE_2, TOGGLE_3, TOGGLE_4 };

/* ================ [ SETUP ] =====================
  setup() function.
*/
void setup() 
{
  // put your setup code here, to run once:

  // Init serial
  Serial.begin(115200);
  Serial.println("\n=== ESP32 DESKTOP DEVELOPMENT GADGEDGIZMO - Ready ===");

  // Initialise MUSHROOM_BUTT and Toggle Switches
  pinMode(MUSHROOM_BUTT, INPUT_PULLUP);
  for (int i = 0; i < 4; i++) pinMode(togglePins[i], INPUT_PULLUP);

  // ensure LED pins are outputs, then attach LEDC via pin-based API
  for (int i = 0; i < 4; i++) {
    pinMode(pwmPins[i], OUTPUT);
    bool ok = ledcAttach(pwmPins[i], PWM_FREQ, PWM_RES);  // pin-based attach
    if (!ok) {
      Serial.printf("Warning: ledcAttach failed for pin %d\n", pwmPins[i]);
    } else {
      Serial.printf("ledcAttach OK for pin %d (freq=%lu res=%u)\n",
                    pwmPins[i], (unsigned long)PWM_FREQ, PWM_RES);
    }
    ledcWrite(pwmPins[i], 0);  // ensure off
  }

  Serial.println("System initialized.");
}  // setup()

/* ================ [ LOOP ] =====================
  main loop() function.
*/
void loop() 
{
  // put your main code here, to run repeatedly:
  handleMushroom();    // check mushroom press

}  // loop()

/* ================ [ HANDLE MUSHROOM ] =====================
  mushroom button handling function.
*/
void handleMushroom() 
{
  int reading = digitalRead(MUSHROOM_BUTT);
  if (reading != lastMushState) lastDebounceTime = millis();
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != mushState) {
      mushState = reading;
      if (mushState == LOW) { // on press
        sequenceState++;
        if (sequenceState > 4) sequenceState = 0;
        // startSequence(sequenceState);
        // Serial.printf("Mushroom pressed -> new state %d\n", sequenceState);
        Serial.println("-> MUSHROOM PRESSED");
      }
    }
  }
  lastMushState = reading;
}