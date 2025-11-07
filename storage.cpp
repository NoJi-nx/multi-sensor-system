#include <iostream>
#include "storage.h"
#include <iomanip>
#include <limits>
#include <cmath> 
#include <algorithm>
#include <string>
#include <sstream>
#include <fstream>
#include <cctype> //kika områden när man trimmar text

using namespace std;

//lägger en ny mätävrde object till listan
void MeasurementStorage::addMeasurement(const Measurement& m) 
{
    measurements.push_back(m); //samlar värden 

}

//beräknar statistik (medelmvärde, min, max etc) för en specific sensor
MeasurementStorage::Stats
MeasurementStorage::computeStats(const string& sensorName) const {
    Stats s; // samlar resultaten på statistiken


    //filtrerar värden 
    double minv = numeric_limits<double>::infinity();
    double maxv = -numeric_limits<double>::infinity();

    //Welford's online algoritm för  medel/varians (https://en.wikipedia.org/wiki/Algorithms_for_calculating_variance#Welford's_online_algorithm)
    //variabel för Welford till beräkna medelvärde & standardavvvikelse
    size_t n = 0;     // antal readings
    double mean = 0.0; // medelvärdet
    double M2 = 0.0;   // summan av kvadratsskillnad

    //hanterar varje lagrad mätvärde
    for (const auto& m : measurements) {
        if (m.sensorName != sensorName) continue;

        //sparar enheten på sensor
        if (!s.hasData) {
            s.unit = m.unit; //antal enhet per sensor
            s.hasData = true;
        }

        //uppdaterar min/max
        if (m.value < minv) minv = m.value;
        if (m.value > maxv) maxv = m.value;

        //uppdaterar Welford algoritm för emdelvärde och standardavvikelse
        ++n;
        double delta = m.value - mean;
        mean += delta / static_cast<double>(n);
        double delta2 = m.value - mean;
        M2 += delta * delta2;

    }
        // om ingen mätvärde hittats
        if (!s.hasData) { 
            return s; //hämtar tom resultat

        }

        //sparar räkningen statistik
        s.count = n;
        s.mean = mean;
        s.min = minv;
        s.max = maxv;

        // beräknar standardavvikelse
        if (n >= 2) {
            double variance_sample = M2 / static_cast<double>(n - 1); // varians (n-1)
            s.stddev = sqrt(variance_sample);
        } else {
            s.stddev = 0.0; // om det finns inte tillräckligt värde för standardavvikelse
        }

        return s;

    }


    //skriver ut räkningar på statistiken för en specifikt av sensor
    void MeasurementStorage::printStats(const string& sensorName) const {
        auto s = computeStats(sensorName); // räknar först

        //om ej data finns
        if (!s.hasData) {
            cout << "No measurements: " << sensorName << "\n";
            return;
        }

        //skriver resultatet i format
        cout << "Statistics for sensor: " << sensorName << "\n";
        cout << string(26 + sensorName.size(), '-') << "\n";

        cout << fixed << setprecision(2);
        cout << "Count     : " << s.count << "\n";
        cout << "Average   : " << s.mean  << " " << s.unit << "\n";
        cout << "Min       : " << s.min   << " " << s.unit << "\n";
        cout << "Max       : " << s.max   << " " << s.unit << "\n";
        cout << "Std Dev   : " << s.stddev << " " << s.unit << "\n";
    }




//skriver alla lagrad mätvärde i en tabell format
void MeasurementStorage::printAll() const
{
    if (measurements.empty()) {
        cout << "[No measurements stored]\n";
        return;
    }

    //column punk för varje column
    const int tsW = 20;
    const int sensorW =18;
    const int valW = 10;
    const int unitW = 8;

    //visar headern/titeln
    cout << left
    << setw(tsW) << "Timestamp"
        << setw(sensorW) << "Sensor"
        << right
        << setw(valW) << "Value"
        << setw(unitW) << "Unit"
        << "\n";

     //visar seperat linje   
    cout << string(tsW + sensorW + valW + unitW, '-') << "\n";
    cout << fixed << setprecision(2);

    //visar varje rad inom mätvärdena genom loop
    for (const auto& m : measurements) {
        cout << left
        << setw(tsW) << m.timestamp
        << setw(sensorW) << m.sensorName
        << right
        << setw(valW) << m.value
        << setw(unitW) << m.unit
        << "\n";
    }
}


//skrive ut endast de mätävrden som tillhör till en specifikt sensor
void MeasurementStorage::printAll(const string& sensorName) const {
    bool any = false; //kikar om det har hittat något


    //skriver ut titeln
      const int tsW = 20;
   int sensorW = max<int>(18, static_cast<int>(sensorName.size()));
    const int valW = 10;
    const int unitW = 8;

    cout << left
         << setw(tsW)     << "Timestamp"
         << setw(sensorW) << "Sensor"
         << right 
         << setw(valW)    << "Value"
         << setw(unitW)   << "Unit"
         << "\n";
         

         cout << string(tsW + sensorW + valW + unitW, '-') << "\n";
         cout << fixed << setprecision(2);

         //skriver ut endast rader som tillhör specifikt sensor
         for (const auto& m : measurements) {
             if (m.sensorName != sensorName) continue;
             any = true;
             cout << left
            << setw(tsW) << m.timestamp
            << setw(sensorW) << m.sensorName
            << right
            << setw(valW) << m.value
            << setw(unitW) << m.unit
            << "\n";
    }
    //visar meddelandet om ingen data finns
    if (!any) {
        cout << "[No measurements stored for sensor: " << sensorName << "]\n";
    }
}

//tar bort utrymme från början och slutet av texten ()
static inline string trim(string s){
    auto isspace_ = [](unsigned char c) {return isspace(c); };

    //tar bort från start
    s.erase(s.begin(), find_if(s.begin(), s.end(), [&](unsigned char c){return !isspace_(c);}));

    //tar bort från slutet
    s.erase(find_if(s.rbegin(), s.rend(), [&](unsigned char c){ return !isspace_(c);}).base(), s.end());
    return s;
}

//sparar till CSV fil förhandgranskar
bool MeasurementStorage::saveToCSV(const string& filename) const {
    ofstream out(filename);
    if (!out) {
        cerr << "Error. Could not open file for writing: " << filename << "\n";
        return false;
    }
    //skriver i varje mätvärde i en linje
    for (const auto& m: measurements) {
        out << m.timestamp << ", "
            << m.sensorName << ", "
            << m.value << ", "
            << m.unit << "\n";
    }

    return true;

}

//laddar från en CSV file och lägger de ttill nuvarande lista
bool MeasurementStorage::loadFromCSV(const string& filename) {
    ifstream in(filename);
    if (!in) {
        cerr << "Error! Could not open fil for reading: " << filename << "\n";
        return false;
    }

    string line;
    size_t added = 0, skipped = 0;

    //läser filen linje från linje
    while (getline(in, line)) {
        if (line.empty()) {skipped++; continue;}

        istringstream ss(line);
        string ts, name, valueStr, unit;


        //delar varje linje till 4 delar genom komma tecken
        if (!getline(ss, ts, ',')) {skipped++; continue;}
        if (!getline(ss, name, ',')) {skipped++; continue;}
        if (!getline(ss, valueStr, ',')) {skipped++; continue;}
        if (!getline(ss, unit)) {skipped++; continue;}

        //rensar extra utrymme
        ts = trim(ts);
        name = trim(name);
        valueStr= trim(valueStr);
        unit = trim(unit);

        //konvertera värde från text till nummer
        double v = 0.0;
        try {
            v = stod(valueStr);
        } catch (...) {
            skipped++;
            continue; //ignorera nummerisk fält

        }

        //lägger till i listan/lager
        measurements.push_back(Measurement{name, unit, v, ts});
        added++;
    }

    //visar hur många linjer var i process
    if (skipped > 0) {
        cerr << "Info Loaded " << added << " rows; skipped" << skipped << " malformed rows.\n";
    }
    return true;
}