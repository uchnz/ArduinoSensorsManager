#include <LibPrintf.h>

#include <DallasArduino.h>

namespace sensor_manager
{

#define TEMPERATURE_PRECISION 9
#define ONE_WIRE_BUS 22
#define TEMPERATURE_PRECISION 9

    DallasArduino::DallasArduino()
    {
        _oneWire.begin(ONE_WIRE_BUS);
        _sensors.setOneWire(&_oneWire);
    }

    void DallasArduino::showNumberOfFoundSensors()
    {
        int num = _sensors.getDeviceCount();
        printf("Locating Dallas Sensors... Found %d devices\n", num);
    }

    void DallasArduino::showParasitePowerMode()
    {
        bool state = _sensors.isParasitePowerMode();
        printf("Parasite power mode: %s \n", state ? "ON" : "OFF");
    }

    void DallasArduino::showSensorsAddresses()
    {
        _oneWire.reset_search();
        Serial.println("Listing sensors' addresses:");
        String messageWithAddressFormatted = "";
        int num = _sensors.getDeviceCount();
        DeviceAddress devAddress;
        for (int i = 0; i < num; i++)
        {
            String addressFormated = "Unable to find an address of the sensor.";
            if (_sensors.getAddress(devAddress, i))
                addressFormated = getAddressString(devAddress);
            messageWithAddressFormatted = "Sensor[" + String(i) + "] Address: " + addressFormated;
            Serial.println(messageWithAddressFormatted);
        }
    }

    String DallasArduino::getAddressString(DeviceAddress deviceAddress)
    {
        String addr = "";
        for (int i = 0; i < 8; i++)
        {
            addr += String(deviceAddress[i], HEX);
        }
        return addr;
    }

    void DallasArduino::initSensors()
    {
        _sensors.begin();
        showNumberOfFoundSensors();
        showParasitePowerMode();
        showSensorsAddresses();
    }

    void DallasArduino::setSensorsPrecision(int precision)
    {
        _oneWire.reset_search();
        printf("Set sensors' precision to: %d\n", precision);
        int num = _sensors.getDeviceCount();
        DeviceAddress devAddress;
        for (int i = 0; i < num; i++)
        {
            if (_sensors.getAddress(devAddress, i))
                _sensors.setResolution(devAddress, precision);
        }
    }

    void DallasArduino::requestCurrentTemperatures()
    {
        Serial.print("Requesting temperatures...");
        _sensors.requestTemperatures();
        Serial.println("DONE");
    }

    uint8_t DallasArduino::getNumberOfSensors()
    {
        return _sensors.getDeviceCount();
    }

    float DallasArduino::getTemperatureByID(uint8_t id)
    {
        float t = _sensors.getTempCByIndex(id);
        if (DEVICE_DISCONNECTED_C == t)
            printf("[ERROR: Device disconnected. Could not read temperature data]\n");
        return t;
    }

    char *DallasArduino::getStringAddressByID(uint8_t id)
    {
        DeviceAddress devAddress;
        _sensors.getAddress(devAddress, id);
        String address = getAddressString(devAddress);
        char addressConverted[8] = "";
        address.toCharArray(addressConverted, 8);

        return addressConverted;
    }
}
