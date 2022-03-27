#pragma once

#include <EthArduino.h>
#include <MQTTArduino.h>
#include <DallasArduino.h>
#include <SensorManager.h>

//*** SETUP HELPERS ***//
void initSystemParameters();
// void initNetworkCard(sensor_manager::EthArduino &eth);
// void initConnectionToMQTTBroker(sensor_manager::MQTTArduino &mqtt);
// void InitDallasSensors(sensor_manager::DallasArduino &dallas);
void initNetworkCard(EthArduino &eth);
void initConnectionToMQTTBroker(MQTTArduino &mqtt);
void InitDallasSensors(DallasArduino &dallas);

//*** LOOP HELPERS ***//
bool isItTimeToParse(uint64_t millisPassedSinceLastParse, const uint16_t scanInterval);

//*** Callabacks ***//
void callbackIncommingMQTTMessages(String &topic, String &payload);