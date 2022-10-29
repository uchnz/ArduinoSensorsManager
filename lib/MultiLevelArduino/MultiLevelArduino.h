#pragma once
#include <ISensor.h>
#include <IIOSensor.h>
#include <TimerArduino.h>
#include <IOArduino.h>

class MultiLevelArduino : public ISensor
{
protected:
    IIOSensor *_io;
    uint8_t _totalIOs;

public:
    MultiLevelArduino(const char *name, IIOSensor &io);
    MultiLevelArduino(const char *name, IIOSensor **io, uint8_t ioNum);
    virtual ~MultiLevelArduino(){};

    // ISensor
    virtual bool requestCurrentMeasurement() override;
    virtual uint8_t getNumberOfConnectedSensors() override;
    virtual double getCurrentMeasurementByID(uint8_t id = 0) override;
    virtual uint8_t getName(char *name) override;

    // double getCurrentMeasurementByID(uint8_t id = 0) override;
};

// class MultiPortIOArduino : public IIOSensor
// {
// private:
//     uint8_t _totalSensors;

// public:
//     MultiPortIOArduino(IIOSensor **ios, uint8_t num);

//     // IIOSensor
//     virtual bool init() override;
//     virtual double read(uint8_t id = 0) override;
//     virtual uint8_t getTotalSensors() override;
// };