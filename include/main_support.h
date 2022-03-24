#pragma once

#include <EthArduino.h>
#include <MQTTArduino.h>
#include <DallasArduino.h>
#include <SensorManager.h>

//*** SETUP HELPERS ***//
void initSystemParameters();
void initNetworkCard(sensor_manager::EthArduino &eth);
void initConnectionToMQTTBroker(sensor_manager::MQTTArduino &mqtt);
void InitDallasSensors(sensor_manager::DallasArduino &dallas);

//*** LOOP HELPERS ***//
void processDataWithInterval(sensor_manager::SensorManager &mgr, uint64_t &millisPassedSinceLastParse, const uint16_t scanInterval);
bool isItTimeToParse(uint64_t millisPassedSinceLastParse, const uint16_t scanInterval);
void sendData(sensor_manager::SensorManager &mgr);

//*** Callabacks ***//
void callbackIncommingMQTTMessages(String &topic, String &payload);