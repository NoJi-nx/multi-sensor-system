#include <iostream>
#include <ctime>   //för RNG, generar slumpmässigt
#include <vector>
#include <string>
#include "sensor.h"
#include "MeasurementStorage.h"
#include "utils.h"
#include <memory> 
#include "TemperatureSensor.h"
#include "HumiditySensor.h"
#include "PressureSensor.h"
#include "AlarmManager.h"
#include "MenuController.h"

using namespace std;

int main()
{
    // slumpmässigt genereras värden
    srand(static_cast<unsigned int>(time(nullptr)));

    //testa timestamp
    cout << "Current time: " << currentTimeStamp() <<"\n";

    // konfigurera & definiera sesnorerna -- Del A
    // lagra genom polymorfism
    vector<unique_ptr<Sensor>> sensors;
    sensors.push_back(make_unique<TemperatureSensor>("Temperature 1", -10.0, 40.0));
    sensors.push_back(make_unique<HumiditySensor>("Humidity 1", 0.0, 100.0));
    sensors.push_back(make_unique<PressureSensor>("Pressure 1", 900.0, 1100.0));

    // central lagring för alla mätvärden -- Del B, C & D
    MeasurementStorage storage;
    AlarmManager alarmManager;

    MenuController::runMenu(sensors, storage, alarmManager);

    return 0;
}
