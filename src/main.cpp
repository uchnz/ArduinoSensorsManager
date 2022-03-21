#include <LibPrintf.h>

#include <EthArduino.h>
#include <MQTTArduino.h>
#include <DallasArduino.h>
#include <SensorManager.h>

sensor_manager::EthArduino eth;
sensor_manager::MQTTArduino mqtt;
sensor_manager::DallasArduino dallas;
sensor_manager::SensorManager mgr(eth, mqtt, dallas);
byte arduinoEthernetMAC[6] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
char MQTTBrokerIP[] = "10.62.202.106";
// char MQTTBrokerIP[] = "public.cloud.shiftr.io";

char *topics[] = {"/UZV1/temp1", "/UZV2/temp1"};

void setup()
{
    Serial.begin(115200);
    printf_init(Serial); // using printf instead of Serial.print is much more agile
    delay(1000);         // make a pause after begin() to clearly display further messages

    printf("\n");

    printf("Sarting setup...");
    dallas.init();
    dallas.setSensorsPrecision(9);
    mgr.initSensors();

    mgr.fillTopicsStrings(topics, 2);

    if (!mgr.connectWithDHCP(arduinoEthernetMAC))
        return;

    if (!mgr.initMQTT(MQTTBrokerIP))
        return;
    mgr.setKeepAliveClient(600);

    // mqtt.begin(MQTTBrokerIP);
    // mqtt.setKeepAlive(600);

    printf("Setup complete.\n\n");
}

//*** HELPERS ***//
bool isItTimeToParse();
void readAndPublishTemperatures();
void processDataWithInterval();
//*** END HELPERS ***//

unsigned long millisPassedSinceLastParse = 0;
const uint16_t scanInterval = 3000;

void loop()
{
    if (!mgr.connectedToMQTT())
        mgr.connectToMQTT();

    // mgr.checkForIncomingMessages();

    // processDataWithInterval();
}

/////////////////////////////////
//*** HELPERS ***//
/////////////////////////////////
bool isItTimeToParse()
{
    if (abs(millis() - millisPassedSinceLastParse) > scanInterval)
        return true;
    return false;
}

void readAndPublishTemperatures()
{
    uint8_t numberOfSensors = mgr.getSavedNumberSensors();
    for (uint8_t i = 0; i < numberOfSensors; i++)
    {
        float temp = mgr.GetCurrentTemperatureByID(i);
        char tempConverted[10];
        dtostrf(temp, 7, 2, tempConverted);
        char *topic = mgr.GetTopicByID(i);
        char *address = mgr.getStringAddressByID(i);
        mgr.publishMessageToBroker(topic, tempConverted);
        printf("Sensor[%d] = %.2f, Address: %s, topic: %s", temp, address, topic);
    }
    millisPassedSinceLastParse = millis();
}

void processDataWithInterval()
{
    if (!isItTimeToParse())
        return;

    mgr.requestCurrentTemperatures();

    readAndPublishTemperatures();
}
//*** END HELPERS ***//
