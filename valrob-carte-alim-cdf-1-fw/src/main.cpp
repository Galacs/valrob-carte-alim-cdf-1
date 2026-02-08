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

enum class power_channel_state_t: uint8_t {
  GOOD,
  OVERRIDE,
  EMS,
  FAULT,
  OFF,
};

class PowerChannel {
public:
  PowerChannel(int en_pin, int pg_pin, int ilm_pin, String channel_name,
               bool enabled = true, float ilim = -1)
      : m_en_pin(en_pin), m_pg_pin(pg_pin), m_ilm_pin(ilm_pin),
        m_channel_name(channel_name), m_mcu_low(!enabled), m_soft_Ilim(ilim) {
    pinMode(m_en_pin, INPUT);
    pinMode(m_pg_pin, INPUT);
    pinMode(m_ilm_pin, INPUT);
    if (enabled) {
      enable();
    }
  };

  void trigger_EMS() {
    m_ems = true;
    disable();
  };
  void release_EMS() {
    m_ems = false;
    if (m_mcu_low) {
      enable();
    }
  };

  void disable() {
    if (m_ems) return;
    pinMode(m_en_pin, OUTPUT);
    digitalWrite(m_en_pin, LOW);
    m_mcu_low = true;
    digitalWrite(m_en_pin, LOW);
  };

  void enable() {
    pinMode(m_en_pin, INPUT);
    m_mcu_low = false;
  };

  // true pg and no low en
  power_channel_state_t get_state() {
    if (m_mcu_low) return power_channel_state_t::OFF;
    if (m_ems) return power_channel_state_t::EMS;
    if (!m_mcu_low && !digitalRead(m_en_pin)) return power_channel_state_t::OVERRIDE;
    if (!m_mcu_low && digitalRead(m_en_pin) && !digitalRead(m_pg_pin)) return power_channel_state_t::FAULT;
    return power_channel_state_t::GOOD;
  }

  void print_state() {
    const static String states[] = {"GOOD", "OVERRIDE", "EMS", "FAULT", "OFF"};
    Serial.printf("%s: %s\n", m_channel_name, states[(uint8_t) get_state()]);
  }

private:
  int m_en_pin;
  int m_pg_pin;
  int m_ilm_pin;
  String m_channel_name;
  bool m_mcu_low;
  bool m_override_low;
  bool m_pg;
  bool m_ems = false;
  float m_current;
  float m_soft_Ilim;
};

float get_batt_volotage() {
  return 0.0;
}

void update_pwr_led() {

}

PowerChannel Channel_5v_1(EN_5V_1_PIN, PG_5V_1_PIN, ILM_5V_1_PIN, "+5V 5A 1");

void setup() {
  Serial.begin(115200);
  // pinMode(PWR_STATUS_PIN, OUTPUT);
  // pinMode(EN_12V_2_PIN, OUTPUT);
  // digitalWrite(EN_12V_2_PIN, LOW);
  // delay(1000);
  // pinMode(EN_12V_2_PIN, INPUT);
}

void loop() {
  Channel_5v_1.print_state();
  delay(500);
  // digitalWrite(PWR_STATUS_PIN, HIGH);
  // delay(200);
  // digitalWrite(PWR_STATUS_PIN, LOW);
  // delay(200);

  // Channel_5v_1.disable();
  // delay(1000);
  // Channel_5v_1.enable();
  // delay(1000);
}
