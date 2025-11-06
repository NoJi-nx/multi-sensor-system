#include <vector>
#include <string>
#include <iostream>
#include "measurement.h"
#pragma once

using namespace std;

//hanterar lagring och visar mätvärden
class MeasurementStorage {
    private:
    vector<Measurement> measurements; //lagrar alla mätvärden

    public: 
    //lägger till nya mätvärden
    void addMeasurement(const Measurement& m);

    //visar alla lagrade mätvärden
    void printAll() const;

    // lägger statisitk resultat  för deklaration - Del C
    struct Stats{
        size_t count = 0;
        double mean = 0.0;
        double min = 0.0;
        double max = 0.0;
        double stddev = 0.0;
        string unit;
        bool hasData = false;
    };

    //statistik för en sensor (namn)
    Stats computeStats(const string& sensorName) const;

    //skrivs ut statistik 
    void printStats(const string& sensorName) const;

    //filtrerat resultat per sensor
    void printAll(const string& sensorName) const;

    //antal lagrade mätvärden
    size_t size() const noexcept {return measurements.size(); }

    //read-only för hjälpmedel i analy/testt
    const vector<Measurement>& data() 
    const { 
        return measurements; 
        }


   //file I/O
   bool saveToCSV(const string& filename) const;

   //laddar mätvärden & ignorerar linjer
   bool loadFromCSV(const string& filename);


   void addReading(
       const string& sensorName, const string& unit,
       double value, const string& ts
       ) {
           measurements.push_back(Measurement{sensorName, unit, value, ts});
       }
};