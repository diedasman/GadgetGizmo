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
#define MUSHROOM_BUTT  35
#define TOGGLE_1       33
#define TOGGLE_2       32
#define TOGGLE_3       26
#define TOGGLE_4       25

#define LED_BLU_1      27
#define LED_BLU_2      12
#define LED_GRN_1      14
#define LED_GRN_2      13

/* ================ [ CONFIG ] =====================
  Add global variables here.
*/
const unsigned long debounceDelay = 50;   // ms - mushroom debounce


/* ================ [ SETUP ] =====================
  setup() function.
*/
void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("\n=== ESP32 DESKTOP DEVELOPMENT GADGEDGIZMO - Ready ===");

}

/* ================ [ LOOP ] =====================
  main loop() function.
*/
void loop() 
{
  // put your main code here, to run repeatedly:

}
