// See this link for a comprehensive guide:
// https://www.pjrc.com/teensy/td_keyboard.html
//                                                                           

#include <Bounce.h>

// Total count of pins used for buttons.
const uint8_t kButtonPinCount = 4;

// Store debouncing objects for buttons.
Bounce* _buttons[kButtonPinCount];
bool _btnDownState[kButtonPinCount];

// All Digital Pins (excluding touch pins): 2, 5 - 14, 20, 21
// The list of digital pins for buttons.
const uint8_t kButtonPins[] = { 2, 5, 6, 7 };

// (r/l)(water/paddle)(pin)

const int lWaterPin = 2;

const int lPaddlePin = 5;

const int rWaterPin = 6;

const int rPaddlePin = 7;

// (r/l)(PaddleElapsed)

elapsedMillis rPaddleElapsed = 0;

elapsedMillis lPaddleElapsed = 0;

int rPaddleValue = 0.0;

int lPaddleValue = 0.0;

// Bools

bool _rIsTiming = false;

bool _lIsTiming = false;

// Map

// The minimum ms you can paddle.
const float validPaddleMin = 200.0;

// The maximum ms you can paddle.
const float validPaddleMax = 2000.0;

// The minimum joystick range.
const float minJoystickInterval = 0.0;

// The maximum joystick range.
const float maxJoystickInterval = 1.0;

// The current key press interval.
float _keyPressInterval = 0.0;

float mapFloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void setup() {
  // Sets up buttons
  for (uint8_t i = 0; i < kButtonPinCount; ++i) {
    pinMode(kButtonPins[i], INPUT_PULLUP);
    _buttons[i] = new Bounce(kButtonPins[i], 100);
  }

  delay(500);
}

void loop() {
  for (uint8_t i = 0; i < kButtonPinCount; ++i) {
    _buttons[i]->update();
    if (_buttons[i]->fallingEdge()) {
      _btnDownState[i] = true;
    } else if (_buttons[i]->risingEdge()) {
      _btnDownState[i] = false;
    }
  }
  
  // 2,  5,  6,  7  - Pins
  // 0,  1,  2,  3  - Index
  // LW, LP, RW, RP - Name
  
  // RIGHT
  // If state of in water is true
  if (_btnDownState[2]) {
    // debug();
    // If we started to paddle
    if (_buttons[3]->fallingEdge()) {
      _rIsTiming = true;
      rPaddleElapsed = 0;
      Serial.println("Starting to paddle");
      // If they stop paddling
    } else if (_buttons[3]->risingEdge()) {
      _rIsTiming = false;
      if (rPaddleElapsed < validPaddleMin || rPaddleElapsed > validPaddleMax) {
        Serial.println(rPaddleElapsed);
        Serial.println("Improper form");
      } else {
        // map to values that are good
        rPaddleValue = rPaddleElapsed;
        _keyPressInterval = mapFloat(rPaddleValue, validPaddleMin, validPaddleMax, minJoystickInterval, maxJoystickInterval);
        Serial.println("Took a stroke");
        Serial.println(_keyPressInterval);
        Joystick.X(_keyPressInterval);
        rPaddleElapsed = 0;
        rPaddleValue = 0;
      }
    }
  } else if (_btnDownState[2] == false) {
    _rIsTiming = false;
  }

  // LEFT
  // If state of in water is true
  if (_btnDownState[0]) {
    // debug();
    // If we started to paddle
    if (_buttons[1]->fallingEdge()) {
      _lIsTiming = true;
      lPaddleElapsed = 0;
      Serial.println("Starting to paddle");
      // If they stop paddling
    } else if (_buttons[1]->risingEdge()) {
      _lIsTiming = false;
      if (lPaddleElapsed < validPaddleMin || lPaddleElapsed > validPaddleMax) {
        Serial.println(rPaddleElapsed);
        Serial.println("Improper form");
      } else {
        // map to values that are good
        lPaddleValue = lPaddleElapsed;
        _keyPressInterval = mapFloat(rPaddleValue, validPaddleMin, validPaddleMax, minJoystickInterval, maxJoystickInterval);
        Serial.println("Took a stroke");
        Serial.println(_keyPressInterval);
        Joystick.Y(_keyPressInterval);
        lPaddleElapsed = 0;
        lPaddleValue = 0;
      }
    }
  } else if (_btnDownState[0] == false) {
    _lIsTiming = false;
  } 
  delay(10);
}

void debug() {
  Serial.print("LW: ");
  Serial.println(_btnDownState[0]);
  Serial.print("LP: ");
  Serial.println(_btnDownState[1]);
  Serial.print("RW: ");
  Serial.println(_btnDownState[2]);
  Serial.print("RP: ");
  Serial.println(_btnDownState[3]);
}

