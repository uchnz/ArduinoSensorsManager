#include <LibPrintf.h>
#include <main_support.h>

uint8_t mac[6] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
char MQTTBrokerIP[] = "10.62.202.108";
const uint8_t sensor_precision = 9;
const uint16_t MQTT_KeepAlive = 600;
char topicToReadSettingsFrom[] = "/settings";

//*** SETUP HELPERS ***//
void initSystemParameters()
{
    Serial.begin(115200);
    printf_init(Serial); // using printf instead of Serial.print is much more agile
    while (!Serial)      // make a pause to initialize Serial for further output
        delay(1000);
}

// void initNetworkCard(sensor_manager::EthArduino &eth)
void initNetworkCard(EthArduino &eth)
{
    if (!eth.connect(mac))
        printf("ERROR: Failed DHCP connection with MAC: %X:%X:%X:%X:%X:%X\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5], mac[6]);
}

// void initConnectionToMQTTBroker(sensor_manager::MQTTArduino &mqtt)
void initConnectionToMQTTBroker(MQTTArduino &mqtt)
{
    mqtt.begin(MQTTBrokerIP);
    mqtt.setKeepAlive(MQTT_KeepAlive);
    mqtt.onMessage(callbackIncommingMQTTMessages);
    mqtt.connect();
    mqtt.subscribeToTopic(topicToReadSettingsFrom);
}

// void InitDallasSensors(sensor_manager::DallasArduino &dallas)
void InitDallasSensors(DallasArduino &dallas)
{
    dallas.init();
    dallas.setSensorsPrecision(sensor_precision);
}
//*** END SETUP HELPERS ***//

/////////////////////////////

//*** LOOP HELPERS ***//
bool isItTimeToParse(uint64_t millisPassedSinceLastParse, const uint16_t scanInterval)
{
    uint64_t millisNow = millis(); // don't inline millis() into abs() - see Notes and Warnings for abs() in Arduino HELP for details
    if (abs(millisNow - millisPassedSinceLastParse) > scanInterval)
        return true;
    return false;
}
//*** END LOOP HELPERS ***//

/////////////////////////////

//*** Callbacks ***//

void callbackIncommingMQTTMessages(String &topic, String &payload)
{
    printf("callback called: %s -> %s\n ", topic.c_str(), payload.c_str());
}
