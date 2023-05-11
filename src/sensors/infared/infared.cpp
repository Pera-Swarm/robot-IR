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
    // Enable the module
    ir.begin();

    // Attach the IR Rx events
    rmtRead(ir.rmt_rx[0], irReceive_0);
    // rmtRead(ir.rmt_rx[1], irReceive_1);
    // rmtRead(ir.rmt_rx[2], irReceive_2);
    // rmtRead(ir.rmt_rx[3], irReceive_3);
}

void parseRmtData(rmt_data_t *items, size_t len, uint8_t rId)
{
    // Read and process the incoming signals
    unsigned int value = ir.parse(items, len, rId);

    if (value == -1)
        Serial.println(">> error: Infared (Parse failed)\n");

    // IR Protocols can be programmed in here
    // ...
}

void send(int value)
{
    // Send the given value
    ir.sendWaveform(value);
}

void sendTestSignal()
{
    // Send a test signal value with random value
    ir.sendTestSignal();
}

void sendTestSignal(unsigned int len)
{
    // Send a test signal value with given length
    ir.sendTestSignal(len);
}
