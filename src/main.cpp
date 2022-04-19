// #include <iarduino_Pressure_BMP.h> // Подключаем библиотеку для работы с датчиками BMP180 или BMP280
// iarduino_Pressure_BMP sensor;      // Создаём объект sensor для работы с датчиком адрес которого на шине I2C установлен по умолчанию.
// void setup()
// {
//     Serial.begin(115200);
//     delay(1000);
//     sensor.begin(); // Инициируем работу с датчиком (начальная высота по умолчанию = 0 метров)
// }
// void loop()
// {
//     // Читаем показания (давление возвращается в мм.рт.ст.)
//     if (sensor.read(1))
//     {
//         Serial.println((String) "CEHCOP BMP" + sensor.type + ": \t P = " + sensor.pressure + "\tMM.PT.CT, \t T = " + sensor.temperature + " *C, \t\t B = " + sensor.altitude + " M.");
//     }
//     else
//     {
//         Serial.println("HET OTBETA OT CEHCOPA");
//     }
//     // Читаем показания (давление возвращается в Па)
//     if (sensor.read(2))
//     {
//         Serial.println((String) "CEHCOP BMP" + sensor.type + ": \t P = " + sensor.pressure + "\tPa, \t\t T = " + sensor.temperature + " *C, \t\t B = " + sensor.altitude + " M.");
//     }
//     else
//     {
//         Serial.println("HET OTBETA OT CEHCOPA");
//     }

//     delay(3000); // Ждём 3 секунды (не обязательно)
// }

// // #include <Wire.h>             // * Подключаем библиотеку для работы с аппаратной шиной I2C.
// #include <iarduino_I2C_SHT.h> //   Подключаем библиотеку для работы с датчиком температуры и влажности I2C-flash (Sensor Humidity and Temperature).
// iarduino_I2C_SHT sht(9);      //   Объявляем объект sht для работы с функциями и методами библиотеки iarduino_I2C_SHT.
//                               // Если при объявлении объекта указать адрес, например, sht(0xBB), то пример будет работать с тем модулем, адрес которого был указан.

// void setup()
// {                         //
//     delay(500);           // * Ждём завершение переходных процессов связанных с подачей питания.
//     Serial.begin(115200); //
//     while (!Serial)
//     {
//         ;
//     } // * Ждём завершения инициализации шины UART.

//     sht.begin(); //   Инициируем работу с датчиком температуры и влажности.
// } //
//   //
// void loop()
// { //
//     Serial.println(sht.getAddress());
//     Serial.print("Температура = ");    //
//     Serial.print(sht.getTem());        //   Выводим текущую температуру, от -40.0 до +125 °C.
//     Serial.print(" °C, влажность = "); //
//     Serial.print(sht.getHum());        //   Выводим текущую влажность воздуха, от 0 до 100%.
//     Serial.print(" %.\r\n");           //
//     delay(500);                        // * Задержка позволяет медленнее заполнять монитор последовательного порта.
// }

#include <LibPrintf.h>
#include <main_support.h>

#include "I2CScanner.h"
#include <Wire.h>

I2CScanner scanner;

EthArduino ethernetModule;
MQTTArduino mqttClientModule;
const uint8_t totalSensorPorts = 6;
const char *addressesToSendTo[totalSensorPorts] = {"/UZV1/temp1", "/UZV2/temp1and2", "/UZV1/mousure", "/UZV1/mq7co", "/UZV1/rd", "/UZV1/floatSensor"};

DallasArduino dallasModule1(32);
DallasArduino dallasModule2(22);
SASArduino moisureR1(A0);
SASArduino moisureC1(A1);
MQ7COArduino mq7co(A2, 7);
SASArduino co2(A3);
SASArduino raindrop(A4);
SASArduino uv(A5);
OnOffSensorArduino floatSensor(38);
iarduino_I2C_SHT shtI2C(0x09);
SHT20Arduino sht20(shtI2C);
iarduino_Pressure_BMP bmpI2C;
BMP280Arduino bmp280(bmpI2C);

// ISensor *d_array[totalSensorPorts] = {&dallasModule1, &dallasModule2, &moisureR1, &mq7co, &raindrop, &floatSensor};

SensorManager sensorsManager(mqttClientModule);

void setup()
{
    initSystemParameters();
    printf("\nStarting setup...\n");

    initNetworkCard(ethernetModule);
    initConnectionToMQTTBroker(mqttClientModule);
    InitDallasSensor(dallasModule1);
    InitDallasSensor(dallasModule2);
    InitMQ7COSensor(mq7co);
    InitSASSensor(moisureR1);
    InitSASSensor(moisureC1);
    InitSASSensor(raindrop);
    InitSASSensor(co2);
    digitalWrite(10, LOW);
    InitSASSensor(uv);
    InitSHT20Sensor(sht20);
    InitBMP280Sensor(bmp280);
    InitOnOffSensor(floatSensor);

    // sensorsManager.initSenorsOnAllPINs(d_array, totalSensorPorts);
    sensorsManager.setAddressesToSendMeasurementsTo(addressesToSendTo, totalSensorPorts);

    printf("Listing connected I2C devices:\n");
    scanner.Init();
    scanner.Scan();

    printf("Setup complete.\n\n");
}

uint32_t millisPassedSinceLastParse = 0;
const uint16_t scanInterval = 2000;
uint32_t i = 0;
void loop()
{
    moisureR1.requestCurrentMeasurement();
    moisureC1.requestCurrentMeasurement();
    mq7co.requestCurrentMeasurement();
    co2.requestCurrentMeasurement();
    raindrop.requestCurrentMeasurement();
    // uv.requestCurrentMeasurement();
    sht20.requestCurrentMeasurement();
    bmp280.requestCurrentMeasurement();
    floatSensor.requestCurrentMeasurement();

    if (!isItTimeToParse(millisPassedSinceLastParse, scanInterval))
        return;

    printf("seconds passed: %u\n", i / 1000);
    i += scanInterval;
    printf("mouisureR: %d\n", (int)moisureR1.getCurrentMeasurementByID());
    // printf("mouisureR analog: %d\n", analogRead(A0));
    printf("mouisureC: %d\n", (int)moisureC1.getCurrentMeasurementByID());
    // printf("mouisureC analog: %d\n", analogRead(A1));
    printf("mq7-co: %d\n", (int)mq7co.getCurrentMeasurementByID());
    // printf("mq7-co analog: %d\n", analogRead(A2));
    printf("mq135-co2: %d\n", (int)co2.getCurrentMeasurementByID());
    // printf("mq135-co2 analog: %d\n", analogRead(A3));
    printf("raindrop: %d\n", (int)raindrop.getCurrentMeasurementByID());
    // printf("raindrop analog: %d\n", analogRead(A4));
    printf("sht20 temp: %.2f\n", sht20.getCurrentMeasurementByID());
    printf("sht20 hum: %.2f\n", sht20.getCurrentMeasurementByID(1));
    printf("bmp280 temp: %.2f\n", bmp280.getCurrentMeasurementByID());
    printf("bmp280 pressure: %.2f\n", bmp280.getCurrentMeasurementByID(1));
    printf("bmp280 altitude: %.2f\n", bmp280.getCurrentMeasurementByID(2));
    printf("float: %d\n", (int)floatSensor.getCurrentMeasurementByID());

    // printf("uv: %d\n", uv.getCurrentMeasurementByID());
    // printf("uv analog: %d\n", analogRead(A5));
    printf("-------------------------\n\n");

    // sensorsManager.refreshSensorsData2D();
    // sensorsManager.sendSensorsData2D();
    millisPassedSinceLastParse = millis();
}
