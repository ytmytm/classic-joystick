
// 2015, Maciej Witkowiak <mwitkowiak@gmail.com>

// simple test for checking all the connections to two digital
// digital (8-bit era) joysticks
// note: this doesn't check for POTX/POTY lines

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

void setup() {
  // joy1
  pinMode(A1,INPUT_PULLUP); // poty
  pinMode(A0,INPUT_PULLUP); // potx
  pinMode(15,INPUT_PULLUP); // up
  pinMode(14,INPUT_PULLUP); // down
  pinMode(16,INPUT_PULLUP); // left
  pinMode(10,INPUT_PULLUP); // right
  pinMode(9,INPUT_PULLUP); // fire
  // joy2
  pinMode(A3,INPUT_PULLUP); // poty
  pinMode(A2,INPUT_PULLUP); // potx
  pinMode(8,INPUT_PULLUP); // fire
  pinMode(7,INPUT_PULLUP); // right
  pinMode(6,INPUT_PULLUP); // left
  pinMode(5,INPUT_PULLUP); // down
  pinMode(4,INPUT_PULLUP); // up

  while (!Serial);

  Serial.println("Hello!");
}

uint8_t lastJoy1=0;
uint8_t lastJoy2=0;

void loop() {
  uint8_t joy1=0;
  if (!digitalRead(9)) joy1 |= 1; joy1 = joy1 << 1;
  if (!digitalRead(10)) joy1 |= 1; joy1 = joy1 << 1;
  if (!digitalRead(16)) joy1 |= 1; joy1 = joy1 << 1;
  if (!digitalRead(14)) joy1 |= 1; joy1 = joy1 << 1;
  if (!digitalRead(15)) joy1 |= 1;
  if (joy1!=lastJoy1) {
    Serial.print("Joy1:");
    Serial.println(joy1,HEX);
    lastJoy1=joy1;
  }

  uint8_t joy2=0;
  if (!digitalRead(8)) joy2 |= 1; joy2 = joy2 << 1;
  if (!digitalRead(7)) joy2 |= 1; joy2 = joy2 << 1;
  if (!digitalRead(6)) joy2 |= 1; joy2 = joy2 << 1;
  if (!digitalRead(5)) joy2 |= 1; joy2 = joy2 << 1;
  if (!digitalRead(4)) joy2 |= 1;
  if (joy2!=lastJoy2) {
    Serial.print("Joy2:");
    Serial.println(joy2,HEX);
    lastJoy2=joy2;
  }

  delay(20);
}

