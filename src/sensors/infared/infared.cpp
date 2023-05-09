#include "infared.h"

SW_Infared ir;

// Interrupts for Remote Rx Event
extern "C" void irReceive_0(uint32_t *data, size_t len)
{
    parseRmtData((rmt_data_t *)data, len, 0);
}
extern "C" void irReceive_1(uint32_t *data, size_t len)
{
    parseRmtData((rmt_data_t *)data, len, 1);
}
extern "C" void irReceive_2(uint32_t *data, size_t len)
{
    parseRmtData((rmt_data_t *)data, len, 2);
}
extern "C" void irReceive_3(uint32_t *data, size_t len)
{
    parseRmtData((rmt_data_t *)data, len, 3);
}

void beginInfared()
{
    rmtRead(ir.rmt_rx[0], irReceive_0);
    ir.begin();
}

void parseRmtData(rmt_data_t *items, size_t len, uint8_t rId)
{
    unsigned int value = ir.parse(items, len, rId);

    if (value == -1)
        Serial.println(">> error: Infared (Parse failed)\n");
    else
        Serial.println(value);

    // IR Protocols can be programmed in here
    // ...
}

void send(int value)
{
    ir.sendWaveform(value);
}

void sendTestSignal(unsigned int value)
{
    ir.sendTestSignal(value);
}
