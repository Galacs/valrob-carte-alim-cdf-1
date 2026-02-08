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

float get_batt_volotage() {
  return 0.0;
}

void update_pwr_led() {

}

void setup() {
  pinMode(PWR_STATUS_PIN, OUTPUT);
}

void loop() {
  digitalWrite(PWR_STATUS_PIN, HIGH);
  delay(200);
  digitalWrite(PWR_STATUS_PIN, LOW);
  delay(200);
}
