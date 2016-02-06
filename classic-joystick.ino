
// 2015, Maciej Witkowiak <mwitkowiak@gmail.com>

// support for C64/128 joystick:
// 5-way control+1 fire button
// potx/y lines are connected but not handled

// DB9 joystick cable (front, looking at holes)
// 54321
// 9876

// DB9 joystick port (front, looking at pins)
// 12345
// 6789

// joystick 1
// 15 -> joystick 1 UP (1)
// 14 -> joystick 1 DOWN (2)
// 16 -> joystick 1 LEFT (3)
// 10 -> joystick 1 RIGHT (4)
// A1 -> joystick 1 POTY (5)
// 9 -> joystick 1 FIRE (6)
// VCC -> joystick 1 +5V (7)
// GND -> joystick 1 GND (8)
// A0 -> joystick 1 POTX (9)

// joystick 2
// 4 -> joystick 2 UP (1)
// 5 -> joystick 2 DOWN (2)
// 6 -> joystick 2 LEFT (3)
// 7 -> joystick 2 RIGHT (4)
// A3 -> joystick 2 POTY (5)
// 8 -> joystick 2 FIRE (6)
// VCC -> joystick 2 +5V (7)
// GND -> joystick 2 GND (8)
// A2 -> joystick 2 POTX (9)

#include "Joystick2.h"

enum class Joy1 : uint8_t { up=15, down=14, left=16, right=10, fire=9, potx=A0, poty=A1 };
enum class Joy2 : uint8_t { up=4, down=5, left=6, right=7, fire=8, potx=A2, poty=A3 };

void setup() {
  // joy1
  pinMode((uint8_t)Joy1::poty,INPUT_PULLUP);
  pinMode((uint8_t)Joy1::potx,INPUT_PULLUP);
  pinMode((uint8_t)Joy1::up,INPUT_PULLUP);
  pinMode((uint8_t)Joy1::down,INPUT_PULLUP);
  pinMode((uint8_t)Joy1::left,INPUT_PULLUP);
  pinMode((uint8_t)Joy1::right,INPUT_PULLUP);
  pinMode((uint8_t)Joy1::fire,INPUT_PULLUP);
  // joy2
  pinMode((uint8_t)Joy2::poty,INPUT_PULLUP);
  pinMode((uint8_t)Joy2::potx,INPUT_PULLUP);
  pinMode((uint8_t)Joy2::up,INPUT_PULLUP);
  pinMode((uint8_t)Joy2::down,INPUT_PULLUP);
  pinMode((uint8_t)Joy2::left,INPUT_PULLUP);
  pinMode((uint8_t)Joy2::right,INPUT_PULLUP);
  pinMode((uint8_t)Joy2::fire,INPUT_PULLUP);

  Joystick[1].begin();
  Joystick[0].begin();
}

int state1x=0, state1y=0, state2x=0, state2y=0, fire1=0, fire2=0;

void loop() {
  uint8_t state1, state2;
  int8_t state;

  // reversed logic because closed switch connects to GND, open is pulled up
  // fire
  state = digitalRead((uint8_t)Joy1::fire) ? 0 : 1;
  if (state!=fire1) {
    fire1 = state;
    Joystick[0].setButton(0,fire1);
  }
  // fire
  state = digitalRead((uint8_t)Joy2::fire) ? 0 : 1;
  if (state!=fire2) {
    fire2 = state;
    Joystick[1].setButton(0,fire2);
  }
  // right / left
  state1 = !digitalRead((uint8_t)Joy1::left);
  state2 = !digitalRead((uint8_t)Joy1::right);
  state = 0;
  if (state1) state=-127;
  if (state2) state=127;
  if (state!=state1x) {
    state1x = state;
    Joystick[0].setXAxis(state1x);
  }
  // right / left
  state1 = !digitalRead((uint8_t)Joy2::left);
  state2 = !digitalRead((uint8_t)Joy2::right);
  state = 0;
  if (state1) state=-127;
  if (state2) state=127;
  if (state!=state2x) {
    state2x = state;
    Joystick[1].setXAxis(state2x);
  }
  //
  // up / down
  state1 = !digitalRead((uint8_t)Joy1::up);
  state2 = !digitalRead((uint8_t)Joy1::down);
  state = 0;
  if (state1) state=-127;
  if (state2) state=127;
  if (state!=state1y) {
    state1y = state;
    Joystick[0].setYAxis(state1y);
  }
  // up / down
  state1 = !digitalRead((uint8_t)Joy2::up);
  state2 = !digitalRead((uint8_t)Joy2::down);
  state = 0;
  if (state1) state=-127;
  if (state2) state=127;
  if (state!=state2y) {
    state2y = state;
    Joystick[1].setYAxis(state2y);
  }

  delay(10);
}
