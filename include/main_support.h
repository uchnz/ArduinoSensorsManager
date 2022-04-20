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
void InitDallasSensor(DallasArduino &dallas);
void InitSASSensor(SASArduino &moisure);
void InitMQ7COSensor(MQ7COArduino &mq7co);
void InitOnOffSensor(OnOffSensorArduino &fl);
void InitSHT20Sensor(SHT20Arduino &sht20);
void InitBMP280Sensor(BMP280Arduino &bmp280);

//*** LOOP HELPERS ***//
bool isItTimeToParse(uint32_t millisPassedSinceLastParse, const uint16_t scanInterval);

//*** Callabacks ***//
void callbackIncommingMQTTMessages(String &topic, String &payload);