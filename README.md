# Carte alim CDF 1

ESP32-S3-WROOM-1 5v 10A and 12v 10A six efuse protected with current and state monitoring channels power supply boards 

The repo also contains minimal firmware for the ESP32 MCU.

## IC List

- ESP32-S3-WROOM-1 (Main MCU Module)
- TLV757P (MCU 3v3 LDO)
- 2X TPS56A37 (10A smps)
- SN65HVD (CAN transceiver)
- 7x TPS259472 (5A efuse)

## Top preview


## Preview 3d render


## Pin definitions

```c
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
```
