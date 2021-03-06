
// 2016, Maciej Witkowiak <mwitkowiak@gmail.com>

// support for C64/128 joystick:
// 4-way control+1 fire button
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

#include "Keyboard.h"
#include "Joystick2.h"

bool g_useKeyboard = false;
bool g_bannerShown = false;

#define SERIAL_SPEED 115200

class JoyAxis {
  public:
    JoyAxis(const uint8_t pinleft, const uint8_t pinright, const uint8_t keyleft, const uint8_t keyright) :
      m_pinleft(pinleft), m_pinright(pinright), m_keyleft(keyleft), m_keyright(keyright) {
        pinMode(m_pinleft, INPUT_PULLUP);
        pinMode(m_pinright, INPUT_PULLUP);
      }
    bool update(bool useKeyboard) {
      uint8_t state1 = !digitalRead(m_pinleft);
      uint8_t state2 = !digitalRead(m_pinright);
      int8_t state = 0;
      if (state1) state = -127;
      if (state2) state = 127;
      if (state != m_state) {
        if (useKeyboard) {
          if (m_state == -127) Keyboard.release(m_keyleft);
          if (m_state == 127) Keyboard.release(m_keyright);
          if (state == -127) Keyboard.press(m_keyleft);
          if (state == 127) Keyboard.press(m_keyright);
        }
        m_state = state;
        return(true);
      }
      return(false);
    }
    int8_t getState() const { return(m_state); };
  private:
    uint8_t m_pinleft, m_pinright;
    uint8_t m_keyleft, m_keyright;
    int8_t m_state {0};
};

class JoyTrigger {
  public:
    JoyTrigger(const uint8_t pintrigger, const uint8_t keytrigger) :
      m_pintrigger(pintrigger), m_keytrigger(keytrigger) {
        pinMode(pintrigger, INPUT_PULLUP);
      }
    bool update(bool useKeyboard) {
      // reversed logic because closed switch connects to GND, open is pulled up
      uint8_t state = digitalRead(m_pintrigger) ? 0 : 1;
      if (state != m_state) {
        if (useKeyboard) {
          if (m_state) {
            Keyboard.release(m_keytrigger);
          } else {
            Keyboard.press(m_keytrigger);
          }
        }
        m_state = state;
        return(true);
      }
      return(false);
    }
    int8_t getState() const { return(m_state); };
  private:
    uint8_t m_pintrigger, m_keytrigger, m_state{0};
};

class DigitalJoystick {
  public:
    DigitalJoystick(const Joystick_ usbJoystick, const JoyAxis yAxis, const JoyAxis xAxis, const JoyTrigger trigger,
      const uint8_t potx, const uint8_t poty) :
      m_usbJoystick(usbJoystick), m_yAxis(yAxis), m_xAxis(xAxis), m_trigger(trigger),
      m_potx(potx), m_poty(poty)
      {
        pinMode(m_potx, INPUT_PULLUP);
        pinMode(m_poty, INPUT_PULLUP);
        m_usbJoystick.begin();
      }
    void update(bool useKeyboard) {
      if (m_trigger.update(useKeyboard)) {
        m_usbJoystick.setButton(0, m_trigger.getState());
      }
      if (m_xAxis.update(useKeyboard)) {
        m_usbJoystick.setXAxis(m_xAxis.getState());
      }
      if (m_yAxis.update(useKeyboard)) {
        m_usbJoystick.setYAxis(m_yAxis.getState());
      }
    }
  private:
    Joystick_ m_usbJoystick;
    JoyTrigger m_trigger;
    JoyAxis m_xAxis, m_yAxis;
    uint8_t m_potx, m_poty;
};

DigitalJoystick digiJoysticks[] = {
  // pins: up, down, left, right, fire; keys: up, down, left, right, fire; pots: x, y
  DigitalJoystick(Joystick_(1), JoyAxis(4, 5, KEY_UP_ARROW, KEY_DOWN_ARROW),
                                JoyAxis(6, 7, KEY_LEFT_ARROW, KEY_RIGHT_ARROW),
                                JoyTrigger(8, KEY_RIGHT_CTRL), A2, A3),
  DigitalJoystick(Joystick_(0), JoyAxis(15, 14, 'w', 's'),
                                JoyAxis(16, 10, 'a', 'd'),
                                JoyTrigger(9, ' '), A0, A1)
};

void handleSerialCommands() {
  // show something when port is opened
  if (Serial && !g_bannerShown) {
    g_bannerShown = true;
    Serial.println("Classic 8-bit era joystick interface");
    Serial.println("Maciej Witkowiak <mwitkowiak@gmail.com> 2016");
    Serial.println("\n\npress 'k' to toggle keyboard emulation");
    Serial.println("\nopen this serial port with 1200 baud speed to reset device");
    Serial.println("\nENJOY!");
  }
  // process commands
  if (Serial.available()) {
    uint8_t c = Serial.read();
    if (c == 'k') {
      g_useKeyboard = !g_useKeyboard;
      if (g_useKeyboard) {
        Serial.print("Keyboard emulation ON: Joy#1 WSAD+space, Joy#2 arrows+right ctrl");
        Keyboard.begin();
      } else {
        Serial.print("Keyboard emulation OFF");
        Keyboard.end();
      }
      Serial.println(" (press 'k' to toggle this setting)");
    }
  }
}

void setup() {
  // joysticks already initialized thorugh DigitalJoystick constructors
  Serial.begin(SERIAL_SPEED);
}

void loop() {
  handleSerialCommands();
  for (auto &p : digiJoysticks) {
    p.update(g_useKeyboard);
  }
  delay(10);
}
