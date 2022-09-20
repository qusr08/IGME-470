// Frank Alfano
// 9/12/22
// Simon Says!

const int GREEN_LEDS = 50;
const int YELLOW_LEDS = 51;
const int BLUE_LEDS = 52;
const int RED_LEDS = 53;

const int GREEN_BUTTON = 44;
const int YELLOW_BUTTON = 45;
const int BLUE_BUTTON = 46;
const int RED_BUTTON = 47;

const int DELAY_BETWEEN_STATES = 2000;
const int SIMON_DELAY = 300;

// The state of the game loop
typedef enum {
  IDLE, SIMON, PLAYER
} GameState;
GameState gameState;

// 4 indecies for each color button
int buttonPresses[4];

// Instead of dynamically allocating an array, just have the maximum pattern 64 colors long
// Nobody will reach the end and break it (I hope)
int simonPattern[48];
int simonPatternIndex = 0;
int playerPatternIndex = 0;
bool patternButtonPressed = false;

// Index 0: LEDs to light up (starting at the lowest (green) LEDs pin index)
// Index 1: Direction of movement (either 1 or -1)
int idleValues[2] = {GREEN_LEDS, 1};

void setup() {
  // Setup LED output mode
  pinMode(GREEN_LEDS, OUTPUT);
  pinMode(YELLOW_LEDS, OUTPUT);
  pinMode(BLUE_LEDS, OUTPUT);
  pinMode(RED_LEDS, OUTPUT);

  // Setup button input mode
  pinMode(GREEN_BUTTON, INPUT);
  pinMode(YELLOW_BUTTON, INPUT);
  pinMode(BLUE_BUTTON, INPUT);
  pinMode(RED_BUTTON, INPUT);

  // The default gamestate should be idle
  // This way the pattern is displayed before the game starts
  gameState = IDLE;

  // Set a random seed for the simon pattern generation
  randomSeed(analogRead(0));
}

void loop() {
  // Update all button inputs
  buttonPresses[0] = digitalRead(GREEN_BUTTON);
  buttonPresses[1] = digitalRead(YELLOW_BUTTON);
  buttonPresses[2] = digitalRead(BLUE_BUTTON);
  buttonPresses[3] = digitalRead(RED_BUTTON);
  
  switch (gameState) {
    case IDLE:
      // Turn on/off LEDs so they match an idle pattern
      // The pattern is the LEDs bouncing back and forth
      for (int i = GREEN_LEDS; i <= RED_LEDS; i++ ) {
        digitalWrite(i, (i == idleValues[0] ? HIGH : LOW));
      }
      // flashLED(idleValues[0], 1, 25);

      // Add the change value to the index
      // This makes the bouncing effect
      idleValues[0] += idleValues[1];
      // If the index of the LEDs to light up next has reached the edge, invert the change value so the movement goes back the other direction
      if (idleValues[0] == RED_LEDS || idleValues[0] == GREEN_LEDS) {
        idleValues[1] *= -1;
      }

      // If any button is pressed during idle mode, start the game
      if (isButtonPressed()) {
        // Set all LEDs to a LOW state (off)
        for (int i = GREEN_LEDS; i <= RED_LEDS; i++ ) {
          digitalWrite(i, LOW);
        }

        // Switch to Simon to display the start of the pattern
        gameState = SIMON;
        simonPatternIndex = 0;
      }
      
      break;
    case SIMON:
      delay(DELAY_BETWEEN_STATES);
          
      // Get a random LED to add to the sequence
      simonPattern[simonPatternIndex] = random(0, 4);
      simonPatternIndex++;
      
      // Light up the LEDs in the pattern sequentially
      for (int i = 0; i < simonPatternIndex; i++) {
        flashLED(GREEN_LEDS + simonPattern[i], 1, SIMON_DELAY);
      }

      // It's now the player's turn to repeat the pattern
      gameState = PLAYER;
      playerPatternIndex = 0;
      
      break;
    case PLAYER:
      // If a button is pressed
      // ... figure out if the player pressed the correct one
      if (!patternButtonPressed) {
        if (isButtonPressed()) {
          patternButtonPressed = true;
          
          // If the correct button was pressed
          // ... continue through the pattern
          if (buttonPresses[simonPattern[playerPatternIndex]] > 0) {
            playerPatternIndex++;
          } else {
            // If the wrong button was pressed
            // ... flash what the correct button was
            flashLED(GREEN_LEDS + simonPattern[playerPatternIndex], 6, SIMON_DELAY);

            // Return back to the IDLE state so the player can start a new game
            delay(DELAY_BETWEEN_STATES);
            gameState = IDLE;
            break;
          }
        } 
      } else if (!isButtonPressed()) {
        // If the player has reached the end of the pattern
        // ... switch back to simon to add onto the pattern
        if (playerPatternIndex == simonPatternIndex) {
          gameState = SIMON;
        }
        
        patternButtonPressed = false;
      }
      
      digitalWrite(GREEN_LEDS, (digitalRead(GREEN_BUTTON) > 0 ? HIGH : LOW));
      digitalWrite(YELLOW_LEDS, (digitalRead(YELLOW_BUTTON) > 0 ? HIGH : LOW));
      digitalWrite(BLUE_LEDS, (digitalRead(BLUE_BUTTON) > 0 ? HIGH : LOW));
      digitalWrite(RED_LEDS, (digitalRead(RED_BUTTON) > 0 ? HIGH : LOW));
      
      break;
  }

  delay(100);
}

// Flash an LED on and off a certain number of times
// int index: The index of the LED to flash
// int count: The number of times to flash LEDs
// int gap: The millisecond delay between the flashes (lower value is faster flashing)
// return: void
void flashLED (int index, int count, int gap) {
  while (count > 0) {
    digitalWrite(index, HIGH);
    delay(gap);
    digitalWrite(index, LOW);
    delay(gap);

    count--;
  }
}

// Check to see if any of the buttons are pressed
// return: Whether or not any of the buttons are pressed (does not tell which button)
bool isButtonPressed () {
  return (buttonPresses[0] + buttonPresses[1] + buttonPresses[2] + buttonPresses[3] > 0);
}
