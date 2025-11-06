#include <iostream>
#include "sensor.h"

using namespace std;

int main()
{
      //slumpmässigt genereras värden
    srand(static_cast<unsigned int>(time(nullptr)));

    //skapa två sensor
    Sensor tempSensor("TempSensor 1", "°C", -10.0, 40.0);
    Sensor humiditySensor("HumiditySensor 1", "%", 0.0, 100.0);


    //simulerar & hämtar värden
    double tempValue = tempSensor.read();
    double humidityValue = humiditySensor.read();

    //visar värden
    cout << tempSensor.getName() <<": " << tempValue << " °C" << endl;
    cout << humiditySensor.getName() << ": " << humidityValue << " %\n" << endl;

    return 0;
}
