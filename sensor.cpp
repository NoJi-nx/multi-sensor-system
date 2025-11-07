#include <stdexcept>
#include <cstdlib>
#include <iostream>
#include <iostream>
#include "sensor.h"

using namespace std;


//implementation
Sensor::Sensor(const string& name, const string& unit, double minValue, double maxValue) 
{
    //validering för rad
    if (minValue > maxValue) {
        throw invalid_argument("Error: minValue cannot be greater than maxValue '" + name + "'");
    }
    
    if (minValue == maxValue) {
        throw invalid_argument("Error: minValue & maxValue cannot be equal '" + name + "'");
    }

    //lägger till värden efter validering
    this->name = name;
    this->unit = unit;
    this->minValue = minValue;
    this->maxValue = maxValue;
}

//simulerar slumpmässigt värde
double Sensor::read() const {
    double randomValue = minValue + static_cast<double>(rand()) / RAND_MAX * (maxValue - minValue);
    return randomValue;
}

