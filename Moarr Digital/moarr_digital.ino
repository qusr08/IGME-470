// Frank Alfano
// 09/08/2022
// Buttons that turn on LEDs with logic gates

// Pins
int OR_LED = 12;
int AND_LED = 13;
int LEFT_BUTTON = 11;
int RIGHT_BUTTON = 10;

void setup() {
  // Setup LEDs and buttons
  pinMode(OR_LED, OUTPUT);
  pinMode(AND_LED, OUTPUT);
  pinMode(LEFT_BUTTON, INPUT);
  pinMode(RIGHT_BUTTON, INPUT);
}

// States for tracking whether or not a button is pressed
int leftButtonState = 0;
int rightButtonState = 0;

void loop() {
  // Update variables based on pressed buttons
  leftButtonState = digitalRead(LEFT_BUTTON);
  rightButtonState = digitalRead(RIGHT_BUTTON);

  // Turn on LEDs if logic gate is satisfied
  digitalWrite(OR_LED, (leftButtonState + rightButtonState > 0 ? HIGH : LOW));
  digitalWrite(AND_LED, (leftButtonState + rightButtonState > 1 ? HIGH : LOW));
}
