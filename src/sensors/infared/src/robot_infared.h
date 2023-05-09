/**
 * @brief    ESP32 IR Sensor library
 * @author   Nuwan Jaliyagoda, Pasan Tennakoon, Dilshani Karunarathna
 * @version  1.1.0
 * @url      https://github.com/NuwanJ/ESP32-IR
 *
 * Licensed under, Creative Commons Attribution-ShareAlike 4.0 International License
 * ------------------------------------------------------------------------------
 */

#include <Arduino.h>

#ifndef SW_IR_H
#define SW_IR_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp32-hal.h"

#include "config/pins.h"

// -- Module specific configurations --------------------------------------------

// Number of IR Receivers
#define IR_SENSOR_COUNT 4

// Maximum length of the signal can be transmittered
#define IR_BIT_LEN 32

// Pulse HIGH/LOW duration
#define PULSE_HIGH_TICKS 20
#define PULSE_LOW_TICKS 10

// Pulse duration for the framing bits
#define PULSE_FRAME_TICKS 10

// Enable the debug logs
#define IR_DEBUG 1

// ------------------------------------------------------------------------------

#define RMT_TX_MODE true
#define RMT_RX_MODE false

/**
 * Define the minimum and maximum acceptable pulse durations for the receiving signals.
 * Pulses with less/greater durations than this will be ignored as a noise
 */
#define RX_MIN_THRESHOLD 1
#define RX_MAX_THRESHOLD 200

class SW_Infared
{
private:
    const uint8_t pin_rmtRx[IR_SENSOR_COUNT] = {PIN_IR_RX_0, PIN_IR_RX_1, PIN_IR_RX_2, PIN_IR_RX_3};

    rmt_obj_t *rmt_send = NULL;
    rmt_data_t dataTx[128];

    int flagRmtRecieve = 0;
    int rmtValueReceived = 0;
    int rmtValueSend = 0;
    float realTickRx, realTickTx;
    char sendBits[64];

    void setupTx();
    void setupRx();

    unsigned int charToInt(char *bitArray, int len);

    unsigned int encodedValue(int value);
    unsigned int decodedValue(int value);

public:
    SW_Infared();
    ~SW_Infared();

    rmt_obj_t *rmt_rx[IR_SENSOR_COUNT];

    void begin();

    int parse(rmt_data_t *items, size_t len, uint8_t rId);

    void sendWaveform(unsigned int value);
    void sendWaveform(unsigned int value, int len);
    void sendBit(short pulseHigh, short pulseLow);

    void sendTestSignal();
    void sendTestSignal(unsigned int len);
};

#endif
