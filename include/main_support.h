#pragma once

#include <EthArduino.h>
#include <MQTTArduino.h>
#include <DallasArduino.h>
#include <MoisureRArduino.h>
#include <MQ7COArduino.h>
#include <RaindropsArduino.h>
#include <FloatArduino.h>
#include <SensorManager.h>

//*** SETUP HELPERS ***//
void initSystemParameters();
void initNetworkCard(EthArduino &eth);
void initConnectionToMQTTBroker(MQTTArduino &mqtt);
void InitDallasSensor(DallasArduino &dallas);
void InitMoisureRSensor(MoisureRArduino &moisure);
void InitMQ7COSensor(MQ7COArduino &mq7co);
void initRaindropsSensor(RaindropsArduino &rd);
void initFloatArduinoSensor(FloatArduino &fl);

//*** LOOP HELPERS ***//
bool isItTimeToParse(uint64_t millisPassedSinceLastParse, const uint16_t scanInterval);

//*** Callabacks ***//
void callbackIncommingMQTTMessages(String &topic, String &payload);