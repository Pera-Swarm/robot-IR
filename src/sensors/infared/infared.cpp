#include "infared.h"

SW_Infared ir;

// NOTE: These code block is originally placed in setup.ino, and moved to here
// Functionality isn't checked yet

// Interrupts for Remote Rx Event
extern "C" void irReceive_0(uint32_t *data, size_t len, void *arg)
{
    parseRmtData((rmt_data_t *)data, len, 0);
}
extern void irReceive_1(uint32_t *data, size_t len, void *arg)
{
    parseRmtData((rmt_data_t *)data, len, 1);
}
extern void irReceive_2(uint32_t *data, size_t len, void *arg)
{
    parseRmtData((rmt_data_t *)data, len, 2);
}
extern void irReceive_3(uint32_t *data, size_t len, void *arg)
{
    parseRmtData((rmt_data_t *)data, len, 3);
}

void beginInfared()
{
    rmtRead(ir.rmt_rx[0], irReceive_0, NULL);
    rmtRead(ir.rmt_rx[1], irReceive_1, NULL);
    rmtRead(ir.rmt_rx[2], irReceive_2, NULL);
    rmtRead(ir.rmt_rx[3], irReceive_3, NULL);
    ir.begin();
}

void parseRmtData(rmt_data_t *items, size_t len, uint8_t rId)
{
    // IR Protocols can be programmed in here

    unsigned int value = ir.parse(items, len, rId);
    // Serial.println(value);
}

void send(int value)
{
    ir.sendWaveform(value);
}
