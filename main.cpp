#include <iostream>
#include <cstdlib> // srand & rand
#include <ctime>     //för RNG, generar slumpmässigt
#include <unordered_set>
#include <vector>
#include <string>
#include <limits> // numerisk begränsning för  input
#include "sensor.h"
#include "measurement.h"
#include "storage.h"
#include "utils.h" 


using namespace std;

//läser menyval med min,max
//rensar och återhämtar från fel input
int menuChoice(int min, int max) {
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

//prompt för text (filnamn, sensor namn etc.)
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

    //konfigurera sesnorerna -- Del A
    //definiera sensorerna
    vector<Sensor> sensors;
    sensors.emplace_back("Temperature", "°C", -10.0, 40.0);
    sensors.emplace_back("Humidity", "%", 0.0, 100.0);

    //central lagring för alla mätvärden -- Del B, C & D
    MeasurementStorage storage;

    //meny loop -- Del D
    while (true) {
        cout << "\n------ MENU ------\n"
             << "1. Read new measurements from all sensors\n"
             << "2. Display statistics for a sensor\n"
             << "3. Display all measurements\n"
             << "4. Save measurements to CSV\n"
             << "5. Load measurements from CSV\n"
             << "6. Exit\n";

      int choice = menuChoice(1, 6);

      if (choice == 1) {
          //läser nya mätvärde från varje sensor och läggs till lagring
          string ts = currentTimeStamp();
        for (auto& s : sensors) {
            double val = s.read();
            storage.addReading(s.getName(), s.getUnit(), val, ts);
        }
         cout << "OK Read " << sensors.size() << " new measurement(s) at " << ts << ".\n";
      }
      else if (choice == 2) {
          //visar statistik för en utvald sensor med exakt namn
          cout << "Available sensors:\n";
          for (const auto& s : sensors) cout << " - " << s.getName() << "\n";

          string name = readLine("Enter the exact sensor name: ");
          storage.printStats(name);

      }
      else  if (choice == 3) {
          //skriver ut  mätvärde i en tabell
          storage.printAll();
      }
      else if (choice == 4) {
          //sparar nuvarande mätvärde till CSV
          string fname = readLine("Filename to save (measurements.csv): ");
          if (fname.empty()) { cout << "Canceled!\n"; continue; }
          if (storage.saveToCSV(fname)) cout << "Saved. " << storage.size() << " rows to " << fname << "\n";
      }
      else if (choice == 5) {
          //laddar mätvärde från CSV till listan/lager
          string fname = readLine("Filename to load (measurements.csv): ");
          if (fname.empty()) { cout << "Canceled!\n"; continue; }
          storage.loadFromCSV(fname); // skriver ut meddelandet

      }

      else if (choice == 6) {
          cout << "Goodbye!\n";
          break;
      }
    }

    return 0;
}
