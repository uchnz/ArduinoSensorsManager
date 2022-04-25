#include <main_support.h>

#include <LibPrintf.h>
#include <Wire.h>

// #include "I2CScanner.h"
// I2CScanner scanner;

EthArduino ethernetModule;
MQTTArduino mqttClientModule;

OneWire ow1(22);
DallasTemperature sensor1(&ow1);
DallasArduino dallas1(sensor1);
OneWire ow2(32);
DallasTemperature sensor2(&ow2);
DallasArduino dallas2(sensor2);
// SASArduino moisureR1(A0);
// SASArduino moisureC1(A1);
// MQ7COArduino mq7co(A2, 7);
// SASArduino co2(A3);
// SASArduino raindrop(A4);
// SASArduino uv(A5);
iarduino_I2C_SHT shtI2C(0x09);
SHT20Arduino sht20(shtI2C);
iarduino_Pressure_BMP bmpI2C;
BMP280Arduino bmp280(bmpI2C);
OnOffSensorArduino floatSensor(38);

// const uint8_t totalSensorPorts = 11;
// const char *sendToAddresses[totalSensorPorts] = {"/temp/dl1", "/temp/dl2", "/moisure/r1", "/moisure/c1",
//                                                  "/gas/co", "/gas/co2", "/water/leak", "/water/level",
//                                                  "/light/uv", "/combo/mois", "/combo/pressure"};
// ISensor *senorsArray[totalSensorPorts] = {&dallas1, &dallas2, &moisureR1, &moisureC1,
//                                           &mq7co, &co2, &raindrop, &floatSensor,
//                                           &uv, &sht20, &bmp280};
const uint8_t totalSensorPorts = 5;
const char *sendToAddresses[totalSensorPorts] = {"/temp/dl1", "/temp/dl2", "/combo/sht20", "/combo/bmp280", "/water/level"};
ISensor *senorsArray[totalSensorPorts] = {&dallas1, &dallas2, &sht20, &bmp280, &floatSensor};

SensorManager sensorsManager(mqttClientModule);

void setup()
{
    initSystemParameters();
    printf("\nStarting setup...\n");

    initNetworkCard(ethernetModule);
    initConnectionToMQTTBroker(mqttClientModule);
    InitDallasSensor(dallas1, "dallas_1");
    InitDallasSensor(dallas2, "dallas_2");
    // InitMQ7COSensor(mq7co);
    // InitSASSensor(moisureR1);
    // InitSASSensor(moisureC1);
    // InitSASSensor(raindrop);
    // InitSASSensor(co2);
    // InitSASSensor(uv);
    InitSHT20Sensor(sht20, "sht");
    InitBMP280Sensor(bmp280, "bmp");
    InitOnOffSensor(floatSensor, "float");

    sensorsManager.initSenorsOnAllPINs(senorsArray, totalSensorPorts);
    sensorsManager.setAddressesToSendMeasurementsTo(sendToAddresses, totalSensorPorts);

    // printf("Listing connected I2C devices:\n");
    // scanner.Init();
    // scanner.Scan();

    printf("Setup complete.\n\n");
}

uint32_t millisPassedSinceLastParse = 0;
const uint16_t scanInterval = 5000;
uint32_t i = 0;
void loop()
{
    dallas1.requestCurrentMeasurement();
    dallas2.requestCurrentMeasurement();
    // moisureR1.requestCurrentMeasurement();
    // moisureC1.requestCurrentMeasurement();
    // mq7co.requestCurrentMeasurement();
    // co2.requestCurrentMeasurement();
    // raindrop.requestCurrentMeasurement();
    // uv.requestCurrentMeasurement();
    sht20.requestCurrentMeasurement();
    bmp280.requestCurrentMeasurement();
    floatSensor.requestCurrentMeasurement();

    sensorsManager.refreshSensorsData2D();

    if (!isItTimeToParse(millisPassedSinceLastParse, scanInterval))
        return;

    printf("seconds passed: %u\n", i / 1000);
    i += scanInterval;
    printf("dallas1 temp1: %.2f\n", dallas1.getCurrentMeasurementByID(0));
    printf("dallas1 temp2: %.2f\n", dallas1.getCurrentMeasurementByID(1));
    printf("dallas2 temp1: %.2f\n", dallas2.getCurrentMeasurementByID());
    printf("dallas2 temp2: %.2f\n", dallas2.getCurrentMeasurementByID(1));
    // printf("mouisureR: %d\n", (int)moisureR1.getCurrentMeasurementByID());
    // printf("mouisureC: %d\n", (int)moisureC1.getCurrentMeasurementByID());
    // printf("mq7-co: %d\n", (int)mq7co.getCurrentMeasurementByID());
    // printf("mq135-co2: %d\n", (int)co2.getCurrentMeasurementByID());
    // printf("raindrop: %d\n", (int)raindrop.getCurrentMeasurementByID());
    printf("sht20 temp: %.2f\n", sht20.getCurrentMeasurementByID());
    printf("sht20 hum: %.2f\n", sht20.getCurrentMeasurementByID(1));
    printf("bmp280 temp: %.2f\n", bmp280.getCurrentMeasurementByID());
    printf("bmp280 pressure: %.2f\n", bmp280.getCurrentMeasurementByID(1));
    printf("bmp280 altitude: %.2f\n", bmp280.getCurrentMeasurementByID(2));
    // printf("uv: %d\n", uv.getCurrentMeasurementByID());
    printf("float: %d\n", (int)floatSensor.getCurrentMeasurementByID());
    printf("-------------------------\n\n");

    sensorsManager.sendSensorsData2D();
    millisPassedSinceLastParse = millis();
}
