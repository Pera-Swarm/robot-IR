#pragma once

#include <Arduino.h>
#include "./src/robot_infared.h"

// This will enable or disable debug messages for IR communnication
#define IR_DEBUG 1

extern SW_Infared ir;

void beginInfared();
void parseRmtData(rmt_data_t *items, size_t len, uint8_t rId);

void send(int value);
void sendTestSignal(unsigned int value);

// Interrupts for Remote Rx Event
extern "C" void irReceive_0(uint32_t *data, size_t len, void *arg);
