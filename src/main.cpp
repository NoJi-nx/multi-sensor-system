#include <iostream>
#include <cstdlib> // srand & rand
#include <ctime>   //för RNG, generar slumpmässigt
#include <vector>
#include <string>
#include <limits> // numerisk begränsning för  input
#include "sensor.h"
#include "MeasurementStorage.h"
#include "utils.h"
#include <iomanip>

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
    if (sensors.empty()) {
        cout << "No sensors available.\n";
        return;
    }

    int idx = chooseSensorByNumber(sensors);
    if (idx < 0) { cout << "Canceled.\n"; return; }
    const Sensor& sensor = sensors[idx];
    const string& name = sensors[idx].getName();

    //vanlig statistik
    storage.printStats(name);
    storage.printAll(name);

    //tröskelanalys 
    if (!sensor.hasThreshold()){
        cout << "\n No threshold set for this sensor. Use option 6 to set one. \n";
        return;
    }

    double threshold = sensor.getThreshold();
    size_t above = 0;
    size_t belowOrEqual = 0;

    //loopa över alla mätvärden och beräkna
    const auto& all = storage.data();
    for (const auto& m : all) {
        if (m.sensorName != name)
        continue;

        if (m.value > threshold)
        above++;
        else 
        belowOrEqual++;
    }

    cout << "\nThreshold analysis for '" << name << "'\n";
    cout << "-----------------------------------\n";
    cout << "Threshold: " << threshold << " " << sensor.getUnit() << "\n";
    cout << "Above threshold    : " << above << "\n";
    cout << "Below or equal value : " << belowOrEqual << "\n";

    //markera varje rad som ligger över tröskel

    cout << "\nMeasurements with threshold marker:\n";
    cout << "( '!' means value is above threshold )\n\n";

    cout << left << setw(20) << "Timestamp"
         << setw(18) << "Sensor"
         << right << setw(10) << "Value"
         << setw(8) << "Unit"
         << "  "
         << "Flag"
         << "\n";

    cout << string(20+18+10+8+6, '-') << "\n";

    cout << fixed << setprecision(2);

    for (const auto& m : all) {
        if (m.sensorName != name)
        continue;

        bool isAbove = (m.value > threshold);
        
    cout << left << setw(20) << m.timestamp
         << setw(18) << m.sensorName
         << right << setw(10) << m.value
         << setw(8) << m.unit
         << "  "
         << (isAbove ? "!" : "")
         << "\n";
    }   
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

//låter använderen välja en sensor och sätte ett tröskelvärde
void setThresholdForSensor(vector<Sensor>& sensors) {
    if (sensors.empty()) {
        cout << "No sensors available.\n";
        return;
    }

    int idx = chooseSensorByNumber(sensors);
    if ( idx < 0) {
        cout << "Canceled.\n";
        return;
    }

    cout << "Enter threshold value for sensor '"
         << sensors[idx].getName() << "': ";

    double threshold;
    while (!(cin >> threshold)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid number. Try again: ";

    } cin.ignore(numeric_limits<streamsize>::max(), '\n');

    sensors[idx].setThreshold(threshold);
    cout << "Threshold set to " << threshold
         << " " << sensors[idx].getUnit()
         << " for sensor '" << sensors[idx].getName() << "'.\n";
}

int main()
{
    // slumpmässigt genereras värden
    srand(static_cast<unsigned int>(time(nullptr)));

    //testa timestamp
    cout << "Current time: " << currentTimeStamp <<"\n";

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
             << "6. Set threshold for a sesnor\n"
             << "7. Exit\n";

        int choice = menuChoice(1, 7);

        if (choice == 1) readAllSensors(sensors, storage);
        else if (choice == 2) showStatsForChosenSensor(sensors, storage);
        else if (choice == 3) storage.printAll();
        else if (choice == 4) saveCSV(storage);
        else if (choice == 5) loadCSV(storage);
        else if (choice == 6) { setThresholdForSensor(sensors); }
        else if (choice == 7) { cout << "Goodbye!\n"; break; }
       
    }

    return 0;
}
