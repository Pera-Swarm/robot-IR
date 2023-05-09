
#include <Arduino.h>

#include "define.h"
#include "config/features.h" // Enable or Diasable services

// This is to hide non-test related source code.
// https://docs.platformio.org/en/latest/plus/unit-testing.html
#ifndef UNIT_TEST

void setup()
{
    // put your setup code here, to run once:

    // Enables Serial Communication with baudRate of 115200
    Serial.begin(115200);
    Serial.println("PlatformIO ESP32 Boilerplate started...");

    pinMode(PIN_LED_INBUILT, OUTPUT);

    beginInfared();
    delay(2000);
}

int i = 100;

void loop()
{
    // put your main code here, to run repeatedly:
    // sendTestSignal(5);
    send(i++);
    delay(6000);
}

#endif
