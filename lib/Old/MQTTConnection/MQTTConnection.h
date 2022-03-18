#ifndef MQTTCONNECTION_H
#define MQTTCONNECTION_H

#include <MQTT.h>
#include <EthernetENC.h>

void MQTT_initializeMQTTClientCallback();
void  MQTT_connect();
bool MQTT_connected();
bool MQTT_checkForNewMessages();
bool MQTT_publishData(String topic, String data);

#endif