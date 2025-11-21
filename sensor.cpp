#include <stdexcept> // för invalid_argument
#include <cstdlib>   // rand()
#include <iostream> 
#include "sensor.h"

using namespace std;

//konvertera enum till läsbar string (printing)
static string sensorTypeToString(SensorType type) {
    switch (type) {
        case SensorType::Temperature: return "Temperature";
        case SensorType::Humidity: return "Humidity";
        default: return "Unknown";
    }
}

//implementerar sensor
Sensor::Sensor(SensorType type, const string& name, const string& unit, double minValue, double maxValue)  
        : _type(type), _name(name), _unit(unit), _minValue(minValue), _maxValue(maxValue)
{
    //validering om range passar
    if (minValue > maxValue) {
        throw invalid_argument("Error: minValue cannot be greater than maxValue for " + sensorTypeToString(type));
    }
    
    if (minValue == maxValue) {
        throw invalid_argument("Error: minValue & maxValue cannot be equal for " + sensorTypeToString(type));
    }

    //lägger till värden efter validering till sensor
    _name = sensorTypeToString(type);
    
}


//simulerar returnerar slumpmässigt värde inom range 
double Sensor::read() const {
    // generar slumpmässig siffra inom  den rangen
    double randomValue = _minValue + static_cast<double>(rand()) / RAND_MAX * (_maxValue - _minValue);
    return randomValue;
}

