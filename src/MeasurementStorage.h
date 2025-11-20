#include <vector>
#include <string>
#include <iostream>
#include "measurement.h"
#pragma once

using namespace std;

//hanterar lagring och visar mätvärden
class MeasurementStorage {
  public: 
    //lägger resulterad statisitk  i en struktur  för deklaration - Del C
    struct Stats{
        size_t count = 0;
        double mean = 0.0;
        double min = 0.0;
        double max = 0.0;
        double stddev = 0.0;
        string unit;
        bool hasData = false;
        string firstTimestamp;
        string lastTimestamp;
    };

private:
    //all mätvärden lagras här
    vector<Measurement> _measurements; //lagrar alla mätvärden

    //statistik för en typ av sensor (skrivs inte ut)
    Stats computeStats(const string& sensorName) const;

  public:
    //lägger till nya mätvärden -- Del B
    void addMeasurement(const Measurement& m);

     //bygger rader för bifogning
   void addReading(
       const string& sensorName, 
       const string& unit,
       double value, 
       const string& timeStamp) 
       {
           _measurements.push_back(Measurement{sensorName, unit, value, timeStamp});
       }

    //skriver ut lagrad mätvärden i en formatterad tabell
    void printAll() const;

    //skrivs ut statistik 
    void printStats(const string& sensorName) const;

    //filtrerat resultat per sensor
    void printAll(const string& sensorName) const;

    //skriver ut  en ASCII histogram av värden för en sensor
    void printHistogram(const string& sensorName) const;

    //skriver ut mätvärrden som är filterad av sensors namn
    void printSearchResults ( const string& sensorName, const string& fromTs, const string& toTs) const;
    
    //antal lagrade mätvärden
    size_t size() const noexcept {return _measurements.size(); }

    //read-only för få tillgång till rådata 
    const vector<Measurement>& data() const { return _measurements; }



   //file I/O (CSV) 
   //sparar till CSV
   bool saveToCSV(const string& filename) const;

   //laddar mätvärden & ignorerar linjer
   bool loadToCSV(const string& filename);

   
};