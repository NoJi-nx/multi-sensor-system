#include <iostream>
#pragma once

using namespace std;

//mätvärden tagen från en sensor. Lagrar data från klassen
struct Measurement {
    string sensorName; //namn på sensoren
    string unit;    // enhet på mätvärden
    double value;   //
    string timestamp; 
};