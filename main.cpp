#include <iostream>
#include <cstdlib>
#include <ctime>
#include "sensor.h"
#include "measurement.h"
#include "storage.h"
#include "utils.h"


using namespace std;

int main()
{
      //slumpmässigt genereras värden
    srand(static_cast<unsigned int>(time(nullptr)));

    //skapa två sensor
    Sensor tempSensor("Temperature 1", "°C", -10.0, 40.0);
    Sensor humiditySensor("Humidity 1", "%", 0.0, 100.0);

    //Lagring 
    MeasurementStorage storage;

    //Fel sensor
    //Sensor brokenSensor("FaultySensor", "°C", 50.0, -10.0); //Ta bort för testa

//simulerar & hämtar värden
    double tempValue = tempSensor.read();
    double humidityValue = humiditySensor.read();

    //skapar matvärde för lagring
    Measurement m1 { tempSensor.getName(), tempSensor.getUnit(), tempValue, currentTimeStamp() };
    Measurement m2 { humiditySensor.getName(), humiditySensor.getUnit(), humidityValue, currentTimeStamp() };

  storage.addMeasurement(m1);
   storage.addMeasurement(m2);

   

   //demo
   storage.printAll();

   cout << "\n";
   storage.printStats("Temperature 1");
   cout << "\n";
   storage.printStats("Humidity 1");

    /*//visar värden
    cout << tempSensor.getName() <<":°C" << tempValue << " °C" << endl;
    cout << humiditySensor.getName() << ": " << humidityValue << " %\n" << endl;*/

    return 0;
}
