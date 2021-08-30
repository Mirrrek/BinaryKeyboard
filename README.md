# BinaryKeyboard
An Arduino-based binary keyboard, where any character is repesented by it's ASCII value 
# Components
- An 32u4 or SAMD based Arduino (Leonardo, Esplora, Zero, Due and MKR Family) - Pro Micro in my case
- Two switches (buttons / keyboard switches)
- Four LEDs (1 RGB LED and 1 regular LED in my case)
- Four 220 Ohm resistors (for the LEDs)
- Two 1k Ohm resistors (for the switches)
# Wiring
(too lazy to make an actual schematic)
```
Ground    ->    LED R (Red)     ->    Resistor 220 Ohm 1    ->    Arduino D8
Ground    ->    LED G (Green)   ->    Resistor 220 Ohm 2    ->    Arduino D7
Ground    ->    LED B (Blue)    ->    Resistor 220 Ohm 3    ->    Arduino D9
Ground    ->    LED O (Output)  ->    Resistor 220 Ohm 4    ->    Arduino D6


Ground    ->    Switch 1        ->    Resistor 1k Ohm 1     ->    VCC (5V)
                                V
                           Arduino D15


Ground    ->    Switch 2        ->    Resistor 1k Ohm 2     ->    VCC (5V)
                                V
                           Arduino D14
```
Note: The digital pins for switches 1 and 2 are pulled up (on accident),
it might be better to instead wire them as pulldown (swap the switch Ground and VCC)
and in the code, change if digitalReads return HIGH.
```
/*
  For pulldown
  Swap switch Ground and VCC
*/

// Instead of

  // Read inputs
  bool pressing0 = digitalRead(PIN_BIT_0) == LOW;
  bool pressing1 = digitalRead(PIN_BIT_1) == LOW;

// Do

  // Read inputs
  bool pressing0 = digitalRead(PIN_BIT_0) == HIGH;
  bool pressing1 = digitalRead(PIN_BIT_1) == HIGH;

// And instead of

    // Wait for release
    while (digitalRead(PIN_BIT_0) == LOW || digitalRead(PIN_BIT_1) == LOW);

// Do

    // Wait for release
    while (digitalRead(PIN_BIT_0) == HIGH || digitalRead(PIN_BIT_1) == HIGH);
```
