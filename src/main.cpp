#include <LibPrintf.h>
#include <Wire.h>

#include <SensorManager.h>
#include <BaseSensor.h>
#include <OnOffSensorArduino.h>
#include <MQ7COArduino.h>
#include <TimerArduino.h>
#include <PhaseTimerArduino.h>
#include <IOArduino.h>
#include <MQ7COIOArduino.h>
#include <SHTIOArduino.h>
#include <BMP280IOArduino.h>
#include <DallasIOArduino.h>
#include <main_support.h>

// #include "I2CScanner.h"
// I2CScanner scanner;

// * more text
//* * some text
//** more text
//* some text
//! remarks
// ! remarks
// ? questions
// TODO some

EthArduino ethernetModule;
MQTTArduino mqttClientModule;

OneWire ow1(22);
DallasTemperature dt1(&ow1);
DallasIOArduino dallas_io(dt1);
BaseSensor dallas1("dallas1", dallas_io);
TimerArduino timer_dallas(300);

OneWire ow2(24);
DallasTemperature dt2(&ow2);
DallasIOArduino dallas_io2(dt2);
BaseSensor dallas2("dallas2", dallas_io2);
TimerArduino timer_dallas2(300);

AnalogIOArduino moisureR_io(A2, INPUT);
BaseSensor moisureR1("moisureR1", moisureR_io);
TimerArduino timer_mr(300);

AnalogIOArduino moisureC_io(A1, INPUT);
BaseSensor moisureC1("moisureC1", moisureC_io);
TimerArduino timer_mc(300);

ThreePhaseTimerArduino sensorTimer;
MQ7COIOArduino mq7_io(A4, 7, sensorTimer);
MQ7COArduino mq7("mq7", mq7_io);
TimerArduino timer_mq7(300);

AnalogIOArduino co2_io(A0, INPUT);
BaseSensor co2("co2", co2_io);
TimerArduino timer_co2(300);

AnalogIOArduino rain_io(A3, INPUT);
BaseSensor rain("rain", rain_io);
TimerArduino timer_rain(300);

AnalogIOArduino uv_io(A5, INPUT);
BaseSensor uv("uv", uv_io);
TimerArduino timer_uv(300);

iarduino_I2C_SHT shtI2C(0x09);
SHTIOArduino sht_io(shtI2C);
BaseSensor sht20("sht", sht_io);
TimerArduino timer_sht(300);

iarduino_Pressure_BMP bmpI2C;
BMP280IOArduino bmp_io(bmpI2C);
BaseSensor bmp280("bmp", bmp_io);
TimerArduino timer_bmp(300);

DigitalIOArduino float_io(38, INPUT_PULLUP);
OnOffSensorArduino float_onoff("float", float_io);
TimerArduino timer_float(300);

const uint8_t totalSensorPorts = 11;
const char *sendToAddresses[totalSensorPorts] = {"/temp/dallas1", "/temp/dallas2", "/moisure/r1", "/moisure/c1", "/mq7", "/co2",
                                                 "/rain", "/uv", "/sht20", "/bmp280", "/float_onoff"};
ISensor *senorsArray[totalSensorPorts] = {&dallas1, &dallas2, &moisureR1, &moisureC1, &mq7, &co2,
                                          &rain, &uv, &sht20, &bmp280, &float_onoff};
// const uint8_t totalSensorPorts = 1;
// const char *sendToAddresses[totalSensorPorts] = {"/float_onoff"};
// ISensor *senorsArray[totalSensorPorts] = {&float_onoff};
SensorManager sensorsManager(mqttClientModule);

void setup()
{
    initSystemParameters();
    Serial.println("before");
    printf("\nStarting setup...\n");

    initNetworkCard(ethernetModule);
    initConnectionToMQTTBroker(mqttClientModule);

    dallas1.init(&timer_dallas);
    dallas2.init(&timer_dallas2);
    moisureR1.init(&timer_mr);
    moisureC1.init(&timer_mc);
    mq7.init(&timer_mq7);
    co2.init(&timer_co2);
    rain.init(&timer_rain);
    uv.init(&timer_uv);
    sht20.init(&timer_sht);
    bmp280.init(&timer_bmp);
    float_onoff.init(&timer_float);

    sensorsManager.init2(senorsArray, sendToAddresses, totalSensorPorts);

    // printf("Listing connected I2C devices:\n");
    // scanner.Init();
    // scanner.Scan();

    printf("Setup complete.\n\n");
}

uint32_t millisPassedSinceLastParse = 0;
const uint16_t scanInterval = 2000;
uint32_t i = 0;
void loop()
{
    sensorsManager.refreshSensors();

    if (!isItTimeToParse(millisPassedSinceLastParse, scanInterval))
        return;

    printf("-------------------------\n");
    printf("seconds: %ld sec, cycles: %ld, interval: %d sec\n", (unsigned long)(millisPassedSinceLastParse / 1000), (unsigned long)(i / scanInterval), (int)scanInterval / 1000);
    i += scanInterval;
    sensorsManager.sendSensorsData();
    sensorsManager.sendSystemInfo(millis());

    millisPassedSinceLastParse = millis();
}
