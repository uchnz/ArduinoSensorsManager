#pragma once

#include <EthArduino.h>
#include <MQTTArduino.h>

//*** SETUP HELPERS ***//
void initSystemParameters();
void initNetworkCard(EthArduino &eth);
void initConnectionToMQTTBroker(MQTTArduino &mqtt);

//*** LOOP HELPERS ***//
bool isItTimeToParse(uint32_t millisPassedSinceLastParse, const uint16_t scanInterval);

//*** Callabacks ***//
void callbackIncommingMQTTMessages(String &topic, String &payload);