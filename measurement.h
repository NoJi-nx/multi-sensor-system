#include <iostream>
#pragma once

using namespace std;

//mätvärden tagen från en sensor. Lagrar data från klassen
struct Measurement {
    string sensorName;
    string unit;
    double value;
    string timestamp; 
};