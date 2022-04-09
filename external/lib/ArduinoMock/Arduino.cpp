#include <Arduino.h>

static ArduinoMock *arduinoMock = nullptr;
ArduinoMock *arduinoMockInstance()
{
    if (!arduinoMock)
        arduinoMock = new ArduinoMock();

    return arduinoMock;
}
void releaseArduinoMock()
{
    if (arduinoMock)
        delete arduinoMock;

    arduinoMock = nullptr;
}

void pinMode(uint8_t pin, uint8_t mode)
{
    assert(arduinoMock != NULL);
    arduinoMock->pinMode(pin, mode);
}

int analogRead(uint8_t)
{
    return -1;
}