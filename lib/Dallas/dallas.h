#ifndef DALLAS_H
#define DALLAS_H


void Dallas_initializeDallasTemperatureSensors();
void Dallas_RequestTemperatures();
int Dallas_GetNumberOfSensors();
double Dallas_GetTemperatureByID(int id);
String Dallas_getAddressByID(int id);

#endif