#include <Keyboard.h>

#define PIN_ENABLE_SWITCH 2

/*
   Arduino Pro Micro pinout
      +---[USB]---+
    1 |o         o| RAW
    0 |o         o| GND
  GND |o         o| RST
  GND |o         o| VCC
    2 |o         o| 21
    3 |o         o| 20
    4 |o         o| 19
    5 |o         o| 18
    6 |o         o| 15
    7 |o         o| 14
    8 |o         o| 16
    9 |o         o| 10
      +-----------+
*/

typedef struct {
  int arduinoPinNumber;
  char keyboardCharacter;
  bool isSoftwareKeyboardKeyDown;
} Channel;

#define CHANNEL_COUNT 3
Channel channels[] = {
  {3, 'a', false},
  {4, 'b', false},
  {5, 'c', false}
};

void setup() {
  TXLED0;

  pinMode(PIN_ENABLE_SWITCH, INPUT);

  for (int i = 0; i < CHANNEL_COUNT; i++) {
    pinMode(channels[i].arduinoPinNumber, INPUT_PULLUP);
  }

  Keyboard.begin();
}

// https://learn.sparkfun.com/tutorials/pro-micro--fio-v3-hookup-guide/all#example-1-blinkies

void loop() {

  const bool isEnabled = digitalRead(PIN_ENABLE_SWITCH) == HIGH;
  if (isEnabled) {
    TXLED1;
    doKeyboardStuff();
  } else {
    TXLED0;
  }
}

void doKeyboardStuff() {
  for (int i = 0; i < CHANNEL_COUNT; i++) {
    const Channel presentChannel = channels[i];

    // Inverted because using built-in pull-up resistor.
    const bool isPhysicalButtonDown = digitalRead(presentChannel.arduinoPinNumber) == LOW;

    if (isPhysicalButtonDown != presentChannel.isSoftwareKeyboardKeyDown) {
      // https://www.arduino.cc/reference/en/language/functions/usb/keyboard/
      if (isPhysicalButtonDown) {
        Keyboard.press(presentChannel.keyboardCharacter);
      } else {
        Keyboard.release(presentChannel.keyboardCharacter);
      }

      // Changing the value in the presentChannel variable does not update the channels array
      channels[i].isSoftwareKeyboardKeyDown = isPhysicalButtonDown;
    }
  }
}
