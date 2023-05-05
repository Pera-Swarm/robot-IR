#include "infared.h"

SW_Infared ir;

// NOTE: These code block is originally placed in setup.ino, and moved to here
// Functionality isn't checked yet

// Interrupts for Remote Rx Event
extern "C" void irReceive_0(uint32_t *data, size_t len, void *arg)
{
    // unsigned int value = ir.parse((rmt_data_t *)data, len, 1);
    // Serial.printf("\n Received: %d at %d with len=%d\n--------------\n", value, 1, len);
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
    // rmtRead(ir.rmt_rx[1], irReceive_1, NULL);
    // rmtRead(ir.rmt_rx[2], irReceive_2, NULL);
    // rmtRead(ir.rmt_rx[3], irReceive_3, NULL);
    ir.begin();

    delay(3000);
    ir.sendTestSignal(3);
    delay(2500);
    ir.sendTestSignal(3);
}

void parseRmtData(rmt_data_t *items, size_t len, uint8_t rId)
{
    // IR Protocols can be programmed in here
    for (int i = 0; i < len; i++)
    {
        rmt_data_t *it = &items[i];
        Serial.printf(">> %2d/%2d - L(%d %d) D(%d %d) V()\n", i, len, it->level0, it->level1, it->duration0, it->duration1, it->val);
    }
    Serial.print("\n");

    // unsigned int value = ir.parse(items, len, rId);
    // Serial.printf("Received: %d at %d\n", value, rId);
}

void send(int value)
{
    // ir.sendTestSignal(3);
    // ir.sendWaveform(value);
}
