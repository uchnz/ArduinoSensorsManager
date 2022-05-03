#pragma once
#include <BaseSensor.h>
#include <iarduino_I2C_SHT.h> // SHT20 is a device with 2 integrated sensors: temperature and humidity.
                              //  id 0 - will be temperature
                              //  id 1 - will be humidity

namespace sht_nm
{
    const uint8_t NUMBER_OF_MEASUREMENTS = 3;
}

class SHT20Arduino : public BaseSensor
{
private:
    uint8_t _currentSavingItemInArray;
    double *_averageMeasurementsArray;
    double **_measurements2DArray;

    void freeMemory();
    void createMeasurementsNewArrays();
    void saveAverageMeasurement();
    bool isArrayFull();

protected:
    virtual void reset();

public:
    SHT20Arduino(const char *name, IIO &io);
    virtual ~SHT20Arduino();

    virtual bool init(ITimer *timer) override;

    virtual bool requestCurrentMeasurement() override;
    virtual uint8_t getNumberOfConnectedSensors() override;
    virtual double getCurrentMeasurementByID(uint8_t id = 0) override;
};
