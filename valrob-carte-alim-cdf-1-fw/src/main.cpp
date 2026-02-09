#include <Arduino.h>
#include <Commander.h>

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

bool every_ms(unsigned long* last_run, unsigned long time) {
  if (millis() > *last_run + time) {
    *last_run = millis();
    return true;
  }
  return false;
}

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
    analogSetAttenuation(adc_attenuation_t::ADC_0db);
    if (enabled) {
      enable();
    }
  };

  void trigger_EMS() {
    m_ems = true;
    pinMode(m_en_pin, OUTPUT);
    digitalWrite(m_en_pin, LOW);
  };
  void release_EMS() {
    m_ems = false;
    if (!m_mcu_low) {
      pinMode(m_en_pin, INPUT);
    } else {
      pinMode(m_en_pin, INPUT);
    }
  };

  void disable() {
    pinMode(m_en_pin, OUTPUT);
    m_mcu_low = true;
    digitalWrite(m_en_pin, LOW);
  };

  void enable() {
    if (m_ems) return;
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

  float get_current() {
    return (analogReadMilliVolts(m_ilm_pin)/1000)/645*182;
  }

  String get_name() {
    return m_channel_name;
  }

  void print_state() {
    const static String states[] = {"GOOD", "OVERRIDE", "EMS", "FAULT", "OFF"};
    Serial.printf("%s: %s, I: %fA\n", m_channel_name, states[(uint8_t) get_state()], get_current());
  }

  bool is_good() {
    return get_state() != power_channel_state_t::FAULT;
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

Commander command = Commander(Serial);

uint ctouint(char c) {
  return c - '0';
}

void doDisableChannel(char* cmd) {
  uint channel = ctouint(cmd[0]);
  if (channel >= 1 && channel <= 5) {
    channels[channel]->disable();
    Serial.printf("Disabled %s\n", channels[channel]->get_name());
  }
}

void doEnableChannel(char* cmd) {
  uint channel = ctouint(cmd[0]);
  if (channel >= 1 && channel <= 5) {
    channels[channel]->enable();
    Serial.printf("Enabled %s\n", channels[channel]->get_name());
  }
}

PowerChannel Channel_5v_1(EN_5V_1_PIN, PG_5V_1_PIN, ILM_5V_1_PIN, "+5V 5A 1");
PowerChannel Channel_5v_2(EN_5V_2_PIN, PG_5V_2_PIN, ILM_5V_2_PIN, "+5V 5A 2");

PowerChannel Channel_12v_1(EN_12V_1_PIN, PG_12V_1_PIN, ILM_12V_1_PIN, "+12V 5A 1");
PowerChannel Channel_12v_2(EN_12V_2_PIN, PG_12V_2_PIN, ILM_12V_2_PIN, "+12V 5A 2");
PowerChannel Channel_12v_3(EN_12V_3_PIN, PG_12V_4_PIN, ILM_12V_3_PIN, "+12V 10A");

static PowerChannel* channels[] = {&Channel_5v_1, &Channel_5v_2, &Channel_12v_1, &Channel_12v_2, &Channel_12v_3};

static PowerChannel* ems_dis_channels[] = {&Channel_5v_1, &Channel_12v_1, &Channel_12v_2, &Channel_12v_3};

void setup() {
  Serial.begin(115200);
  pinMode(PWR_STATUS_PIN, OUTPUT);
  pinMode(BAU, INPUT_PULLUP);

  command.add('E', doEnableChannel);
  command.add('D', doDisableChannel);
}

unsigned long last_pwr_led_blink = 0;
bool pwr_led_state = false;
void update_pwr_led(bool ems) {
  bool is_good = true;
  for (size_t i = 0; i < sizeof(channels)/sizeof(PowerChannel*); i++) {
    is_good &= channels[i]->is_good();
  }
  if (!digitalRead(BAU)) {
    digitalWrite(PWR_STATUS_PIN, !is_good);
  } else if (every_ms(&last_pwr_led_blink, 200)) {
    digitalWrite(PWR_STATUS_PIN, pwr_led_state);
    pwr_led_state = !pwr_led_state;
  }
}


bool update_ems() {
  if (digitalRead(BAU)) {
    for (size_t i = 0; i < sizeof(ems_dis_channels)/sizeof(PowerChannel*); i++) {
      ems_dis_channels[i]->trigger_EMS();
    }
    return true;
  } else {
    for (size_t i = 0; i < sizeof(ems_dis_channels)/sizeof(PowerChannel*); i++) {
      ems_dis_channels[i]->release_EMS();
    }
    return false;
  }
}

unsigned long last_pwr_led_update = 0;
unsigned long last_serial_update = 0;

void loop() {
  if (every_ms(&last_pwr_led_update, 10)) {
    update_pwr_led(update_ems());
  }
  if (every_ms(&last_serial_update, 500)) {
    Serial.println("channels status:");
    for (size_t i = 0; i < sizeof(channels)/sizeof(PowerChannel*); i++) {
      channels[i]->print_state();
    }
  }
  command.run();
}
