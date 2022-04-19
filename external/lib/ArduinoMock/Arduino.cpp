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

void analogWrite(uint8_t pin, uint8_t value)
{
    assert(arduinoMock != NULL);
    arduinoMock->analogWrite(pin, value);
}

int analogRead(uint8_t pin)
{
    assert(arduinoMock != NULL);
    return arduinoMock->analogRead(pin);
}

void digitalWrite(uint8_t pin, uint8_t value)
{
    assert(arduinoMock != NULL);
    arduinoMock->digitalWrite(pin, value);
}

int digitalRead(uint8_t pin)
{
    assert(arduinoMock != NULL);
    return arduinoMock->digitalRead(pin);
}

uint32_t millis()
{
    assert(arduinoMock != NULL);
    return arduinoMock->millis();
}