#include <vector>
#include <string>
#include <iostream>
#include "measurement.h"
#pragma once

using namespace std;

//hanterar lagring och visar mätvärden
class MeasurementStorage {
    private:
    //all mätvärden lagras här
    vector<Measurement> measurements; //lagrar alla mätvärden

    public: 
    //lägger till nya mätvärden -- Del B
    void addMeasurement(const Measurement& m);

    //skriver ut lagrad mätvärden i en formatterad tabell
    void printAll() const;

    //lägger resulterad statisitk  i en struktur  för deklaration - Del C
    struct Stats{
        size_t count = 0;
        double mean = 0.0;
        double min = 0.0;
        double max = 0.0;
        double stddev = 0.0;
        string unit;
        bool hasData = false;
    };

    //statistik för en typ av sensor (skrivs inte ut)
    Stats computeStats(const string& sensorName) const;

    //skrivs ut statistik 
    void printStats(const string& sensorName) const;

    //filtrerat resultat per sensor
    void printAll(const string& sensorName) const;

    //antal lagrade mätvärden
    size_t size() const noexcept {return measurements.size(); }

    //read-only för få tillgång till rådata
    const vector<Measurement>& data() const { return measurements; }


   //file I/O (CSV) 
   //sparar till CSV
   bool saveToCSV(const string& filename) const;

   //laddar mätvärden & ignorerar linjer
   bool loadFromCSV(const string& filename);


    //bygger rader för bifogning
   void addReading(
       const string& sensorName, const string& unit,
       double value, const string& ts
       ) {
           measurements.push_back(Measurement{sensorName, unit, value, ts});
       }
};