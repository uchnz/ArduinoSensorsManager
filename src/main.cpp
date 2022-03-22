#include <LibPrintf.h>

#include <EthArduino.h>
#include <MQTTArduino.h>
#include <DallasArduino.h>
#include <SensorManager.h>

//#include <EthernetENC.h>

sensor_manager::EthArduino eth;
sensor_manager::MQTTArduino mqtt;
sensor_manager::DallasArduino dallas;
sensor_manager::SensorManager mgr(eth, mqtt, dallas);
byte arduinoEthernetMAC[6] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
char MQTTBrokerIP[] = "10.62.202.108";

const char *topics[] = {"/UZV1/temp1", "/UZV2/temp1"};

//////////////////////////////////////
//*** SETUP HELPERS DECLARATIONS ***//
void initSystemParameters();
//*** END DECLARATIONS ***//
void setup()
{
    initSystemParameters();
    printf("\nStarting setup...\n");

    dallas.init();
    dallas.setSensorsPrecision(9);
    mgr.initSensors();

    mgr.fillTopicsStrings(topics, 2);

    if (!eth.connect(arduinoEthernetMAC))
        return;

    // if (!mgr.connectWithDHCP(arduinoEthernetMAC))
    //     return;

    mqtt.begin(MQTTBrokerIP);
    mqtt.setKeepAlive(600);

    printf("Setup complete.\n\n");
}
//*** SETUP HELPERS ***//
void initSystemParameters()
{
    Serial.begin(115200);
    printf_init(Serial); // using printf instead of Serial.print is much more agile
    while (!Serial)      // make a pause to initialize Serial for further output
        delay(1000);
}
//*** END SETUP HELPERS ***//
/////////////////////////////
/////////////////////////////

/////////////////////////////////////
//*** LOOP HELPERS DECLARATIONS ***//
void processDataWithInterval();
bool isItTimeToParse();
void readAndAct();
//*** END DECLARATIONS ***//

unsigned long millisPassedSinceLastParse = 0;
const uint16_t scanInterval = 3000;

void loop()
{
    if (!mgr.connectedToMQTT())
        mgr.connectToMQTT();

    mgr.checkForIncomingMessages();

    processDataWithInterval();
}

//*** LOOP HELPERS ***//
void processDataWithInterval()
{
    if (!isItTimeToParse())
        return;

    mgr.requestCurrentTemperatures();

    readAndAct();
}

bool isItTimeToParse()
{
    if (abs(millis() - millisPassedSinceLastParse) > scanInterval)
        return true;
    return false;
}

void readAndAct()
{
    uint8_t numberOfSensors = mgr.getSavedNumberSensors();
    for (uint8_t i = 0; i < numberOfSensors; i++)
    {
        float temp = mgr.GetCurrentTemperatureByID(i);
        char tempConverted[10];
        dtostrf(temp, 7, 2, tempConverted);
        char topic[100];
        mgr.GetTopicByID(i, topic);
        mgr.publishMessageToBroker(topic, tempConverted);

        char address[15];
        mgr.getStringAddressByID(i, address);
        printf("Sensor[%d] = %.2f, Address: %s, topic: %s\n", i, temp, address, topic);
    }
    millisPassedSinceLastParse = millis();
}
//*** END HELPERS ***//
