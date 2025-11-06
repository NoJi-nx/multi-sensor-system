#include <string>
#pragma once

using namespace std;

//deklarera data 
class Sensor {
    private:
    string name;
    string unit;
    double minValue;
    double maxValue;

    public:
    //konstruktor
    Sensor(const string& name, const string& unit, double minValue, double maxValue);

    //metod för simulera sensor
    double read() const;

    //getter metod för visa värden
    string getName() const { return name;}
    string getUnit() const { return unit; }

    
};