#include <MQ7COArduino.h>
#include <MQ7Impl.h>

void MQ7COArduino::changeSignalAndHeaterPINs(uint8_t signalPIN, uint8_t signalHeaterPIN)
{
    _signalPIN = signalPIN;
    _signalHeaterPIN = signalHeaterPIN;
    _sensorInitCompleted = false;
    _currentSavingItemInArray = 0;
    _averageOfAllMeasuredValues = mq7co_nm::UNINITIALIZED_MEASUREMENT_VALUE;

    for (uint8_t i = 0; i < mq7co_nm::NUMBER_OF_MEASUREMENTS; i++)
        _valueArray[i] = mq7co_nm::UNINITIALIZED_MEASUREMENT_VALUE;
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

void MQ7COArduino::initName(const char *name)
{
    int nameLengthWithNull = strlen(name) + 1;
    if (nameLengthWithNull > mq7co_nm::MAX_SENSOR_NAME)
        nameLengthWithNull = mq7co_nm::MAX_SENSOR_NAME - 1;

    memcpy(_sensorName, name, nameLengthWithNull);
}
bool MQ7COArduino::init(const char *name, uint16_t ReadingInterval)
{
    if (_signalPIN == _signalHeaterPIN)
        return false;

    pinMode(_signalPIN, INPUT);
    pinMode(_signalHeaterPIN, OUTPUT);
    _sensorInitCompleted = true;
    _pimpl->resetOnInit(ReadingInterval);
    _pimpl->setPhase(mq7impl::HEATING);
    setHeaterVoltageForPhase(mq7co_nm::HIGH_5_0);

    return true;
}
void MQ7COArduino::setHeaterVoltageForPhase(mq7co_nm::VOLTAGE voltage)
{
    analogWrite(_signalHeaterPIN, voltage);
}

void MQ7COArduino::readNextMeasurement()
{
    if (_currentSavingItemInArray < mq7co_nm::NUMBER_OF_MEASUREMENTS)
        _valueArray[_currentSavingItemInArray++] = analogRead(_signalPIN);
}

void MQ7COArduino::resetArrayCounter()
{
    _currentSavingItemInArray = 0;
}
void MQ7COArduino::saveAverageMeasurement()
{
    _averageOfAllMeasuredValues = 0;
    for (uint8_t i = 0; i < _currentSavingItemInArray; i++)
        _averageOfAllMeasuredValues += _valueArray[i];

    _averageOfAllMeasuredValues = _averageOfAllMeasuredValues / _currentSavingItemInArray;
}

uint8_t MQ7COArduino::getNumberOfConnectedSensors()
{
    return mq7co_nm::NUMBER_OF_SENSORS_ON_BUS;
}

double MQ7COArduino::getCurrentMeasurementByID(uint8_t id)
{
    return _averageOfAllMeasuredValues;
}

bool MQ7COArduino::requestCurrentMeasurement()
{
    if (!_sensorInitCompleted)
        return false;

    if (_pimpl->isInPhase(mq7impl::HEATING))
    {
        if (!_pimpl->isPhaseCompleted(mq7impl::HEATING))
            return false;
        _pimpl->setPhase(mq7impl::COOLING);
        setHeaterVoltageForPhase(mq7co_nm::LOW_1_4);
    }
    if (_pimpl->isInPhase(mq7impl::COOLING))
    {
        if (!_pimpl->isPhaseCompleted(mq7impl::COOLING))
            return false;
        _pimpl->setPhase(mq7impl::READING);
    }
    if (_pimpl->isInPhase(mq7impl::READING))
    {
        if (!_pimpl->isTimeToReadMeasurement())
            return false;
        readNextMeasurement();
        if (!_pimpl->isPhaseCompleted(mq7impl::READING))
            return false;
        saveAverageMeasurement();
        resetArrayCounter();
    }
    _pimpl->setPhase(mq7impl::HEATING);
    setHeaterVoltageForPhase(mq7co_nm::HIGH_5_0);

    return true;
}

// uint8_t MQ7COArduino::getName(char *&name)
uint8_t MQ7COArduino::getName(char *name)
{
    return 0;
}