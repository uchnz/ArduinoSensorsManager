// #include <DallasTemperature.h>
// #include <dallas.h>

// #define ONE_WIRE_BUS 22
// #define TEMPERATURE_PRECISION 9
// OneWire oneWire(ONE_WIRE_BUS);
// DallasTemperature sensors(&oneWire);

// String getAddressString(DeviceAddress deviceAddress)
// {
//   // String addr = "";
//   // for (int i = 0; i < 8; i++)
//   // {
//   //   addr += String(deviceAddress[i], HEX);
//   // }
//   // return addr;
// }

// String Dallas_getAddressByID(int id)
// {
//   // DeviceAddress devAddress;
//   // sensors.getAddress(devAddress, id);
//   // String str = getAddressString(devAddress);
//   // return str;
// }

// int Dallas_GetNumberOfSensors()
// {
//   // return sensors.getDeviceCount();
// }

// double Dallas_GetTemperatureByID(int id)
// {
//   // double t = sensors.getTempCByIndex(id);
//   // if (DEVICE_DISCONNECTED_C == t)
//   //   Serial.println("[Error: Could not read temperature data]");
//   // return t;
// }

// void Dallas_RequestTemperatures()
// {
//   // Serial.print("Requesting temperatures...");
//   // sensors.requestTemperatures();
//   // Serial.println("DONE");
// }

// void Dallas_showSensorsAddresses()
// {
//   // oneWire.reset_search();
//   // Serial.println("Listing sensors' addresses:");
//   // String messageWithAddressFormatted = "";
//   // int num = sensors.getDeviceCount();
//   // DeviceAddress devAddress;
//   // for (int i = 0; i < num; i++)
//   // {
//   //   String addressFormated = "Unable to find an address of the sensor.";
//   //   if (sensors.getAddress(devAddress, i))
//   //     addressFormated = getAddressString(devAddress);
//   //   messageWithAddressFormatted = "Sensor[" + String(i) + "] Address: " + addressFormated;
//   //   Serial.println(messageWithAddressFormatted);
//   // }
// }

// void Dallas_showNumberOfFoundSensors()
// {
//   //   Serial.print("Locating Dallas Sensors..."); Serial.print(" Found ");
//   //   int num = sensors.getDeviceCount();
//   //   Serial.print(num, DEC); Serial.println(" devices.");
// }

// void Dallas_showParasitePowerMode()
// {
//   // Serial.print("Parasite power mode: ");
//   // if (sensors.isParasitePowerMode())
//   //   Serial.println("ON");
//   // else
//   //   Serial.println("OFF");
// }

// void Dallas_setPrecision()
// {
//   // oneWire.reset_search();
//   // Serial.print("Set sensors' resolution to: ");
//   // Serial.println(TEMPERATURE_PRECISION);
//   // int num = sensors.getDeviceCount();
//   // DeviceAddress devAddress;
//   // for (int i = 0; i < num; i++)
//   // {
//   //   if (sensors.getAddress(devAddress, i))
//   //     sensors.setResolution(devAddress, TEMPERATURE_PRECISION);
//   // }
// }

// void Dallas_initializeDallasTemperatureSensors()
// {
//   // sensors.begin();
//   // Dallas_showNumberOfFoundSensors();
//   // Dallas_showParasitePowerMode();
//   // Dallas_showSensorsAddresses();
//   // Dallas_setPrecision();
// }
