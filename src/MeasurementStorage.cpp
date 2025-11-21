#include <iostream>
#include "MeasurementStorage.h"
#include <iomanip>
#include <limits>
#include <cmath>
#include <algorithm>
#include <string>
#include <sstream>
#include <fstream>
#include <cctype> //kika områden när man trimmar text
#include <map>

using namespace std;


//
namespace {
    // tar bort utrymme från början och slutet av texten
    inline string trim(string s) {
        auto isspace_ = [](unsigned char c){return isspace(c); };
        //från start
        s.erase(s.begin(), find_if(s.begin(), s.end(),
                [&](unsigned char c){return !isspace_(c); }));
        //från slut
        s.erase(find_if(s.rbegin(), s.rend(),
                [&](unsigned char c){return !isspace_(c); }).base(),
                s.end());   
          return s;           
    }
}

// lägger en ny mätävrde object till listan
void MeasurementStorage::addMeasurement(const Measurement& m)
{
    _measurements.push_back(m); // samlar värden
}

// beräknar statistik (medelmvärde, min, max etc) för en specific sensor
MeasurementStorage::Stats
MeasurementStorage::computeStats(const string &sensorName) const
{
    Stats stats; // samlar resultaten på statistiken //DOMINIK->(type Stats stats for clarity)

    // filtrerar värden
    double minv = numeric_limits<double>::infinity();
    double maxv = -numeric_limits<double>::infinity();

    // Welford's online algoritm för  medel/varians (https://en.wikipedia.org/wiki/Algorithms_for_calculating_variance#Welford's_online_algorithm)
    // variabel för Welford till beräkna medelvärde & standardavvvikelse
    size_t n = 0;      // antal readings
    double mean = 0.0; // medelvärdet
    double M2 = 0.0;   // summan av kvadratsskillnad

    // hanterar varje lagrad mätvärde
    for (const auto& measurement : _measurements)
    { // DOMINIK->(I would type measurement : measurements for better clarity)
        if (measurement.sensorName != sensorName)
            continue;

        // sparar enheten på sensor
        if (!stats.hasData)
        {
            stats.unit = measurement.unit; // antal enhet per sensor
            stats.hasData = true;
            stats.firstTimestamp = measurement.timestamp;
            stats.lastTimestamp = measurement.timestamp;
        } else {
            if (measurement.timestamp < stats.firstTimestamp)
                stats.firstTimestamp = measurement.timestamp;
            if (measurement.timestamp > stats.lastTimestamp) 
                stats.lastTimestamp = measurement.timestamp;
        }

        // uppdaterar min/max
        if (measurement.value < minv)
            minv = measurement.value;
        if (measurement.value > maxv)
            maxv = measurement.value;

        // uppdaterar Welford algoritm för emdelvärde och standardavvikelse
        ++n;
        double delta = measurement.value - mean;
        mean += delta / static_cast<double>(n);
        double delta2 = measurement.value - mean;
        M2 += delta * delta2;
    }
    // om ingen mätvärde hittats
    if (!stats.hasData)
    {
        return stats; // hämtar tom resultat
    }

    // sparar räkningen statistik
    stats.count = n;
    stats.mean = mean;
    stats.min = minv;
    stats.max = maxv;

    // beräknar standardavvikelse
    if (n >= 2)
    {
        double variance_sample = M2 / static_cast<double>(n - 1); // varians (n-1)
        stats.stddev = sqrt(variance_sample);
    }
    else
    {
        stats.stddev = 0.0; // om det finns inte tillräckligt värde för standardavvikelse
    }

    return stats;
}

// skriver ut räkningar på statistiken för en specifikt av sensor
void MeasurementStorage::printStats(const string &sensorName) const
{
   Stats stats = computeStats(sensorName); // räknar först

    // om ej data finns
    if (!stats.hasData)
    {
        cout << "No measurements: " << sensorName << "\n";
        return;
    }

    cout << "Time span : " << stats.firstTimestamp << "  ->  " << stats.lastTimestamp << "\n";

    // skriver resultatet i format
    cout << "Statistics for sensor: " << sensorName << "\n";
    cout << string(26 + sensorName.size(), '-') << "\n";

    cout << fixed << setprecision(2);
    cout << "Count     : " << stats.count << "\n";
    cout << "Average   : " << stats.mean << " " << stats.unit << "\n";
    cout << "Min       : " << stats.min << " " << stats.unit << "\n";
    cout << "Max       : " << stats.max << " " << stats.unit << "\n";
    cout << "Std Dev   : " << stats.stddev << " " << stats.unit << "\n";
}

void MeasurementStorage::printHistogram(const string& sensorName) const
{
    vector<double> values;
    string unit;

    for (const auto& m : _measurements) {
        if (m.sensorName != sensorName)
        continue;

        values.push_back(m.value);
        if (unit.empty()) {
            unit = m.unit; //minnas enhet för visning
        }
    }

    if (values.empty()) {
        cout << "No measurements for sensor: " << sensorName << "\n";
        return;
    }

    map<int, size_t> freq;

    for (double v : values) {
        int key = static_cast<int>(round(v));
        freq[key]++;
    }

    cout << "\nASCII histogram for " << sensorName << " (unit: " << unit << ")\n";
    cout << string(40 + sensorName.size(), '-') << "\n";

    cout << fixed << setprecision(1);

    for (const auto& entry : freq) {
        int rounded = entry.first;
        size_t count = entry.second;

        //bygger samling av * baserad på räkning
        string bar(count, '*');

        cout << " " << setw(5) << rounded << " " << unit << " | "
             << bar << "\n";
    }
}

// skriver alla lagrad mätvärde i en tabell format
void MeasurementStorage::printAll() const
{
    if (_measurements.empty())
    {
        cout << "No measurements stored\n";
        return;
    }

    // column punk för varje column
    const int tsW = 20;
    const int sensorW = 18;
    const int valW = 10;
    const int unitW = 8;

    // visar headern/titeln
    cout << left // DOMINIK->(Nice formatting!)
         << setw(tsW) << "Timestamp"
         << setw(sensorW) << "Sensor"
         << right
         << setw(valW) << "Value"
         << setw(unitW) << "Unit"
         << "\n";

    // visar seperat linje
    cout << string(tsW + sensorW + valW + unitW, '-') << "\n";
    cout << fixed << setprecision(2);

    // visar varje rad inom mätvärdena genom loop
    for (const auto& measurement : _measurements)
    {
        cout << left
             << setw(tsW) << measurement.timestamp
             << setw(sensorW) << measurement.sensorName
             << right
             << setw(valW) << measurement.value
             << setw(unitW) << measurement.unit
             << "\n";
    }
}

// skrive ut endast de mätvärden som tillhör till en specifikt sensor
void MeasurementStorage::printAll(const string &sensorName) const
{
    bool found = false; // kikar om det har hittat något //DOMINIK->(maybe name it found?)

    // skriver ut titeln
    const int tsW = 20;
    int sensorW = max<int>(18, static_cast<int>(sensorName.size()));
    const int valW = 10;
    const int unitW = 8;

    cout << left
         << setw(tsW) << "Timestamp"
         << setw(sensorW) << "Sensor"
         << right
         << setw(valW) << "Value"
         << setw(unitW) << "Unit"
         << "\n";

    cout << string(tsW + sensorW + valW + unitW, '-') << "\n";
    cout << fixed << setprecision(2);

    // skriver ut endast rader som tillhör specifikt sensor
    for (const auto& measurement : _measurements)
    {
        if (measurement.sensorName != sensorName)
            continue;
        found = true;
        cout << left
             << setw(tsW) << measurement.timestamp
             << setw(sensorW) << measurement.sensorName
             << right
             << setw(valW) << measurement.value
             << setw(unitW) << measurement.unit
             << "\n";
    }
    // visar meddelandet om ingen data finns
    if (!found)
    {
        cout << "No measurements stored for sensor: " << sensorName << "]\n";
    }
}

void MeasurementStorage::printSearchResults(const string& sensorName, const string& fromTs, const string& toTs) const 
{
    bool any = false;

    //samma kolumn som printAll()
    const int tsW = 20;
    int sensorW = max<int>(18, static_cast<int>(sensorName.size()));
    const int valW = 10;
    const int unitW = 8;

    cout << left
         << setw(tsW) << "Timestamp"
         << setw(sensorW) << "Sensor"
         << right
         << setw(valW) << "Value"
         << setw(unitW) << "Unit"
         << "\n";

    cout << string(tsW + sensorW + valW + unitW, '-') << "\n";
    cout << fixed << setprecision(2);

    // skriver ut endast rader som tillhör specifikt sensor
    for (const auto& m : _measurements)
    {
        if (m.sensorName != sensorName)
            continue;

       //filtrera     
        if (!fromTs.empty() && m.timestamp < fromTs)
            continue;    
        if (!toTs.empty() && m.timestamp > toTs)
            continue;    
        any = true;

        cout << left
             << setw(tsW) << m.timestamp
             << setw(sensorW) << m.sensorName
             << right
             << setw(valW) << m.value
             << setw(unitW) << m.unit
             << "\n";
    }
    // visar meddelandet om ingen data finns
    if (!any)
    {
        cout << "No measurements match the given filter \n";
    }

}


// sparar till CSV fil förhandgranskar
bool MeasurementStorage::saveToCSV(const string &filename) const
{
    ofstream out(filename);
    if (!out)
    {
        cerr << "Error. Could not open file for writing: " << filename << "\n";
        return false;
    }
    // skriver i varje mätvärde i en linje
    for (const auto& measurement : _measurements)
    {
        out << measurement.timestamp << ", "
            << measurement.sensorName << ", "
            << measurement.value << ", "
            << measurement.unit << "\n";
    }

    return true;
}

// laddar från en CSV file och lägger de ttill nuvarande lista
bool MeasurementStorage::loadToCSV(const string& filename)
{
    ifstream in(filename);
    if (!in)
    {
        cerr << "Error! Could not open fil for reading: " << filename << "\n";
        return false;
    }

    _measurements.clear(); //ersätter gammal data 

    string line;
    size_t added = 0, skipped = 0;

    // läser filen linje från linje
    while (getline(in, line))
    {
        if (line.empty())
        {
            skipped++;
            continue;
        }

        istringstream ss(line);
        string ts, name, valueStr, unit;

        // delar varje linje till 4 delar genom komma tecken
        if (!getline(ss, ts, ','))
        {
            skipped++;
            continue;
        }
        if (!getline(ss, name, ','))
        {
            skipped++;
            continue;
        }
        if (!getline(ss, valueStr, ','))
        {
            skipped++;
            continue;
        }
        if (!getline(ss, unit))
        {
            skipped++;
            continue;
        }

        // rensar extra utrymme
        ts = trim(ts);
        name = trim(name);
        valueStr = trim(valueStr);
        unit = trim(unit);

        // konvertera värde från text till nummer
        double v = 0.0;
        try
        {
            v = stod(valueStr);
        }
        catch (...)
        {
            skipped++;
            continue; // ignorera nummerisk fält
        }

        // lägger till i listan/lager
        _measurements.push_back(Measurement{name, unit, v, ts});
        added++;
    }

    // visar hur många linjer var i process
    if (skipped > 0)
    {
        cerr << "Info Loaded " << added << " rows; skipped" << skipped << " malformed rows.\n";
    }
    return true;
}