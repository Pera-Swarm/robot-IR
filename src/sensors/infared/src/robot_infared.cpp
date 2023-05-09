/**
 * @brief    ESP32 IR Sensor library
 * @author   Nuwan Jaliyagoda, Pasan Tennakoon, Dilshani Karunarathna
 * @version  1.0.0
 * @url      https://github.com/NuwanJ/ESP32-IR
 *
 * Licensed under, Creative Commons Attribution-ShareAlike 4.0 International License
 * ------------------------------------------------------------------------------
 */

#include "config/features.h"
#include "config/pins.h"
#include "robot_infared.h"

/**
 * @brief      Constructor of the IR Tx/Rx interface library
 * @return     void
 */
SW_Infared::SW_Infared()
{
    this->setupTx();
    this->setupRx();
}

/**
 * @brief      Destructor of the IR Tx/Rx interface library
 * @return     void
 */
SW_Infared::~SW_Infared()
{
    // Deactivate the RMT Transmitter
    if (rmtDeinit(this->rmt_send) == false && ENABLE_SERIAL_COMMUNICATION)
        Serial.println(">> error: Infared (Tx module deactivation failed)\n");

    for (int i = 0; i < IR_SENSOR_COUNT; i++)
    {
        // Deactivate the RMT Reveiver
        if (rmtDeinit(this->rmt_rx[i]) == false && ENABLE_SERIAL_COMMUNICATION)
            Serial.printf(">> error: Infared (Tx module[%d] deactivation failed)\n\n", i);
    }
}

/**
 * @brief      Begin the IR Communication module
 * @return     void
 */
void SW_Infared::begin()
{
    Serial.println(">> IR\t\t: Enabled");
    if (ENABLE_SERIAL_COMMUNICATION)
        Serial.printf("\t\t: Real tick set to: Tx: %fns | Rx: %fns\n\n", this->realTickTx, this->realTickRx);
}

/**
 * @brief      Initiate a RMT interface as a Transmitter
 * @return     void
 */
void SW_Infared::setupTx()
{
    // Setup the RMT transmitter
    if ((this->rmt_send = rmtInit(PIN_IR_TX, true, RMT_MEM_64)) == NULL)
        if (ENABLE_SERIAL_COMMUNICATION)
            Serial.println(">> error: Infared (Tx module failed)\n");

    // Update the tick frequency
    this->realTickTx = rmtSetTick(this->rmt_send, 200);

    // Generate carrier waveform of 38KHz
    for (int i = 0; i < 128; i++)
        this->dataTx[i].val = 0x80010000 + ((32) << 17) + ((32) << 1);
}

/**
 * @brief      Initiate the RMT interfaces as Receivers
 * @return     void
 */
void SW_Infared::setupRx()
{
    for (int i = 0; i < IR_SENSOR_COUNT; i++)
    {
        // Setup the RMT reveiver
        if ((this->rmt_rx[i] = rmtInit(this->pin_rmtRx[i], RMT_RX_MODE, RMT_MEM_64)) == NULL)
            if (ENABLE_SERIAL_COMMUNICATION)
                Serial.println(">> error: Infared (Rx module failed)\n");

        // Apply a filter for the incomming waveform
        rmtSetFilter(this->rmt_rx[i], true, 10);

        // Update the tick frequency
        this->realTickRx = rmtSetTick(this->rmt_rx[i], 5000); // x10 uS
    }

    // Setup the receive events -> handle in outside of this interface now
    // rmtRead(this->rmt_rx[0], this->irReceive_0);
    // rmtRead(this->rmt_rx[1], this->irReceive_1);
    // rmtRead(this->rmt_rx[2], this->irReceive_2);
    // rmtRead(this->rmt_rx[3], this->irReceive_3);
}

/**
 * @brief      Parse and Decode received IR signal
 * @return     integer
 */
int SW_Infared::parse(rmt_data_t *items, size_t len, uint8_t rId)
{
    if (len > (IR_BIT_LEN + 2) * 2)
        return -1; // Something went wrong, too much readings

    if (IR_DEBUG)
        Serial.print("<< Received\n");

    rmt_data_t *it = &items[0];
    unsigned int value = 0;

    // Trim the first and last framing bits, and iterate over the rest
    for (size_t i = 1; i < (len - 1); i++)
    {
        it = &items[i];
        if (IR_DEBUG)
            Serial.printf("%0d*\t (%d, %d), (%d, %d)\n", i, it->level0, it->duration0, it->level1, it->duration1);

        if ((it->duration0 < RX_MAX_THRESHOLD) && (it->duration1 < RX_MAX_THRESHOLD) && (it->duration0 > RX_MIN_THRESHOLD) && (it->duration1 > RX_MIN_THRESHOLD))
        {
            // Accepted signal
            bitWrite(value, i - 1, (it->duration0 > it->duration1));
        }
        else
        {
            // Invalid signal, however, mark it as 0 and print for debuugging
            bitWrite(value, i - 1, 0);
        }
    }

    if (IR_DEBUG)
        Serial.printf("Received << \t %d (%d)\n", value, rId);

    if (len == IR_BIT_LEN + 2)
        // Received value is in correct format
        return value;
    else
        // Received with some error
        return -1;
}

/**
 * @brief      Send a test signal of random number
 * @return     void
 */
void SW_Infared::sendTestSignal()
{
    unsigned int randNumber = esp_random();
    this->sendWaveform(randNumber, IR_BIT_LEN);
}

/**
 * @brief      Send a test signal of Square waves
 * @param len  The length of the waveform need to be sent, without the framing bits
 * @return     void
 */
void SW_Infared::sendTestSignal(unsigned int len)
{
    // Send the signal, with starting and ending framing bits
    for (uint8_t i = 0; i < len + 2; i++)
    {
        this->sendBit(PULSE_HIGH_TICKS, PULSE_LOW_TICKS);
    }
}

/**
 * @brief      Transmit the given value
 * @param value  unsigned integer value, that should be sent
 * @return     void
 */
void SW_Infared::sendWaveform(unsigned int value)
{
    this->sendWaveform(value, IR_BIT_LEN); // default: 32bits
}

/**
 * @brief      Transmit the given bit stream
 * @param value  unsigned integer value, that should be sent
 * @param len  length of the bits need to be transmitted. Should be less than 32
 * @return     void
 */
void SW_Infared::sendWaveform(unsigned int value, int len)
{
    uint8_t bitI;
    // Store the last value send through IR LEDs
    rmtValueSend = value;

    if (IR_DEBUG)
        Serial.print("Send \t> "); // LSB First

    // Send the start framing bit
    this->sendBit(PULSE_FRAME_TICKS, PULSE_FRAME_TICKS);

    // send signal bits
    for (uint8_t i = 0; i < len; i++)
    {
        bitI = bitRead(value, i);

        if (bitI == 1)
            this->sendBit(PULSE_HIGH_TICKS, PULSE_LOW_TICKS);
        else
            this->sendBit(PULSE_LOW_TICKS, PULSE_HIGH_TICKS);
        if (IR_DEBUG)
            Serial.print(bitI);
    }

    // Send the end framing bit
    this->sendBit(PULSE_FRAME_TICKS, PULSE_FRAME_TICKS);
    if (IR_DEBUG)
        Serial.printf("      << %ud\n", value);
}

/**
 * @brief      Transmit a given bit via RMT's Tx channel
 * @param pulseHigh No. of pulse frames should be transmittered as HIGH (0-255)
 * @param pulseLow  No. of pulse frames should be transmittered as HIGH (0-255)
 * @return     void
 */
void SW_Infared::sendBit(short pulseHigh, short pulseLow)
{
    rmtWrite(rmt_send, dataTx, pulseHigh);
    delayMicroseconds(26 * (pulseHigh + pulseLow));
}

/**
 * @brief       The function to encode a value with Error Detection scheme
 * @param value value, which should be encoded
 * @return     unsigned int encodedValue
 */
unsigned int SW_Infared::encodedValue(int value)
{
    // TODO: Consider the last 5 bits of the 32, which will be not able to send via this scheme
    char bitArray[32];
    int count = 0;

    for (int i = 1; i <= 32; ++i)
    {
        if (ceil(log2(i)) != floor(log2(i)))
            bitArray[32 - i] = bitRead(value, count++) + '0';
        else
            bitArray[32 - i] = '0';
    }

    // Debugging -------
    if (ENABLE_SERIAL_COMMUNICATION && IR_DEBUG)
    {
        for (int i = 0; i < 32; ++i)
            Serial.print(bitArray[i]);
        Serial.println();
    }
    // -----------------

    for (int i = 0; i < 6; ++i)
    {
        int evenCount = 0;
        for (int j = 1; j <= 32; ++j)
        {
            if (ceil(log2(j)) != floor(log2(j)) && bitRead(j, i) == 1)
            {
                if (bitArray[32 - j] == '1')
                    ++evenCount;
            }
        }
        if (evenCount % 2 == 0)
            bitArray[32 - ((int)(pow(2, i)))] = '0';
        else
            bitArray[32 - ((int)(pow(2, i)))] = '1';
    }

    // Debugging -------
    if (ENABLE_SERIAL_COMMUNICATION && IR_DEBUG)
    {
        for (int i = 0; i < 32; ++i)
            Serial.print(bitArray[i]);
        Serial.println();
    }
    // -----------------

    return this->charToInt(bitArray, 32);
}

/**
 * @brief       The function to decode a value with Error Detection scheme
 * @param value value, which should be decoded
 * @return     unsigned int dencodedValue
 */
unsigned int SW_Infared::decodedValue(int value)
{
    char checkBitArray[6];
    int count = 5;

    for (int i = 0; i < 6; ++i)
    {
        if (ENABLE_SERIAL_COMMUNICATION)
            Serial.printf("%d : ", i);

        int evenCount = 0;

        for (int j = 0; j < 32; ++j)
        {
            if (bitRead((j + 1), i) == 1)
            {
                if (ENABLE_SERIAL_COMMUNICATION)
                    Serial.printf("%d (%d), ", j, bitRead(value, j));
                if (bitRead(value, j) == 1)
                    ++evenCount;
            }
        }
        if (ENABLE_SERIAL_COMMUNICATION)
            Serial.println();

        if (evenCount % 2 == 0)
            checkBitArray[count--] = '0';
        else
            checkBitArray[count--] = '1';
    }

    if (ENABLE_SERIAL_COMMUNICATION && IR_DEBUG)
    {
        Serial.println("-----------Check Bits----------");
        for (int i = 0; i < 6; ++i)
            Serial.print(checkBitArray[i]);
        Serial.println();
    }

    unsigned int checkValue = charToInt(checkBitArray, 6);

    if (ENABLE_SERIAL_COMMUNICATION)
        Serial.println(checkValue);

    if (checkValue == 0)
        return value;

    char bitArray[32];
    count = 0;
    for (int i = 1; i <= 32; ++i)
        bitArray[32 - i] = bitRead(value, count++) + '0';

    if (ENABLE_SERIAL_COMMUNICATION)
    {
        for (int i = 0; i < 32; ++i)
            Serial.print(bitArray[i]);
        Serial.println();
    }

    bitArray[32 - checkValue] = !(bitArray[32 - checkValue] - '0') + '0';

    if (ENABLE_SERIAL_COMMUNICATION)
    {
        for (int i = 0; i < 32; ++i)
            Serial.print(bitArray[i]);
        Serial.println();
    }

    return this->charToInt(bitArray, 32);
}

/**
 * @brief       The function to convert a char array of '1's and '0's to int
 * @param bitArray  char* of the array
 * @param len       length of the array that should be considered
 * @example         charToInt("1001010111100100", 16)
 * @return     unsigned integer value
 */
unsigned int SW_Infared::charToInt(char *bitArray, int len)
{
    unsigned int value = 0;
    for (int i = 0; i < len; i++)
        value = (value << 1) | (bitArray[i] == '1');

    return value;
}