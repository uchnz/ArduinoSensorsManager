#pragma once

#include <EthArduino.h>
#include <MQTTArduino.h>
#include <DallasArduino.h>
#include <SASArduino.h>
#include <MQ7COArduino.h>
#include <SHT20Arduino.h>
#include <BMP280Arduino.h>
#include <OnOffSensorArduino.h>
#include <SensorManager.h>

//*** SETUP HELPERS ***//
void initSystemParameters();
void initNetworkCard(EthArduino &eth);
void initConnectionToMQTTBroker(MQTTArduino &mqtt);
void InitDallasSensor(DallasArduino &dallas, const char *name, uint16_t interval = 0);
void InitMQ7COSensor(MQ7COArduino &mq7co, const char *name, uint16_t interval = 0);
void InitOnOffSensor(OnOffSensorArduino &fl, const char *name, uint16_t interval = 0);
// void InitSHT20Sensor(SHT20Arduino &sht20, const char *name, uint16_t interval = 0);
void InitBMP280Sensor(BMP280Arduino &bmp280, const char *name, uint16_t interval = 0);

//*** LOOP HELPERS ***//
bool isItTimeToParse(uint32_t millisPassedSinceLastParse, const uint16_t scanInterval);

//*** Callabacks ***//
void callbackIncommingMQTTMessages(String &topic, String &payload);