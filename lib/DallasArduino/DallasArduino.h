#pragma once

#include <DallasTemperature.h>
#include <ISensor.h>
#include <Arduino.h>

namespace dallas_nm
{
    const uint8_t NUMBER_OF_MEASUREMENTS = 3;
    const int UNINITIALIZED_MEASUREMENT_VALUE = -127;
    const uint16_t DEFAULT_READING_INTERVAL = 100;
    const uint8_t MAX_SENSOR_NAME = 30;
}

class DallasArduino : public ISensor
{
private:
    DallasTemperature *_ds;
    uint8_t _totalConnectedSensors;
    char _sensorName[dallas_nm::MAX_SENSOR_NAME];
    double *_averageTemperatures;
    double **_temperatures2DArray;
    uint8_t _currentSavingItemInArray;
    uint32_t _startReadMillis;
    uint16_t _readingInterval;

    void initName(const char *name);
    void saveAverageMeasurement();
    bool isReadyForNextRead(uint32_t now);
    bool isArrayFull();

public:
    ~DallasArduino();
    DallasArduino(DallasTemperature &dt);
    bool init(const char *name, uint16_t ReadingInterval = dallas_nm::DEFAULT_READING_INTERVAL);

    // ISensor
    void requestCurrentMeasurement() override;
    uint8_t getNumberOfConnectedSensors() override;
    double getCurrentMeasurementByID(uint8_t id = 0) override;
    uint8_t getName(char *name);
};
