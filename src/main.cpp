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
#include <thread> //this_thread::sleep_for
#include <chrono> //chrono::seconds
#include <memory> 
#include "TemperatureSensor.h"
#include "HumiditySensor.h"
#include "PressureSensor.h"

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
int chooseSensorByNumber(const vector<unique_ptr<Sensor>>& sensors) {
    if (sensors.empty()) {
        cout << "No sensors available.\n";
        return -1;
    }

    cout << "Available sensors:\n";
    for (size_t i = 0; i < sensors.size(); ++i) {
        cout << " " << (i + 1) << ") " << sensors[i]->name() << "\n";
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
void readAllSensors(const vector<unique_ptr<Sensor>>& sensors, MeasurementStorage& storage) {
    if (sensors.empty()) {
        cout << "No sensors to read from.\n";
        return;
    }

    string timeStamp = currentTimeStamp();

    for (const auto& sensor : sensors) {
        double value = sensor->read();
        storage.addReading(sensor->name(), sensor->unit(), value, timeStamp);
    }
    cout << "OK! Read "  << sensors.size() << " new measurement(s) at "  << timeStamp << ".\n";

    
}

void showStatsForChosenSensor(const vector<unique_ptr<Sensor>>& sensors, const MeasurementStorage& storage) {
    if (sensors.empty()) {
        cout << "No sensors available.\n";
        return;
    }

    int idx = chooseSensorByNumber(sensors);
    if (idx < 0) { cout << "Canceled.\n"; return; }

    const Sensor& sensor = *sensors[idx];
    string name = sensor.name();

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
    cout << "Threshold: " << threshold << " " << sensor.unit() << "\n";
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

    cout << "\nASCII histogram:\n";
    storage.printHistogram(name);   
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
void setThresholdForSensor(vector<unique_ptr<Sensor>>& sensors) {
    if (sensors.empty()) {
        cout << "No sensors available.\n";
        return;
    }

    int idx = chooseSensorByNumber(sensors);
    if ( idx < 0) {
        cout << "Canceled.\n";
        return;
    }

    string name = sensors[idx]->name();
    string unit = sensors[idx]->unit();

    cout << "Enter threshold value for sensor '"
         << name << "' (" << unit << "): ";

    double threshold;
    while (!(cin >> threshold)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid number. Try again: ";

    } cin.ignore(numeric_limits<streamsize>::max(), '\n');

    sensors[idx]->setThreshold(threshold);
    cout << "Threshold set to " << threshold
         << " " << sensors[idx]->unit()
         << " for sensor '" << sensors[idx]->name() << "'.\n";
}

void searchMeasurements(const vector<unique_ptr<Sensor>>& sensors, const MeasurementStorage& storage) 
{
    if (sensors.empty()) {
        cout << "No sensors available.\n";
        return;
    }

    //välj sensorn först
    int idx = chooseSensorByNumber(sensors);
    if (idx < 0) {
        cout << "Canceled.\n";
        return;
    }

     string sensorName = sensors[idx]->name();

    //efterfrågar användare för tidspunkt
    cout << "Enter FROM timestamp (YYYY-MM-DD HH:MM:SS) or press Enter for no lower limit:\n";
    string fromTs;
    getline(cin, fromTs); //om det finns rester
    if (fromTs.empty()) {
        //om det blir tomt
    }

    cout << "Enter TO timestamp (YYYY-MM-DD HH:MM:SS) or press Enter for no lower limit:\n";
    string toTs;
    getline(cin, toTs);

    cout << "\nSearching measurements for '" << sensorName << "'...\n";
    storage.printSearchResults(sensorName, fromTs, toTs);

}

//automatist mätläge som läser alla sensorer var sekund & totalt gånger
void autoMeasureInterval(const vector<unique_ptr<Sensor>>& sensors, MeasurementStorage& storage)
{
    if (sensors.empty()) {
        cout << "No sensors configured\n";
        return;
    }

    //hur ofta
    cout << "Enter interval in seconds between measurements: ";
    int seconds;
    while (!(cin >> seconds) || seconds <=0) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Please enter a positive number for seconds: ";
    }

    //antal gånger
    cout << "How many measurements do you want to collect? ";
    int count;
       while (!(cin >> count) || count <=0) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Please enter a positive number for count: ";

}

cin.ignore(numeric_limits<streamsize>::max(), '\n'); //rensar radslut

cout << "\nStarting automatic mode: "
     << count << " measurement cycles, every "
     << seconds << "second(s).\n";

     for (int i =1; i <=count; i++){
         cout << "\n--- Cycle " << i <<" / " << count << "---\n";
         //använder helpern
         readAllSensors(sensors, storage);

         if (i < count) {
             cout << "Waiting " << seconds << " second(s)...\n";
             this_thread::sleep_for(chrono::seconds(seconds));
         }
     }

     cout << "\nAutomatic measurement finished.\n";

}

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

    // meny loop -- Del D
    while (true)
    {
        cout << "\n------ MENU ------\n"
             << "1. Read new measurements from all sensors\n"
             << "2. Display statistics for a sensor\n"
             << "3. Display all measurements\n"
             << "4. Save measurements to CSV\n"
             << "5. Load measurements from CSV\n"
             << "6. Automatic measurement in intervals\n"
             << "7. Set threshold for a sesnor\n"
             << "8. Search measurements\n"
             << "9. Exit\n";

        int choice = menuChoice(1, 9);

        if (choice == 1) readAllSensors(sensors, storage);
        else if (choice == 2) showStatsForChosenSensor(sensors, storage);
        else if (choice == 3) storage.printAll();
        else if (choice == 4) saveCSV(storage);
        else if (choice == 5) loadCSV(storage);
        else if (choice == 6) { autoMeasureInterval(sensors, storage); }
        else if (choice == 7) { setThresholdForSensor(sensors); }
        else if (choice == 8) { searchMeasurements(sensors, storage); }
        else if (choice == 9) { cout << "Goodbye!\n"; break; }
       
    }

    return 0;
}
