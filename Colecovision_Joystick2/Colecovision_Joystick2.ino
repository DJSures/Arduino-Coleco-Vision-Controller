#include "Joystick.h"
#include "Keyboard.h"

// ***********************************************************************
// **                                                                   **
// ** Keyboard USB HID for Teensy or Pro Micro                          **
// ** By DJ Sures (Synthiam.com) (c)2019                                **
// **                                                                   **
// ** Updated: November 16, 2019                                        **
// **                                                                   **
// ** NOTE                                                              **
// ** ~~~~                                                              **
// ** For this to work on Raspberry Pi (Linux), you need to add the     **
// ** the following to the /boot/cmdline.txt...                         **
// **                                                                   **
// ** usbhid.quirks=0x2341:0x8036:0x040    <- for Leonardo              **
// ** usbhid.quirks=0x2341:0x8037:0x040    <- for Micro                 **
// **                                                                   **
// ** To see which Arduino device you have, run lsusb -v and look       **
// ** at the output for deteced Arduino devices. The Micro will have    **
// ** an ID of 8037 and Leonardo 8036                                   **
// **                                                                   **
// ***********************************************************************

// uncomment to use for serial terminal debugging rather than usb hid device
// this is so you can see the ascii values to verify
//#define SERIAL_DEBUG

#define NO_PRESS 255

// ********************************************************
// Inputs
// ********************************************************
#define J1UP 7
#define J1DOWN 8
#define J1LEFT 9
#define J1RIGHT 10
#define J1BUTTON 16

#define J2UP 2
#define J2DOWN 3
#define J2LEFT 4
#define J2RIGHT 5
#define J2BUTTON 6

#define HW1BUTTON 14
#define HW2BUTTON 15

// ********************************************************
// Outputs
// ********************************************************
#define J1KeypadMode A2
#define J1JoystickMode A3

#define J2KeypadMode A0
#define J2JoystickMode A1

// ********************************************************
// Variables
// ********************************************************
byte _lastJ1X = NO_PRESS;
byte _lastJ1Y = NO_PRESS;
byte _lastJ1A = NO_PRESS; // button 1 (Fire)
byte _lastJ1B = NO_PRESS; // button 2 (Arm)
byte _lastJ1K = NO_PRESS; // Keypad

byte _lastJ2X = NO_PRESS;
byte _lastJ2Y = NO_PRESS;
byte _lastJ2A = NO_PRESS; // button 1 (Fire)
byte _lastJ2B = NO_PRESS; // button 2 (Arm)
byte _lastJ2K = NO_PRESS; // Keypad

byte _lastHW1 = NO_PRESS; // hardware button 1 on the console
byte _lastHW2 = NO_PRESS; // hardware button 2 on the console

Joystick_ _j1 = Joystick_(
    0x03,
    JOYSTICK_TYPE_JOYSTICK,
    14,
    0,
    true,
    true,
    false,
    false,
    false,
    false,
    false,
    false,
    false,
    false,
    false  
  );
  
Joystick_ _j2 = Joystick_(
    0x06,
    JOYSTICK_TYPE_JOYSTICK,
    14,
    0,
    true,
    true,
    false,
    false,
    false,
    false,
    false,
    false,
    false,
    false,
    false  
  );

void setup() {

#ifdef SERIAL_DEBUG

  Serial.begin(9600);
  
#else

  _j1.setXAxisRange(-127, 127);
  _j1.setYAxisRange(-127, 127);
  _j1.begin();
  
  _j2.setXAxisRange(-127, 127);
  _j2.setYAxisRange(-127, 127);
  _j2.begin();

  Keyboard.begin();
#endif

  pinMode(J1UP, INPUT_PULLUP);
  pinMode(J1RIGHT, INPUT_PULLUP);
  pinMode(J1DOWN, INPUT_PULLUP);
  pinMode(J1LEFT, INPUT_PULLUP);
  pinMode(J1BUTTON, INPUT_PULLUP);
  pinMode(J1KeypadMode, OUTPUT);
  pinMode(J1JoystickMode, OUTPUT);

  pinMode(J2UP, INPUT_PULLUP);
  pinMode(J2RIGHT, INPUT_PULLUP);
  pinMode(J2DOWN, INPUT_PULLUP);
  pinMode(J2LEFT, INPUT_PULLUP);
  pinMode(J2BUTTON, INPUT_PULLUP);
  pinMode(J2KeypadMode, OUTPUT);
  pinMode(J2JoystickMode, OUTPUT);

  pinMode(HW1BUTTON, INPUT_PULLUP);
  pinMode(HW2BUTTON, INPUT_PULLUP);
}

// Returns the joystick button index that is pressed
byte getKeyboardPressed(byte r, byte u, byte d, byte l) {

  if (u && d && !l && !r) // 0
    return 2;
  else if (!u && !d && !l && r) // 1
    return 3;
  else if (!u && !d && l && !r) // 2 
    return 4;
  else if (u && !d && !l && r) // 3
    return 5;
  else if (u && d && l && !r) // 4
    return 6;
  else if (!u && d && l && !r) // 5
    return 7;
  else if (u && !d && !l && !r) // 6
    return 8;
  else if (!u && !d && l && r) // 7
    return 9;
  else if (!u && d && l && r) // 8
    return 10;
  else if (!u && d && !l && !r) // 9
    return 11;
  else if (!u && d && !l && r) // *
    return 12;
  else if (u && !d && l && !r) // #
    return 13;

  return NO_PRESS;
}

void loop() {

#ifdef SERIAL_DEBUG

  // Set joystick mode
  digitalWrite(J1KeypadMode, true);
  digitalWrite(J1JoystickMode, false);
  digitalWrite(J2KeypadMode, true);
  digitalWrite(J2JoystickMode, false);

  Serial.print(digitalRead(J1UP));
  Serial.print(digitalRead(J1DOWN));
  Serial.print(digitalRead(J1LEFT));
  Serial.print(digitalRead(J1RIGHT));
  Serial.print(digitalRead(J1BUTTON));

  Serial.print(' ');

  Serial.print(digitalRead(J2UP));
  Serial.print(digitalRead(J2DOWN));
  Serial.print(digitalRead(J2LEFT));
  Serial.print(digitalRead(J2RIGHT));
  Serial.print(digitalRead(J2BUTTON));

  Serial.print(' ');

  Serial.print(digitalRead(HW1BUTTON));
  Serial.print(digitalRead(HW2BUTTON));

  Serial.print(' ');

  // Set keypad mode
  digitalWrite(J1KeypadMode, false);
  digitalWrite(J1JoystickMode, true);
  digitalWrite(J2KeypadMode, false);
  digitalWrite(J2JoystickMode, true);

  bool r1 = !digitalRead(J1RIGHT);
  bool u1 = !digitalRead(J1UP);
  bool d1 = !digitalRead(J1DOWN);
  bool l1 = !digitalRead(J1LEFT);

  Serial.print(' ');
  Serial.print(getKeyboardPressed(r1, u1, d1, l1));

  bool r2 = !digitalRead(J2RIGHT);
  bool u2 = !digitalRead(J2UP);
  bool d2 = !digitalRead(J2DOWN);
  bool l2 = !digitalRead(J2LEFT);

  Serial.print(' ');
  Serial.print(getKeyboardPressed(r2, u2, d2, l2));
  
  Serial.write("\r\n");

#else

  // ---------------------------------------------------------
  // JOYSTICK 1
  // ---------------------------------------------------------

  // Set J1 joystick mode
  digitalWrite(J1KeypadMode, true);
  digitalWrite(J1JoystickMode, false);
  delayMicroseconds(10);
  
  // UP <-> DOWN
  byte j1y = NO_PRESS;

  if (!digitalRead(J1UP))
    j1y = 0;
  else if (!digitalRead(J1DOWN))
    j1y = 1;

  if (j1y != _lastJ1Y) {

    if (j1y == 0)
      _j1.setYAxis(-127);
    else if (j1y == 1)
      _j1.setYAxis(127);
    else
      _j1.setYAxis(0);

    _lastJ1Y = j1y;
  }

  // LEFT <-> RIGHT
  byte j1x = NO_PRESS;

  if (!digitalRead(J1LEFT))
    j1x = 0;
  else if (!digitalRead(J1RIGHT))
    j1x = 1;

  if (j1x != _lastJ1X) {

    if (j1x == 0)
      _j1.setXAxis(-127);
    else if (j1x == 1)
      _j1.setXAxis(127);
    else
      _j1.setXAxis(0);

    _lastJ1X = j1x;
  }

  // Fire button
  byte j1a = !digitalRead(J1BUTTON);

  if (j1a != _lastJ1A) {
      
     _j1.setButton(0, j1a);

    _lastJ1A = j1a;
  }
  
  // Set keypad mode  
  digitalWrite(J1KeypadMode, false);
  digitalWrite(J1JoystickMode, true);
  delayMicroseconds(10);

  // ARM button
  byte j1b = !digitalRead(J1BUTTON);

  if (j1b != _lastJ1B) {
      
     _j1.setButton(1, j1b);

    _lastJ1B = j1b;
  }

  // Keypad
  bool r1 = !digitalRead(J1RIGHT);
  bool u1 = !digitalRead(J1UP);
  bool d1 = !digitalRead(J1DOWN);
  bool l1 = !digitalRead(J1LEFT);

  byte j1k = getKeyboardPressed(r1, u1, d1, l1);

  if (j1k != _lastJ1K) {

    if (_lastJ1K != NO_PRESS)
      _j1.setButton(_lastJ1K, 0);
    
    if (j1k != NO_PRESS)
      _j1.setButton(j1k, 1);

    _lastJ1K = j1k;
  }

  // ---------------------------------------------------------
  // HARDWARE BUTTONS (on console)
  // ---------------------------------------------------------

  // HW1 button
  byte h1 = !digitalRead(HW1BUTTON);

  if (h1 != _lastHW1) {

      if (h1)
        Keyboard.press('q');
      else
        Keyboard.release('q');

    _lastHW1 = h1;
  }

  // HW2 button
  byte h2 = !digitalRead(HW2BUTTON);

  if (h2 != _lastHW2) {

      if (h2)
        Keyboard.press('w');
      else
        Keyboard.release('w');

    _lastHW2 = h2;
  }

  // ---------------------------------------------------------
  // JOYSTICK 2
  // ---------------------------------------------------------
  
  // Set J2 joystick mode
  digitalWrite(J2KeypadMode, true);
  digitalWrite(J2JoystickMode, false);
  delayMicroseconds(10);
  
  // UP <-> DOWN
  byte j2y = NO_PRESS;

  if (!digitalRead(J2UP))
    j2y = 0;
  else if (!digitalRead(J2DOWN))
    j2y = 1;

  if (j2y != _lastJ2Y) {

    if (j2y == 0)
      _j2.setYAxis(-127);
    else if (j2y == 1)
      _j2.setYAxis(127);
    else
      _j2.setYAxis(0);

    _lastJ2Y = j2y;
  }

  // LEFT <-> RIGHT
  byte j2x = NO_PRESS;

  if (!digitalRead(J2LEFT))
    j2x = 0;
  else if (!digitalRead(J2RIGHT))
    j2x = 1;

  if (j2x != _lastJ2X) {

    if (j2x == 0)
      _j2.setXAxis(-127);
    else if (j2x == 1)
      _j2.setXAxis(127);
    else
      _j2.setXAxis(0);

    _lastJ2X = j2x;
  }

  // Fire button
  byte j2a = !digitalRead(J2BUTTON);

  if (j2a != _lastJ2A) {
      
     _j2.setButton(0, j2a);

    _lastJ2A = j2a;
  }
  
  // Set keypad mode  
  digitalWrite(J2KeypadMode, false);
  digitalWrite(J2JoystickMode, true);
  delayMicroseconds(10);

  // ARM button
  byte j2b = !digitalRead(J2BUTTON);

  if (j2b != _lastJ2B) {
      
     _j2.setButton(1, j2b);

    _lastJ2B = j2b;
  }

  // Keypad
  bool r2 = !digitalRead(J2RIGHT);
  bool u2 = !digitalRead(J2UP);
  bool d2 = !digitalRead(J2DOWN);
  bool l2 = !digitalRead(J2LEFT);

  byte j2k = getKeyboardPressed(r2, u2, d2, l2);

  if (j2k != _lastJ2K) {

    if (_lastJ2K != NO_PRESS)
      _j2.setButton(_lastJ2K, 0);
    
    if (j2k != NO_PRESS)
      _j2.setButton(j2k, 1);

    _lastJ2K = j2k;
  }

#endif

  delay(25);
}
