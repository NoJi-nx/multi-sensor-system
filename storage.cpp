#include <iostream>
#include "storage.h"
#include <iomanip>
#include <limits>
#include <cmath> 
#include <algorithm>
#include <string>

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