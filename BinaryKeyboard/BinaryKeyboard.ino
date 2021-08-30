/*
  TODO:
    Add an endianness switch
*/

#include <Keyboard.h>

#define READ_DELAY 50
#define TIMEOUT_DELAY 1000

#define PIN_BIT_0 15
#define PIN_BIT_1 14

#define PIN_LED_R 8
#define PIN_LED_G 7
#define PIN_LED_B 9
#define PIN_LED_O 6

enum LEDs { LED_R, LED_G, LED_B, LED_O, LED_C };

char pointer = 0;
unsigned long startTime = 0;
char character = 0b00000000;

void setup() {
  pinMode(PIN_BIT_0, INPUT);
  pinMode(PIN_BIT_1, INPUT);
  pinMode(PIN_LED_R, OUTPUT);
  pinMode(PIN_LED_G, OUTPUT);
  pinMode(PIN_LED_B, OUTPUT);
  pinMode(PIN_LED_O, OUTPUT);
  Keyboard.begin();
  setLED(LED_R);
  setLED(LED_O);
}

void loop() {
  delay(READ_DELAY);
  setLED(LED_C);
  
  // Read inputs
  bool pressing0 = digitalRead(PIN_BIT_0) == LOW;
  bool pressing1 = digitalRead(PIN_BIT_1) == LOW;

  // If the time since last keypress is greater or equal to TIMEOUT, abort
  if (pointer != 0 && (millis() - startTime) >= TIMEOUT_DELAY) {
    // Remove all printed bits
    for (int i = 0; i < pointer + 2; i++) {
      Keyboard.write(0x08);
    }

    // Reset
    setLED(LED_R);
    character = 0b00000000;
    pointer = 0;
  }

  // If any key is pressed
  if (pressing0 || pressing1) {
    // Append the bit to the character
    character = character << 1;
    character |= pressing1 ? 0b00000001 : 0b00000000;

    pressing1 ? setLED(LED_G) : setLED(LED_B);

    // Print the prefix
    if (pointer == 0) {
      Keyboard.write('<');
    } else {
      Keyboard.write(0x08);
    }

    // Print the bit
    Keyboard.write(pressing1 ? '1' : '0');
    Keyboard.write('>');

    // If character is full
    if (pointer == 7) {     
      // Remove all printed bits
      for (int i = 0; i < 10; i++) {
        Keyboard.write(0x08);
      }

      bool validCharacter = (character > 0x20 && character < 0x7f) ||
                            character == 0x08 ||  // Backspace
                            character == 0x09 ||  // Tab
                            character == 0x1b;    // Escape

      if (validCharacter) {
        // Print the character
        Keyboard.write(character);
        setLED(LED_O);
      } else {
        // remove all
        setLED(LED_R);
      }

      // Reset
      character = 0b00000000;
      pointer = 0;
    } else {
      // Reset timeout and increment poiner
      startTime = millis();
      pointer++;
    }

    // Wait for release
    while (digitalRead(PIN_BIT_0) == LOW || digitalRead(PIN_BIT_1) == LOW);
  }
}

void setLED(LEDs led) {
  switch(led) {
    case LED_R:
      digitalWrite(PIN_LED_R, HIGH);
      digitalWrite(PIN_LED_G, LOW);
      digitalWrite(PIN_LED_B, LOW);
      break;
    case LED_G:
      digitalWrite(PIN_LED_R, LOW);
      digitalWrite(PIN_LED_G, HIGH);
      digitalWrite(PIN_LED_B, LOW);
      break;
    case LED_B:
      digitalWrite(PIN_LED_R, LOW);
      digitalWrite(PIN_LED_G, LOW);
      digitalWrite(PIN_LED_B, HIGH);
      break;
    case LED_O:
      digitalWrite(PIN_LED_O, HIGH);
      break;
    case LED_C:
      digitalWrite(PIN_LED_R, LOW);
      digitalWrite(PIN_LED_G, LOW);
      digitalWrite(PIN_LED_B, LOW);
      digitalWrite(PIN_LED_O, LOW);
      break;
  }
}
