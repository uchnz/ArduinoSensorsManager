#include <MQTTConnection.h>

MQTTClient clientMQTT;
EthernetClient net;

void MQTT_subscribeToTopic(String topic)
{
  Serial.println("Trying to subscribe...");
  String result = "Subscribed to topic: ";
  if ( clientMQTT.subscribe(topic) )
    result += topic;
  else
    result = "Can't subscribe to topic:" + topic;
  Serial.println(result);
}


bool MQTT_publishData(String topic, String data)
{
  return clientMQTT.publish(topic, data);
}

bool MQTT_checkForNewMessages()
{
  return  clientMQTT.loop();
}

bool MQTT_connected()
{
  bool status = clientMQTT.connected();
  Serial.print("Connection status: ");
  Serial.println(status);
  return status;
}

void MQTT_connect()
{
  if (clientMQTT.connected())
    return;
  Serial.print("Connecting to MQTT broker...");
  while (!clientMQTT.connect("arduinoID", "admin", "admin")) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("connected.");
}

void messageReceived(String &topic, String &payload)
{
  Serial.println("not subscribed to any topic");
}

void MQTT_initializeMQTTClientCallback()
{
  String serverIP = "10.62.202.106";
  IPAddress ip;
  ip.fromString(serverIP);
  Serial.print("Initializing client with MQTT Server IP: ");
  Serial.println(serverIP);
  clientMQTT.begin(ip, net);
  Serial.println("Increase keep alive to 600 sec.");
  clientMQTT.setKeepAlive(60);
  Serial.print("Installing a callback function for reading MQTT messages... ");
  clientMQTT.onMessage(messageReceived);
  Serial.println("a callback function Installed.");
}

