#include <DallasArduino.h>

DallasArduino::DallasArduino(DallasTemperature &dt)
    : _ds(&dt)
{
    _totalConnectedSensors = 0;
    _averageTemperatures = nullptr;
    _temperatures2DArray = nullptr;
    _currentSavingItemInArray = 0;
    _startReadMillis = 0;
    _readingInterval = dallas_nm::DEFAULT_READING_INTERVAL;
}

DallasArduino::~DallasArduino()
{
    if (_averageTemperatures)
        delete _averageTemperatures;

    if (_temperatures2DArray)
    {
        for (uint8_t i = 0; i < _totalConnectedSensors; i++)
            delete[] _temperatures2DArray[i];

        delete[] _temperatures2DArray;
    }
}

void DallasArduino::initName(const char *name)
{
    int nameLengthWithNull = strlen(name) + 1;
    if (nameLengthWithNull > dallas_nm::MAX_SENSOR_NAME)
        nameLengthWithNull = dallas_nm::MAX_SENSOR_NAME - 1;

    memcpy(_sensorName, name, nameLengthWithNull);
}
bool DallasArduino::init(const char *name, uint16_t ReadingInterval)
{
    if (_totalConnectedSensors > 0)
        return false;

    _ds->begin();
    _totalConnectedSensors = _ds->getDeviceCount();
    if (!_totalConnectedSensors)
        return false;

    _readingInterval = ReadingInterval;

    _averageTemperatures = new double[_totalConnectedSensors];
    for (uint8_t i = 0; i < _totalConnectedSensors; i++)
        _averageTemperatures[i] = dallas_nm::UNINITIALIZED_MEASUREMENT_VALUE;

    _temperatures2DArray = new double *[_totalConnectedSensors];
    for (uint8_t i = 0; i < _totalConnectedSensors; i++)
        _temperatures2DArray[i] = new double[dallas_nm::NUMBER_OF_MEASUREMENTS];

    for (uint8_t i = 0; i < _totalConnectedSensors; i++)
        for (uint8_t j = 0; j < dallas_nm::NUMBER_OF_MEASUREMENTS; j++)
            _temperatures2DArray[i][j] = dallas_nm::UNINITIALIZED_MEASUREMENT_VALUE;

    return true;
}

void DallasArduino::saveAverageMeasurement()
{
    for (uint8_t i = 0; i < _totalConnectedSensors; i++)
        _averageTemperatures[i] = 0;

    for (uint8_t i = 0; i < _totalConnectedSensors; i++)
        for (uint8_t j = 0; j < _currentSavingItemInArray; j++)
            _averageTemperatures[i] += _temperatures2DArray[i][j];

    for (uint8_t i = 0; i < _totalConnectedSensors; i++)
        _averageTemperatures[i] = _averageTemperatures[i] / _currentSavingItemInArray;

    _currentSavingItemInArray = 0;
}
bool DallasArduino::isReadyForNextRead(uint32_t now)
{
    return (now - _startReadMillis) >= _readingInterval;
}
bool DallasArduino::isArrayFull()
{
    return (_currentSavingItemInArray > dallas_nm::NUMBER_OF_MEASUREMENTS - 1);
}
void DallasArduino::requestCurrentMeasurement()
{
    if (!_totalConnectedSensors)
        return;

    uint32_t now = millis();
    if (!isReadyForNextRead(now))
        return;

    if (isArrayFull())
        saveAverageMeasurement();

    _ds->requestTemperatures();
    for (uint8_t i = 0; i < _totalConnectedSensors; i++)
        _temperatures2DArray[i][_currentSavingItemInArray] = _ds->getTempCByIndex(i);

    _currentSavingItemInArray++;
    _startReadMillis = millis();
}

uint8_t DallasArduino::getNumberOfConnectedSensors()
{
    return _totalConnectedSensors;
}

double DallasArduino::getCurrentMeasurementByID(uint8_t id)
{
    if (!_totalConnectedSensors)
        return dallas_nm::UNINITIALIZED_MEASUREMENT_VALUE;

    if (id >= _totalConnectedSensors)
        return dallas_nm::UNINITIALIZED_MEASUREMENT_VALUE;

    return _averageTemperatures[id];
}

// uint8_t DallasArduino::getName(char *&name)
uint8_t DallasArduino::getName(char *name)
{
    return 0;
}