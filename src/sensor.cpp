#include <stdexcept> // för invalid_argument
#include <cstdlib>   // rand()
#include <iostream> 
#include "sensor.h"

using namespace std;


//implementerar sensor
Sensor::Sensor(const string& name, const string& unit, double minValue, double maxValue) 
{
    //validering om range passar
    if (minValue > maxValue) {
        throw invalid_argument("Error: minValue cannot be greater than maxValue '" + name + "'");
    }
    
    if (minValue == maxValue) {
        throw invalid_argument("Error: minValue & maxValue cannot be equal '" + name + "'");
    }

    //lägger till värden efter validering till sensor
    this->name = name;
    this->unit = unit;
    this->minValue = minValue;
    this->maxValue = maxValue;
}

//simulerar returnerar slumpmässigt värde inom range 
double Sensor::read() const {
    // generar slumpmässig siffra inom  den rangen
    double randomValue = minValue + static_cast<double>(rand()) / RAND_MAX * (maxValue - minValue);
    return randomValue;
}

