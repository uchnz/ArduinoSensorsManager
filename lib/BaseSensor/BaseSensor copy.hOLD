#include <ISensor.h>
#include <Arduino.h>
#include <IOArduino.h>
#include <TimerArduino.h>

namespace basesensor_nm
{
    const uint8_t MAX_SENSOR_NAME = 30;
    const uint8_t NOT_INITIALIZED_SENSOR = 255;
    const double UNINITIALIZED_MEASUREMENT_VALUE = 0xFFFFFFFF;
}

class BaseSensor : public ISensor
{
private:
    char _sensorName[basesensor_nm::MAX_SENSOR_NAME];
    bool _sensorInitCompleted;

    bool saveName(const char *name);
    bool isInitialized();

protected:
    IIO &_io;
    ITimer *_timer;

    virtual void reset();

public:
    BaseSensor(const char *name, IIO &io);
    virtual ~BaseSensor();

    bool setName(const char *name);
    virtual bool init(ITimer *timer);

    // ISensor
    virtual bool requestCurrentMeasurement() override;
    virtual uint8_t getNumberOfConnectedSensors() override;
    virtual double getCurrentMeasurementByID(uint8_t id = 0) override;
    virtual uint8_t getName(char *name) override;
};
