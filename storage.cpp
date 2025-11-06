#include <iostream>
#include "storage.h"
#include <iomanip>
#include <limits>
#include <cmath> 
#include <algorithm>
#include <string>
#include <sstream>
#include <fstream>

using namespace std;

//lägger nya matvärden
void MeasurementStorage::addMeasurement(const Measurement& m) 
{
    measurements.push_back(m);

}

MeasurementStorage::Stats
MeasurementStorage::computeStats(const string& sensorName) const {
    Stats s;


    //filtrerar värden 
    double minv = numeric_limits<double>::infinity();
    double maxv = -numeric_limits<double>::infinity();

    //Welford's online algoritm för  medel/varians
    //https://en.wikipedia.org/wiki/Algorithms_for_calculating_variance#Welford's_online_algorithm
    size_t n = 0;
    double mean = 0.0;
    double M2 = 0.0;

    for (const auto& m : measurements) {
        if (m.sensorName != sensorName) continue;

        if (!s.hasData) {
            s.unit = m.unit; //antal enhet per sensor
            s.hasData = true;
        }

        //uppdaterar min/max
        if (m.value < minv) minv = m.value;
        if (m.value > maxv) maxv = m.value;

        //uppdaterar Welford
        ++n;
        double delta = m.value - mean;
        mean += delta / static_cast<double>(n);
        double delta2 = m.value - mean;
        M2 += delta * delta2;

    }

        if (!s.hasData) { //ingen mätvärde för sensor
            return s; // hasdata = false

        }

        s.count = n;
        s.mean = mean;
        s.min = minv;
        s.max = maxv;

        if (n >= 2) {
            double variance_sample = M2 / static_cast<double>(n - 1); // varians (n-1)
            s.stddev = sqrt(variance_sample);
        } else {
            s.stddev = 0.0; // definiera som 0.0 vid ett värde
        }

        return s;

    }

    void MeasurementStorage::printStats(const string& sensorName) const {
        auto s = computeStats(sensorName);

        if (!s.hasData) {
            cout << "No measurements: " << sensorName << "\n";
            return;
        }

        cout << "Statistics for sensor: " << sensorName << "\n";
        cout << string(26 + sensorName.size(), '-') << "\n";

        cout << fixed << setprecision(2);
        cout << "Count     : " << s.count << "\n";
        cout << "Average   : " << s.mean  << " " << s.unit << "\n";
        cout << "Min       : " << s.min   << " " << s.unit << "\n";
        cout << "Max       : " << s.max   << " " << s.unit << "\n";
        cout << "Std Dev   : " << s.stddev << " " << s.unit << "\n";
    }




//läser all lagrade mätvärden
void MeasurementStorage::printAll() const
{
    if (measurements.empty()) {
        cout << "[No measurements stored]\n";
        return;
    }

    const int tsW = 20;
    const int sensorW =18;
    const int valW = 10;
    const int unitW = 8;

    //visar headern
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

    //visar varje rad inom matvärdena
    for (const auto& m : measurements) {
        cout << left
        << setw(tsW) << m.timestamp
        << setw(sensorW) << m.sensorName
        << setw(valW) << m.value
        << right
        << setw(unitW) << m.unit
        << "\n";
    }
}

void MeasurementStorage::printAll(const string& sensorName) const {
    bool any = false;
      
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

         for (const auto& m : measurements) {
             if (m.sensorName != sensorName) continue;
             any = true;
             cout << left
            << setw(tsW) << m.timestamp
            << setw(sensorW) << m.sensorName
            << setw(valW) << m.value
            << right
            << setw(unitW) << m.unit
            << "\n";
    }
    if (!any) {
        cout << "[No measurements stored for sensor: " << sensorName << "]\n";
    }
}

//hjälpmedel för analys
static inline string trim(string s){
    auto isspace_ = [](unsigned char c) {return isspace(c); };
    s.erase(s.begin(), find_if(s.begin(), s.end(), [&](unsigned char c){return !isspace_(c);}));
    s.erase(find_if(s.rbegin(), s.rend(), [&](unsigned char c){ return !isspace_(c);}).base(), s.end());
    return s;
}

bool MeasurementStorage::saveToCSV(const string& filename) const {
    ofstream out(filename);
    if (!out) {
        cerr << "Error. Could not open file for writing: " << filename << "\n";
        return false;
    }
    //skriver i rader som visar datum och sensor
    for (const auto& m: measurements) {
        out << m.timestamp << ", "
            << m.sensorName << ", "
            << m.value << ", "
            << m.unit << "\n";
    }

    return true;

}

bool MeasurementStorage::loadFromCSV(const string& filename) {
    ifstream in(filename);
    if (!in) {
        cerr << "Error! Could not open fil for reading: " << filename << "\n";
        return false;
    }

    string line;
    size_t added = 0, skipped = 0;

    while (getline(in, line)) {
        if (line.empty()) {skipped++; continue;}

        istringstream ss(line);
        string ts, name, valueStr, unit;


        //delar i 4 delar
        if (!getline(ss, ts, ',')) {skipped++; continue;}
        if (!getline(ss, name, ',')) {skipped++; continue;}
        if (!getline(ss, valueStr, ',')) {skipped++; continue;}
        if (!getline(ss, unit)) {skipped++; continue;}

        ts = trim(ts);
        name = trim(name);
        valueStr= trim(valueStr);
        unit = trim(unit);

        //konvertera värde till double
        double v = 0.0;
        try {
            v = stod(valueStr);
        } catch (...) {
            skipped;
            continue; //ignorera nummerisk fält

        }

        //lägger till i lager
        measurements.push_back(Measurement{name, unit, v, ts});
        added++;
    }

    if (skipped > 0) {
        cerr << "Info Loaded " << added << " rows; skipped" << skipped << " malformed rows.\n";
    }
    return true;
}