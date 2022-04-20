#include <gmock/gmock.h>
#include <stdint.h>

class OneWire
{
public:
    OneWire(){};
    OneWire(uint8_t pin){};

    MOCK_METHOD(void, begin, (uint8_t));
    MOCK_METHOD(void, reset_search, ());
};

class DallasTemperature
{
public:
    DallasTemperature(){};
    DallasTemperature(OneWire *){};
    DallasTemperature(OneWire *, uint8_t){};

    MOCK_METHOD(void, setOneWire, (OneWire *));
    MOCK_METHOD(void, begin, ());
    MOCK_METHOD(uint8_t, getDeviceCount, ());
    MOCK_METHOD(void, requestTemperatures, ());
    MOCK_METHOD(float, getTempCByIndex, (uint8_t));
};
