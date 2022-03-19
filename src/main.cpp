#include <LibPrintf.h>

#include <EthArduino.h>
#include <MQTTArduino.h>
#include <DallasArduino.h>
#include <SensorManager.h>

String topics[] = {"/UZV1/temp1", "/UZV2/temp1"};
// char *myStrings[] = {"This is string 1", "This is string 2", "This is string 3",
//                      "This is string 4", "This is string 5", "This is string 6"};

unsigned long millisPassed = 0;
uint16_t scanInterval = 3000;

//*** HELPERS ***//
void sendTopicData(sensor_manager::SensorManager *mg, String topic, String data)
{
    printf("Sending teperature data to topic: ");
    Serial.print(topic);
    String sentResult = " -> failed.";
    if (mg->publishMessageToBroker(topic.c_str(), data.c_str()))
        sentResult = " -> sent.";
    Serial.println(sentResult);
}

void printTemperatureDebugInfo(double temperature, int id, sensor_manager::SensorManager *mg)
{
    String addr = mg->getStringAddressByID(id);
    String formatTemperatureString = "Sensor[" + String(id) + "] = " + String(temperature, 2) + ", Address: " + addr;
    Serial.println(formatTemperatureString);
}

void sendTemperaturesToMQTTBroker(sensor_manager::SensorManager *mg)
{
    if (nullptr == mg)
        return;
    uint8_t num = mg->getNumberOfSensors();
    for (uint8_t i = 0; i < num; i++)
    {
        float t = mg->getTemperatureByID(i);
        // String str = mg->printTemperatureDebugInfo(t, i);
        // sendTopicData(mg, topics[i], String(t, 2));
    }
}

void processDataWithInterval(uint16_t interval, sensor_manager::SensorManager *mg)
{
    if (abs(millis() - millisPassed) > interval)
    {
        mg->requestCurrentTemperatures();
        sendTemperaturesToMQTTBroker(mg);
        millisPassed = millis();
    }
}
//*** END HELPERS ***//

sensor_manager::EthArduino eth;
sensor_manager::MQTTArduino mqtt;
sensor_manager::DallasArduino dallas;
sensor_manager::SensorManager mgr(eth, mqtt, dallas);
byte arduinoEthernetMAC[6] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
char MQTTBrokerIP[] = "10.62.202.106";

void setup()
{
    Serial.begin(115200);
    printf_init(Serial); // using printf instead of Serial.print is much more agile
    delay(1000);         // make a pause after begin() to clearly display further messages
    printf("\n");

    dallas.init();
    dallas.setSensorsPrecision(9);
    //    mgr.initDallasSensors();
    //    mgr.setSensorsPrecision(9);

    if (!mgr.connectWithDHCP(arduinoEthernetMAC))
        return;

    if (!mgr.initMQTT(MQTTBrokerIP))
        return;
    mgr.setKeepAliveClient(600);

    printf("Setup complete.\n\n");
}

void loop()
{
    if (!mgr.connectedToMQTT())
        mgr.connectToMQTT();

    mgr.checkForIncomingMessages();

    processDataWithInterval(scanInterval, &mgr);
}
