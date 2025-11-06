#include "sensor.h"

using namespace std;


//implementation
Sensor::Sensor(const string& name, const string& unit, double minValue, double maxValue) 
: name(name), unit(unit), minValue(minValue), maxValue(maxValue)
{}

//simulerar slumpmässigt värde
double Sensor::read() const {
    double randomValue = minValue + static_cast<double>(rand()) / RAND_MAX * (maxValue - minValue);
    return randomValue;
}

string Sensor::getName() const {
    return name;
}