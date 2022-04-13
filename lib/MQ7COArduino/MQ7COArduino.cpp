#include <MQ7COArduino.h>
#include <Arduino.h>
#include <MQ7Impl.h>

// class MQ7Impl
// {
// private:
//     uint32_t _phaseStartedMls;
//     PHASES _currentPhase;
//     uint32_t _previusReadingMillis;

//     bool isOutsideMillisInterval(PHASES phase, uint32_t delta);

// public:
//     MQ7Impl();

//     bool setPhase(PHASES phase);
//     bool isInPhase(PHASES phase);
//     bool isPhaseCompleted(PHASES phase);
//     bool isTimeToReadMeasurement();
// };

// MQ7Impl::MQ7Impl()
// {
//     _phaseStartedMls = 0;
//     _currentPhase = NONE;
//     _previusReadingMillis = 0;
// }

// bool MQ7Impl::setPhase(PHASES phase)
// {
//     _phaseStartedMls = millis();
//     _currentPhase = phase;

//     return true;
// }

// bool MQ7Impl::isInPhase(PHASES phase)
// {
//     return _currentPhase == phase;
// }

// bool MQ7Impl::isOutsideMillisInterval(PHASES phase, uint32_t delta)
// {
//     return phase <= delta;
// }
// bool MQ7Impl::isPhaseCompleted(PHASES phase)
// {
//     uint32_t now = millis();
//     uint32_t delta = now - _phaseStartedMls;

//     return isOutsideMillisInterval(phase, delta);
// }

// bool MQ7Impl::isTimeToReadMeasurement()
// {
//     uint32_t now = millis();
//     if (now - _previusReadingMillis < READING_INTERVAL)
//         return false;

//     _previusReadingMillis = now;

//     return true;
// }

///////////////////////////////////

void MQ7COArduino::changeSignalAndHeaterPINs(uint8_t signalPIN, uint8_t signalHeaterPIN)
{
    _signalPIN = signalPIN;
    _signalHeaterPIN = signalHeaterPIN;
    _sensorInitCompleted = false;
    // _phaseStartedMls = 0;
    // _currentPhase = NONE;
    _currentSavingItemInArray = 0;
    _averageOfAllMeasuredValues = UNINITIALIZED_MEASUREMENT_VALUE;
    // _previusReadingMillis = 0;

    for (uint8_t i = 0; i < NUMBER_OF_MEASUREMENTS; i++)
        _valueArray[i] = UNINITIALIZED_MEASUREMENT_VALUE;
}

MQ7COArduino::~MQ7COArduino()
{
    delete _pimpl;
}
MQ7COArduino::MQ7COArduino()
    : _pimpl(new MQ7Impl())
{
    changeSignalAndHeaterPINs(0, 0);
}
MQ7COArduino::MQ7COArduino(uint8_t signalPIN, uint8_t signalHeaterPIN)
    : _pimpl(new MQ7Impl())
{
    changeSignalAndHeaterPINs(signalPIN, signalHeaterPIN);
}

bool MQ7COArduino::init_new()
{
    if (_signalPIN == _signalHeaterPIN)
        return false;

    pinMode(_signalPIN, OUTPUT);
    pinMode(_signalHeaterPIN, OUTPUT);
    _sensorInitCompleted = true;
    _pimpl->setPhase(HEATING);
    setHeaterVoltageForPhase(HIGH_5_0);
    // setHeaterVoltageForPhase(HEATING);

    return true;
}
// bool MQ7COArduino::init()
// {
//     if (_signalPIN == _signalHeaterPIN)
//         return false;

//     pinMode(_signalPIN, OUTPUT);
//     pinMode(_signalHeaterPIN, OUTPUT);
//     _sensorInitCompleted = true;

//     return true;
// }

// void MQ7COArduino::setHeaterVoltageForPhase_new(PHASES phase)
// {
//     analogWrite(_signalHeaterPIN, phase == HEATING ? HIGH_5_0 : LOW_1_4);
// }

// void MQ7COArduino::setHeaterVoltageForPhase(PHASES phase)
void MQ7COArduino::setHeaterVoltageForPhase(VOLTAGE voltage)
{
    analogWrite(_signalHeaterPIN, voltage);
    // analogWrite(_signalHeaterPIN, phase == HEATING ? HIGH_5_0 : LOW_1_4);
}
// bool MQ7COArduino::setPhase(PHASES phase)
// {
//     if (!_sensorInitCompleted)
//         return false;

//     _phaseStartedMls = millis();
//     _currentPhase = phase;
//     setHeaterVoltageForPhase(phase);

//     return true;
// }

// bool MQ7COArduino::isOutsideMillisInterval(PHASES phase, uint32_t delta)
// {
//     return phase <= delta;
// }
// bool MQ7COArduino::isPhaseCompleted(PHASES phase)
// {
//     uint32_t now = millis();
//     uint32_t delta = now - _phaseStartedMls;

//     return isOutsideMillisInterval(phase, delta);
// }

// bool MQ7COArduino::isInPhase(PHASES phase)
// {
//     return _currentPhase == phase;
// }

// bool MQ7COArduino::isReadyToReadNextMeasurement(uint32_t now)
// {
//     if (now - _previusReadingMillis >= READING_INTERVAL)
//         return true;

//     return false;
// }
// bool MQ7COArduino::readNextMeasurement()
// {
//     if (READING != _currentPhase)
//         return false;

//     uint32_t now = millis();
//     if (!isReadyToReadNextMeasurement(now))
//         return false;

//     _valueArray[_currentSavingItemInArray++] = analogRead(_signalPIN);
//     _previusReadingMillis = now;

//     return true;
// }
void MQ7COArduino::readNextMeasurement_new()
{
    if (_currentSavingItemInArray < NUMBER_OF_MEASUREMENTS)
        _valueArray[_currentSavingItemInArray++] = analogRead(_signalPIN);
}

// int MQ7COArduino::calculateAverageOfAllMeasurements()
// {
//     int sumOfMeasurements = 0;
//     for (uint8_t i = 0; i < NUMBER_OF_MEASUREMENTS; i++)
//         sumOfMeasurements += _valueArray[i];

//     return sumOfMeasurements / NUMBER_OF_MEASUREMENTS;
// }
void MQ7COArduino::saveAverageMeasurement()
{
    _averageOfAllMeasuredValues = 0;
    for (uint8_t i = 0; i < NUMBER_OF_MEASUREMENTS; i++)
        _averageOfAllMeasuredValues += _valueArray[i];

    _averageOfAllMeasuredValues = _averageOfAllMeasuredValues / NUMBER_OF_MEASUREMENTS;
}

// bool MQ7COArduino::saveAverageMeasurement()
// {
//     if (!isPhaseCompleted(READING))
//         return false;

//     _averageOfAllMeasuredValues = calculateAverageOfAllMeasurements();

//     return true;
// }

uint8_t MQ7COArduino::getNumberOfConnectedSensors()
{
    return NUMBER_OF_SENSORS_ON_BUS;
}

float MQ7COArduino::getCurrentMeasurementByID(uint8_t id)
{
    return _averageOfAllMeasuredValues;
}

void MQ7COArduino::requestCurrentMeasurement()
{
    //     if (isInPhase(HEATING))
    //     {
    //         if (!isPhaseCompleted(HEATING))
    //             return;
    //         setPhase(COOLING);
    //     }
    //     if (isInPhase(COOLING))
    //     {
    //         if (!isPhaseCompleted(COOLING))
    //             return;
    //         setPhase(READING);
    //     }
    //     if (isInPhase(READING))
    //     {
    //         readNextMeasurement();
    //         if (!isPhaseCompleted(READING))
    //             return;
    //         saveAverageMeasurement();
    //     }
    //     setPhase(HEATING);
}

void MQ7COArduino::requestCurrentMeasurement_new()
{
    if (_pimpl->isInPhase(HEATING))
    {
        if (!_pimpl->isPhaseCompleted(HEATING))
            return;
        _pimpl->setPhase(COOLING);
        //        setHeaterVoltageForPhase(COOLING);
        setHeaterVoltageForPhase(LOW_1_4);
    }
    if (_pimpl->isInPhase(COOLING))
    {
        if (!_pimpl->isPhaseCompleted(COOLING))
            return;
        _pimpl->setPhase(READING);
    }
    if (_pimpl->isInPhase(READING))
    {
        if (!_pimpl->isTimeToReadMeasurement())
            return;
        readNextMeasurement_new();
        if (!_pimpl->isPhaseCompleted(READING))
            return;
        saveAverageMeasurement();
    }
    _pimpl->setPhase(HEATING);
    // setHeaterVoltageForPhase(HEATING);
    setHeaterVoltageForPhase(HIGH_5_0);
}