#include <LibPrintf.h>
#include <main_support.h>

uint8_t mac[6] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
char MQTTBrokerIP[] = "10.62.202.108";
const uint8_t sensor_precision1 = 9;
const uint8_t sensor_precision2 = 12;
const uint16_t MQTT_KeepAlive = 600;
char topicToReadSettingsFrom[] = "/settings";

//*** SETUP HELPERS ***//
void initSystemParameters()
{
#if ARDUINO
    delay(1000); // wait for stability on some boards to prevent garbage Serial
#endif
    Serial.begin(115200);
    printf_init(Serial); // using printf instead of Serial.print is much more agile
    while (!Serial)      // make a pause to initialize Serial for further output
        delay(1000);
}
void initNetworkCard(EthArduino &eth)
{
    if (!eth.connect(mac))
        printf("ERROR: Failed DHCP connection with MAC: %X:%X:%X:%X:%X:%X\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5], mac[6]);
}
void initConnectionToMQTTBroker(MQTTArduino &mqtt)
{
    mqtt.begin(MQTTBrokerIP);
    mqtt.setKeepAlive(MQTT_KeepAlive);
    mqtt.onMessage(callbackIncommingMQTTMessages);
    mqtt.connect();
    mqtt.subscribeToTopic(topicToReadSettingsFrom);
}
void InitDallasSensor(DallasArduino &dallas)
{
    dallas.init();
    dallas.setSensorsPrecision(sensor_precision1);
}
void InitSASSensor(SASArduino &sas)
{
    sas.init(500);
}
void InitMQ7COSensor(MQ7COArduino &mq7co)
{
    mq7co.init(7000);
}
void InitSHT20Sensor(SHT20Arduino &sht20)
{
    sht20.init(1000);
}
void InitBMP280Sensor(BMP280Arduino &bmp280)
{
    bmp280.init(1000);
}

void initFloatArduinoSensor(FloatArduino &fl)
{
    fl.init();
}
//*** END SETUP HELPERS ***//

//*** LOOP HELPERS ***//
bool isItTimeToParse(uint32_t millisPassedSinceLastParse, const uint16_t scanInterval)
{
    uint32_t millisNow = millis();
    if ((millisNow - millisPassedSinceLastParse) > scanInterval)
        return true;

    return false;
}
//*** END LOOP HELPERS ***//

//*** Callbacks ***//
void callbackIncommingMQTTMessages(String &topic, String &payload)
{
    printf("callback called: %s -> %s\n ", topic.c_str(), payload.c_str());
}
