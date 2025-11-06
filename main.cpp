#include <iostream>
#include <cstdlib>
#include <ctime>
#include <unordered_set>
#include <vector>
#include <string>
#include <limits>
#include "sensor.h"
#include "measurement.h"
#include "storage.h"
#include "utils.h"


using namespace std;

int meanuChoice(int min, int max) {
    while (true) {
        cout << "\nChoose (" << min << "-" << max << "): ";
        int choice;
        if (cin >> choice && choice >= min && choice <= max) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return choice;
        }

        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input!. Please enter a number between " << min << " and " << max << ".\n";
    }
}

//prompt för text
string readLine(const string& prompt){
    cout << prompt;
    string s;
    getline(cin, s);
    return s;
}

int main()
{
      //slumpmässigt genereras värden
    srand(static_cast<unsigned int>(time(nullptr)));

    //skapa två sensor
    Sensor tempSensor("Temperature 1", "°C", -10.0, 40.0);
    Sensor humiditySensor("Humidity 1", "%", 0.0, 100.0);

    //lagring 
    MeasurementStorage storage;

    //meny loop
    while (true) {
        cout << "\n------ MENU ------\n"
             << "1. Read new measurements from all sensors\n"
             << "2. Display statistics for a sensor\n"
             << "3. Display all measurments\n"
             << "4. Save measurements to CSV\n"
             << "5. Load measurements from CSV\n"
             << "6. Exit\n";

     

    }

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

   

   //skriver ut värden
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
