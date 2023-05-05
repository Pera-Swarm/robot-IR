
#include <Arduino.h>

#include "define.h"
#include "config/features.h" // Enable or Diasable services

// This is to hide non-test related source code.
// https://docs.platformio.org/en/latest/plus/unit-testing.html
#ifndef UNIT_TEST

int i;

void setup()
{
    // put your setup code here, to run once:

    // Enables Serial Communication with baudRate of 115200
    Serial.begin(115200);
    Serial.println("PlatformIO ESP32 Boilerplate started...");

    pinMode(PIN_LED_INBUILT, OUTPUT);
    beginInfared();

    i = 100;
}

void loop()
{
    digitalWrite(PIN_LED_INBUILT, HIGH);
    send(i++);
    delay(500);
    digitalWrite(PIN_LED_INBUILT, LOW);
    delay(4500);
}

#endif
