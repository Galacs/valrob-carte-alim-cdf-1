#include <Arduino.h>

#define PWR_STATUS_PIN 17

#define PG_12V_PIN 37
#define PG_5V_PIN 48
#define VSENSE_PIN 17

#define CAN_RX 35
#define CAN_TX 36

#define BAU 42

#define ILM_5V_1_PIN 1
#define ILM_5V_2_PIN 3
#define ILM_5V_3_PIN 2

#define ILM_12V_1_PIN 1
#define ILM_12V_2_PIN 7
#define ILM_12V_3_PIN 5
#define ILM_12V_4_PIN 4

#define EN_5V_1_PIN 39
#define EN_5V_2_PIN 21
#define EN_5V_3_PIN 40

#define EN_12V_1_PIN 11
#define EN_12V_2_PIN 13
#define EN_12V_3_PIN 8

#define PG_5V_1_PIN 38
#define PG_5V_2_PIN 47
#define PG_5V_3_PIN 41

#define PG_12V_1_PIN 10
#define PG_12V_2_PIN 12
#define PG_12V_3_PIN 18
#define PG_12V_4_PIN 16

class PowerChannel {
public:
  PowerChannel(int pin, String channel_name, bool enabled = true,
               float ilim = -1)
      : m_pin(pin), m_channel_name(channel_name), m_mcu_low(!enabled),
        m_soft_Ilim(ilim) {};

  int m_pin;
  String m_channel_name;
  bool m_mcu_low;
  bool m_override_low;
  bool m_pg;
  float m_current;
  float m_soft_Ilim;
};

float get_batt_volotage() {
  return 0.0;
}

void update_pwr_led() {

}

void setup() {
  pinMode(PWR_STATUS_PIN, OUTPUT);
  pinMode(EN_12V_2_PIN, OUTPUT);
  digitalWrite(EN_12V_2_PIN, LOW);
  delay(1000);
  pinMode(EN_12V_2_PIN, INPUT);
}

void loop() {
  digitalWrite(PWR_STATUS_PIN, HIGH);
  delay(200);
  digitalWrite(PWR_STATUS_PIN, LOW);
  delay(200);
}
