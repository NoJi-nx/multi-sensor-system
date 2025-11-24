#include "MenuController.h"
#include "utils.h"
#include <iostream>
#include <limits> // numerisk begränsning för  input
#include <thread>
#include <chrono>
#include <iomanip>

using namespace std;

// läser menyval med min,max
// rensar och återhämtar från fel input
int MenuController::menuChoice(int min, int max)
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
string MenuController::readLine(const string &prompt)
{
    cout << prompt;
    string s;
    getline(cin, s);
    return s;
}

//list sensorer och låt användaren använda genom nummer. -- Dominick review
int MenuController::chooseSensorByNumber(const vector<unique_ptr<Sensor>>& sensors) {
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
void MenuController::readAllSensors(const vector<unique_ptr<Sensor>>& sensors, MeasurementStorage& storage, AlarmManager& alarms) {
    if (sensors.empty()) {
        cout << "No sensors to read from.\n";
        return;
    }

    string timeStamp = currentTimeStamp();

    for (const auto& sensor : sensors) {
        double value = sensor->read();
        storage.addReading(sensor->name(), sensor->unit(), value, timeStamp);

        //alarm checkning
        alarms.check(sensor->name(), value, sensor->unit(), timeStamp);
    }
    cout << "OK! Read "  << sensors.size() << " new measurement(s) at "  << timeStamp << ".\n";

    
}

void MenuController::showStatsForChosenSensor(const vector<unique_ptr<Sensor>>& sensors, const MeasurementStorage& storage) {
    if (sensors.empty()) {
        cout << "No sensors available.\n";
        return;
    }

    int idx = chooseSensorByNumber(sensors);
    if (idx < 0) { cout << "Canceled.\n"; return; }
   
    string name = sensors[idx]->name();

    //vanlig statistik
    storage.printStats(name);
    storage.printAll(name);
    
        

    cout << "\nASCII histogram:\n";
    storage.printHistogram(name);   
}

void MenuController::saveCSV(MeasurementStorage& storage) {

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

void MenuController::loadCSV(MeasurementStorage& storage) {
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



void MenuController::searchMeasurements(const vector<unique_ptr<Sensor>>& sensors, const MeasurementStorage& storage) 
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
void MenuController::autoMeasureInterval(const vector<unique_ptr<Sensor>>& sensors, MeasurementStorage& storage, AlarmManager& alarms)
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
         readAllSensors(sensors, storage, alarms);

         if (i < count) {
             cout << "Waiting " << seconds << " second(s)...\n";
             this_thread::sleep_for(chrono::seconds(seconds));
         }
     }

     cout << "\nAutomatic measurement finished.\n";

}

//användaren använder sensorer, sätter begräsning och riktning
void MenuController::configureThreshold(vector<unique_ptr<Sensor>>& sensors, AlarmManager& alarms)
{
    int idx = chooseSensorByNumber(sensors);
    if (idx < 0) { cout << "Canceled.\n"; return; }

    string name = sensors[idx]->name();
    string unit = sensors[idx]->unit();

    cout << "Set threshold for '" << name << "' (" << unit << ")\n";
    cout << "Enter limit: ";

    double limit;
    while (!(cin >> limit)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Wrong number. Try again: ";
    }

    cout << "Alarm direction?\n";
    cout << "1) Alarm if value is ABOVE limit\n";
    cout << "2) Alarm if value is BELOW limit\n";
    int dir = menuChoice(1, 2);

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    alarms.setThreshold(Threshold{name, limit, dir==1});
    cout << "Threshold set: " << name
         << (dir==1 ? " > " : " < ")
         << limit << " " << unit << "\n";
}

//helper för alarm
void MenuController::showAlarms(const AlarmManager& alarms) {
    alarms.printAlarms();
}

void MenuController::runMenu(
    vector<unique_ptr<Sensor>>& sensors, 
        MeasurementStorage& storage,
        AlarmManager& alarms
) {
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
             << "7. Configure threshold rule (alarms)\n"
             << "8. Show alarms\n"
             << "9. Search measurements\n"
             << "10. Exit\n";

        int choice = menuChoice(1, 10);

        if (choice == 1) readAllSensors(sensors, storage, alarms);
        else if (choice == 2) showStatsForChosenSensor(sensors, storage);
        else if (choice == 3) storage.printAll();
        else if (choice == 4) saveCSV(storage);
        else if (choice == 5) loadCSV(storage);
        else if (choice == 6) { autoMeasureInterval(sensors, storage, alarms); }
        else if (choice == 7) { configureThreshold(sensors, alarms); }
        else if (choice == 8) { showAlarms(alarms); }
        else if (choice == 9) { searchMeasurements(sensors, storage); }
        else if (choice == 10) { cout << "Goodbye!\n"; break; }
}
}