#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// -------------------------------
// UART
// -------------------------------

constexpr uint32_t PC_BAUD  = 115200;
constexpr uint32_t BNO_BAUD = 3000000;

// -------------------------------
// ESP32 GPIO
// -------------------------------

constexpr gpio_num_t PIN_BNO_TX    = GPIO_NUM_14;
constexpr gpio_num_t PIN_BNO_RX    = GPIO_NUM_15;
constexpr gpio_num_t PIN_BNO_RESET = GPIO_NUM_13;

// Optional

constexpr gpio_num_t PIN_STATUS_LED = GPIO_NUM_4;

#endif