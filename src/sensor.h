#include <string>
#include <stdexcept>
#include <iostream>
#pragma once

using namespace std;

enum class SensorType{
    Temperature, 
    Humidity,
    Unknown
};
// deklarera  sesnor data
class Sensor
{
private:
    SensorType _type;
    string _name;
    string _unit;
    double _minValue;
    double _maxValue;
    // DOMINIK// - I would suggest to use a ENUM for sensor type, that shields from misspellings
public:
    // konstruktor
    // skapar en sensor och visar namn, enhet och range
    Sensor(SensorType type, const string& name, const string& unit, double minValue, double maxValue);

    // metod för simulera sensor och ta mätvärden från sensor inom range
    double read() const;
    // getter metoden
    string getName() const { return _name; } //returnerar läsbar namn baserad på type
    string getUnit() const { return _unit; } // visar sensorens enhet
    SensorType getType() const { return _type; }
};