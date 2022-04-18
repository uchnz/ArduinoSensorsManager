#pragma once
#include <Arduino.h>
#include <ISensor.h>
#include <iarduino_Pressure_BMP.h> // BMP280 is a device with 3 integrated sensors: temperature, pressure, altitude.
                                   //  id 0 - will be temperature
                                   //  id 1 - will be pressure
                                   //  id 2 - will be altitude

namespace bmp_nm
{
    const uint8_t NUMBER_OF_MEASUREMENTS = 3;
    const int UNINITIALIZED_MEASUREMENT_VALUE = -127;
    const uint8_t NUMBER_OF_SENSORS_ON_BUS = 3;
    const uint16_t DEFAULT_READING_INTERVAL = 3000;
    const uint8_t TEMPERATURE_ID = 0;
    const uint8_t PRESSURE_ID = 1;
    const uint8_t ALTITUDE_ID = 2;
}

class BMP280Arduino : public ISensor
{
private:
    iarduino_Pressure_BMP &_bmp;
    bool _sensorInitCompleted;
    float _sensorTemperatureArray[bmp_nm::NUMBER_OF_MEASUREMENTS];
    float _sensorPressureArray[bmp_nm::NUMBER_OF_MEASUREMENTS];
    float _sensorAltitudeArray[bmp_nm::NUMBER_OF_MEASUREMENTS];
    uint8_t _currentSavingItemInArray;
    float _temperatureAverage;
    float _pressureAverage;
    float _altitudeAverage;
    uint32_t _startReadMillis;
    uint16_t _readingInterval;

    void saveAverageMeasurement();
    bool isReadyForNextRead(uint32_t now);
    bool isArrayFull();

public:
    BMP280Arduino(iarduino_Pressure_BMP &bmp);
    bool init(uint16_t ReadingInterval = bmp_nm::DEFAULT_READING_INTERVAL);
    uint8_t getAddress();

    void requestCurrentMeasurement() override;
    uint8_t getNumberOfConnectedSensors() override;
    float getCurrentMeasurementByID(uint8_t id = 0) override;
};
