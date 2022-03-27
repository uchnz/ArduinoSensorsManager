#include <EthArduino.h>
#include <EthernetENC.h>

// namespace sensor_manager
// {
bool EthArduino::connect(uint8_t (&mac)[6])
{
    Serial.print(F("Detecting DHCP Server... "));
    if (0 == Ethernet.begin(mac))
    {
        Serial.println(F("ERROR: Unable to configure Ethernet using DHCP!!"));
        return false;
    }
    Serial.println(F(" ethernet configured via DHCP."));
    Serial.print("IP: ");
    Serial.println(Ethernet.localIP());
    Serial.print("MASK: ");
    Serial.println(Ethernet.subnetMask());
    Serial.print("GW: ");
    Serial.println(Ethernet.gatewayIP());
    Serial.print("DNS: ");
    Serial.println(Ethernet.dnsServerIP());
    return true;
}
// }

// bool SensorManager::isEmptyMAC(uint8_t (&mac)[6])
// {
//     bool emptyMAC = true;
//     for (uint8_t i = 0; i < sizeof(mac); i++)
//     {
//         if (0 != mac[i])
//             emptyMAC = false;
//     }
//     return emptyMAC;
// }

// IEth
// bool SensorManager::connectWithDHCP(uint8_t (&mac)[6])
// {
//     if (isEmptyMAC(mac))
//         return false;

//     return _eth.connect(mac);
// }
