#include "infared.h"

SW_Infared ir;

// Interrupts for Remote Rx Event
extern "C" void irReceive_0(uint32_t *data, size_t len)
{
    parseRmtData((rmt_data_t *)data, len, 0);
}

void beginInfared()
{
    rmtRead(ir.rmt_rx[0], irReceive_0);
    ir.begin();
}

void parseRmtData(rmt_data_t *items, size_t len, uint8_t rId)
{
    // IR Protocols can be programmed in here

    // rmt_data_t *it = &items[0];
    // // unsigned int value = 0;

    // Serial.print("\n<< Received\n"); // LSB First

    // for (size_t i = 0; i < len; i++)
    // {
    //     it = &items[i];
    //     // Serial.printf("%0d\t (%d, %d), (%d, %d)\n", i, it->level0, it->duration0, it->level1, it->duration1);

    //     if ((it->duration0 < RX_MAX_THRESHOLD) && (it->duration1 < RX_MAX_THRESHOLD) && (it->duration0 > RX_MIN_THRESHOLD) && (it->duration1 > RX_MIN_THRESHOLD))
    //     {
    //         Serial.printf("%0d*\t (%d, %d), (%d, %d)\n", i, it->level0, it->duration0, it->level1, it->duration1);
    //     }
    //     else
    //     {
    //         // For debug
    //         Serial.printf("%0d \t (%d, %d), (%d, %d)\n", i, it->level0, it->duration0, it->level1, it->duration1);
    //     }
    // }
    unsigned int value = ir.parse(items, len, rId);
    Serial.println(value);
}

void send(int value)
{
    ir.sendWaveform(value);
}

void sendTestSignal(unsigned int value)
{
    ir.sendTestSignal(value);
}
