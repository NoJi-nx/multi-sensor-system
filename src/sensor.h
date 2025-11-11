#include <string>
#include <iostream>
#pragma once

using namespace std;

// deklarera  sesnor data
class Sensor
{
private:
    string name;
    string unit;
    double minValue;
    double maxValue;
    // DOMINIK// - I would suggest to use a ENUM for sensor type, that shields from misspellings
public:
    // konstruktor
    // skapar en sensor och visar namn, enhet och range
    Sensor(const string &name, const string &unit, double minValue, double maxValue);

    // metod för simulera sensor och ta mätvärden från sensor inom range
    double read() const;

    // getter metoden
    string getName() const { return name; } // visar sensorens namn
    string getUnit() const { return unit; } // visar sensorens enhet
};