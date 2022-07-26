#include <Keyboard.h>

/*
               Arduino Pro Micro pinout

  Arduino   KiCad                    KiCad    Arduino
   pin      symbol                   symbol    pin
  number     pin     +---[USB]---+    pin     number
    1         1      |o         o|    24       RAW
    0         2      |o         o|    23       GND
  GND         3      |o         o|    22       RST
  GND         4      |o         o|    21       VCC
    2         5      |o         o|    20       21
    3         6      |o         o|    19       20
    4         7      |o         o|    18       19
    5         8      |o         o|    17       18
    6         9      |o         o|    16       15
    7        10      |o         o|    15       14
    8        11      |o         o|    14       16
    9        12      |o         o|    13       10
                     +-----------+
*/

#define PIN_INPUT_ENABLE_SWITCH 4 // Pin 7 on KiCad symbol is Arduino pin 4
#define PIN_OUTPUT_ENABLE_LIGHT 5 // Pin 8 on KiCad symbol is Arduino pin 5

#define DEBOUNCE_DELAY_MILLISEC 5 // only five milliseconds needed!

typedef struct {
  int arduinoPinNumber;
  char keyboardCharacter;
  bool isSoftwareKeyboardKeyDown;
  // stuff for debouncing is below
  int lastDigitalReading;
  unsigned long lastDebounceTimestamp;
} Channel;

#define CHANNEL_COUNT 8
Channel channels[] = {
  {10, '1', false, LOW, 0}, // Pin 13 on KiCad symbol is Arduino pin 10
  {16, '2', false, LOW, 0}, // Pin 14 on KiCad symbol is Arduino pin 16
  {14, '3', false, LOW, 0}, // Pin 15 on KiCad symbol is Arduino pin 14
  {15, '4', false, LOW, 0}, // Pin 16 on KiCad symbol is Arduino pin 15
  {18, '5', false, LOW, 0}, // Pin 17 on KiCad symbol is Arduino pin 18
  {19, '6', false, LOW, 0}, // Pin 18 on KiCad symbol is Arduino pin 19
  {20, '7', false, LOW, 0}, // Pin 19 on KiCad symbol is Arduino pin 20
  {21, '8', false, LOW, 0}  // Pin 20 on KiCad symbol is Arduino pin 21
};

bool needToReleaseKeysWhenDisabled = false;

void setup() {
  pinMode(PIN_INPUT_ENABLE_SWITCH, INPUT_PULLUP);
  pinMode(PIN_OUTPUT_ENABLE_LIGHT, OUTPUT);
  for (int i = 0; i < CHANNEL_COUNT; i++) {
    pinMode(channels[i].arduinoPinNumber, INPUT_PULLUP);
  }
  Keyboard.begin();
}


void loop() {

  // Inverted because using the Arduino's built-in pull-up resistor.
  const bool isEnabled = digitalRead(PIN_INPUT_ENABLE_SWITCH) == LOW;

  if (isEnabled) {
    digitalWrite(PIN_OUTPUT_ENABLE_LIGHT, HIGH);
    needToReleaseKeysWhenDisabled = true;
    doKeyboardStuff();
  } else {
    digitalWrite(PIN_OUTPUT_ENABLE_LIGHT, LOW);
    if (needToReleaseKeysWhenDisabled) {
      releaseKeys();
    }
    needToReleaseKeysWhenDisabled = false;
  }

}

void doKeyboardStuff() {
  for (int i = 0; i < CHANNEL_COUNT; i++) {

    // Inverted because using the Arduino's built-in pull-up resistor.
    const bool presentDigitalReading = digitalRead(channels[i].arduinoPinNumber) == LOW;

    /*
      Debouncing code is from the build-in Arduino example.
      Accessable in the Arduino software from File > Examples > 02.Digital > Debounce
      Also online at https://docs.arduino.cc/built-in-examples/digital/Debounce
    */

    if (presentDigitalReading != channels[i].lastDigitalReading) {
      channels[i].lastDebounceTimestamp = millis();
    }

    if ((millis() - channels[i].lastDebounceTimestamp) > DEBOUNCE_DELAY_MILLISEC) {
      // The present reading has been there for longer than the debounce delay.

      if (presentDigitalReading != channels[i].isSoftwareKeyboardKeyDown) {
        if (presentDigitalReading) {
          Keyboard.press(channels[i].keyboardCharacter);
        } else {
          Keyboard.release(channels[i].keyboardCharacter);
        }
        channels[i].isSoftwareKeyboardKeyDown = presentDigitalReading;
      }
    }

    channels[i].lastDigitalReading = presentDigitalReading;
  }

}

void releaseKeys() {
  for (int i = 0; i < CHANNEL_COUNT; i++) {
    if (channels[i].isSoftwareKeyboardKeyDown) {
      Keyboard.release(channels[i].keyboardCharacter);
      channels[i].isSoftwareKeyboardKeyDown = false;
    }
  }
}

/*
  Copyright Peter Froud 2022.

  This source describes Open Hardware and is licensed under the CERN-OHL-S v2.

  You may redistribute and modify this source and make products using it under
  the terms of the CERN-OHL-S v2 (https://ohwr.org/cern_ohl_s_v2.txt).

  This source is distributed WITHOUT ANY EXPRESS OR IMPLIED WARRANTY,
  INCLUDING OF MERCHANTABILITY, SATISFACTORY QUALITY AND FITNESS FOR A
  PARTICULAR PURPOSE. Please see the CERN-OHL-S v2 for applicable conditions.

  Source location: https://github.com/pfroud/jeopardy-buzzer-controller
*/
