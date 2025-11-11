#include <iostream>
#include <cstdlib> // srand & rand
#include <ctime>   //för RNG, generar slumpmässigt
#include <vector>
#include <string>
#include <limits> // numerisk begränsning för  input
#include "sensor.h"
#include "MeasurementStorage.h"
#include "utils.h"

using namespace std;

// läser menyval med min,max
// rensar och återhämtar från fel input
int menuChoice(int min, int max)
{
    while (true)
    {
        cout << "\nChoose (" << min << "-" << max << "): ";
        int choice;
        if (cin >> choice && choice >= min && choice <= max)
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return choice;
        }
        // DOMINIK// add an else just for easier code reading
        else
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input!. Please enter a number between " << min << " and " << max << ".\n";
        }
    }
}

// prompt för text (filnamn, sensor namn etc.)
string readLine(const string &prompt)
{
    cout << prompt;
    string s;
    getline(cin, s);
    return s;
}

//list sensorer och låt användaren använda genom nummer. -- Dominick review
int chooseSensorByNumber(const vector<Sensor>& sensors) {
    if (sensors.empty()) {
        cout << "No sensors available.\n";
        return -1;
    }

    cout << "Available sensors:\n";
    for (size_t i = 0; i < sensors.size(); ++i) {
        cout << " " << (i + 1) << ") " << sensors[i].getName() << "\n";
    }
    cout << " 0) Cancel\n";

    while (true) {
        cout << "Pick a sensor by number (0-" << sensors.size() << "):" ;
        int n;
        if (cin >> n) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (n == 0) return -1;
            if (n >= 1 && static_cast<size_t>(n) <=sensors.size()) {
                return n - 1;
            }
        }else {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cout << "Invalid choice. Try again.\n";    
        
    }
}

//helpers (förslag av Dominik)
void readAllSensors(const vector <Sensor>& sensors, MeasurementStorage& storage) {
    if (sensors.empty()) {
        cout << "No sensors to read from.\n";
        return;
    }
    string timeStamp = currentTimeStamp();
    for (const auto& sensor : sensors) {
        double value = sensor.read();
        storage.addReading(sensor.getName(), sensor.getUnit(), value, timeStamp);
    }
    cout << "OK Read "  << sensors.size() << " new measurement(s) at "  << timeStamp << ".\n";
}

void showStatsForChosenSensor(const vector<Sensor> & sensors, const MeasurementStorage& storage) {
    int idx = chooseSensorByNumber(sensors);
    if (idx < 0) { cout << "Canceled.\n"; return; }
    const string& name = sensors[idx].getName();
    storage.printStats(name);
    storage.printAll(name);
}

void saveCSV(MeasurementStorage& storage) {

            // sparar nuvarande mätvärde till CSV
            string fname = readLine("Filename to save (measurements.csv): ");
            if (fname.empty())
            {
                cout << "Canceled!\n";
                return;
            }
            if (storage.saveToCSV(fname))
                cout << "Saved. " << storage.size() << " rows to " << fname << "\n";
}

void loadCSV(MeasurementStorage& storage) {
    // laddar mätvärde från CSV till listan/lager
            string fname = readLine("Filename to load (measurements.csv): ");
            if (fname.empty())
            {
                cout << "Canceled!\n";
                return;
            }
           if (storage.loadToCSV(fname))
            cout << "Loaded File: " << fname << "\n"; // skriver ut meddelandet
}

int main()
{
    // slumpmässigt genereras värden
    srand(static_cast<unsigned int>(time(nullptr)));

    // konfigurera sesnorerna -- Del A
    // definiera sensorerna
    vector<Sensor> sensors;
    sensors.emplace_back(SensorType::Temperature, "Temperature 1", "°C", -10.0, 40.0);
    sensors.emplace_back(SensorType::Humidity, "Humidity 1", "%", 0.0, 100.0);

    // central lagring för alla mätvärden -- Del B, C & D
    MeasurementStorage storage;

    // meny loop -- Del D
    while (true)
    {
        cout << "\n------ MENU ------\n"
             << "1. Read new measurements from all sensors\n"
             << "2. Display statistics for a sensor\n"
             << "3. Display all measurements\n"
             << "4. Save measurements to CSV\n"
             << "5. Load measurements from CSV\n"
             << "6. Exit\n";

        int choice = menuChoice(1, 6);

        if (choice == 1) readAllSensors(sensors, storage);
        else if (choice == 2) showStatsForChosenSensor(sensors, storage);
        else if (choice == 3) storage.printAll();
        else if (choice == 4) saveCSV(storage);
        else if (choice == 5) loadCSV(storage);
        else if (choice == 6) { cout << "Goodbye!\n"; break; }
       
    }

    return 0;
}
