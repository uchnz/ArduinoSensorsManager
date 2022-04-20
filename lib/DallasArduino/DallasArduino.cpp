//#include <LibPrintf.h>

#include <DallasArduino.h>

// #define ONE_WIRE_BUS_DEFAULT_PIN 22

// DallasArduino::DallasArduino(DallasTemperature &dt) //, OneWire &ow)
//     : _ds(&dt)                                      //, _ow(&ow)
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

DallasArduino::DallasArduino(uint8_t sensorPIN)
{
    //     _oneWire.begin(sensorPIN);
    //     _sensors.setOneWire(&_oneWire);
}

DallasArduino::DallasArduino()
{
    // _oneWire.begin(ONE_WIRE_BUS_DEFAULT_PIN);
    // _sensors.setOneWire(&_oneWire);
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

// void DallasArduino::showNumberOfFoundSensors()
// {
//     int num = _sensors.getDeviceCount();
//     // printf("Locating Dallas Sensors... Found %d devices\n", num);
// }

// void DallasArduino::showParasitePowerMode()
// {
//     // bool state = _sensors.isParasitePowerMode();
//     // printf("Parasite power mode: %s \n", state ? "ON" : "OFF");
// }

// void DallasArduino::showSensorsAddresses()
// {
//     // _oneWire.reset_search();
//     // // Serial.println("Listing sensors' addresses:");
//     // String messageWithAddressFormatted = "";
//     // int num = _sensors.getDeviceCount();
//     // DeviceAddress devAddress;
//     // for (int i = 0; i < num; i++)
//     // {
//     //     String addressFormated = "Unable to find an address of the sensor.";
//     //     if (_sensors.getAddress(devAddress, i))
//     //         addressFormated = getAddressString(devAddress);
//     //     messageWithAddressFormatted = "Sensor[" + String(i) + "] Address: " + addressFormated;
//     //     // Serial.println(messageWithAddressFormatted);
//     // }
// }

// String DallasArduino::getAddressString(DeviceAddress deviceAddress)
// {
//     String addr = "";
//     for (int i = 0; i < 8; i++)
//     {
//         addr += String(deviceAddress[i], HEX);
//     }
//     return addr;
// }

bool DallasArduino::init(uint16_t readingInterval)
{
    if (_totalConnectedSensors > 0)
        return false;

    _ds->begin();
    _totalConnectedSensors = _ds->getDeviceCount();
    if (!_totalConnectedSensors)
        return false;

    _readingInterval = readingInterval;

    _averageTemperatures = new float[_totalConnectedSensors];
    for (uint8_t i = 0; i < _totalConnectedSensors; i++)
        _averageTemperatures[i] = dallas_nm::UNINITIALIZED_MEASUREMENT_VALUE;

    _temperatures2DArray = new float *[_totalConnectedSensors];
    for (uint8_t i = 0; i < _totalConnectedSensors; i++)
        _temperatures2DArray[i] = new float[dallas_nm::NUMBER_OF_MEASUREMENTS];

    for (uint8_t i = 0; i < _totalConnectedSensors; i++)
        for (uint8_t j = 0; j < dallas_nm::NUMBER_OF_MEASUREMENTS; j++)
            _temperatures2DArray[i][j] = dallas_nm::UNINITIALIZED_MEASUREMENT_VALUE;

    return true;

    //    _sensors.begin();

    //    _ow->begin(22);
    // _ds->setOneWire(_ow);

    //     showNumberOfFoundSensors();
    //     showParasitePowerMode();
    //     showSensorsAddresses();
}

// void DallasArduino::setSensorsPrecision(int precision)
// {
//     _oneWire.reset_search();
//     // printf("Set sensors' precision to: %d\n", precision);
//     int num = _sensors.getDeviceCount();
//     DeviceAddress devAddress;
//     for (int i = 0; i < num; i++)
//     {
//         if (_sensors.getAddress(devAddress, i))
//             _sensors.setResolution(devAddress, precision);
//     }
// }

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

// void DallasArduino::requestCurrentMeasurement()
// {
//     if (!_totalConnectedSensors)
//         return;

//     // _ds->requestTemperatures();
//     // _average[0] = 36.6;
//     // _average[1] = -17.3;
//     // _average[2] = 0;

//     uint32_t now = millis();
//     if (!isReadyForNextRead(now))
//         return;

//     if (isArrayFull())
//         saveAverageMeasurement();

//     for (uint8_t i = 0; i < _totalConnectedSensors; i++)
//     {
//         _averageTemperatures[i] = _ds->getTempCByIndex(i);
//     }

//     // Serial.print("Requesting temperatures...");
//     // _sensors.requestTemperatures();
//     // Serial.println("DONE");
// }

uint8_t DallasArduino::getNumberOfConnectedSensors()
{
    // return _sensors.getDeviceCount();
    return _totalConnectedSensors;
}

float DallasArduino::getCurrentMeasurementByID(uint8_t id)
{
    if (!_totalConnectedSensors)
        return dallas_nm::UNINITIALIZED_MEASUREMENT_VALUE;

    if (id >= _totalConnectedSensors)
        return dallas_nm::UNINITIALIZED_MEASUREMENT_VALUE;

    // float t = _sensors.getTempCByIndex(id);
    // if (DEVICE_DISCONNECTED_C == t)
    //     ;
    // printf("[ERROR: Device disconnected. Could not read temperature data]\n");
    // return t;

    // /    return _average[id];
    return _averageTemperatures[id];
}
