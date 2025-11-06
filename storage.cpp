#include <iostream>
#include "storage.h"
#include <iomanip>

using namespace std;

//lägger nya matvärden
void MeasurementStorage::addMeasurement(const Measurement& m) 
{
    measurements.push_back(m);

}

//läser all lagrad matvärden
void MeasurementStorage::printAll() const
{
    if (measurements.empty()) {
        cout << "[No measurements stored]\n";
        return;
    }

    //visar headern
    cout << left
    << setw(20) << "Timestamp"
        << setw(18) << "Sensor"
        << setw(10) << "Value"
        << setw(8) << "Unit"
        << "\n";

     //visar seperat linje   
    cout << string(20 + 18 + 10 + 8, '-') << "\n";

    //visar varje rad inom matvärdena
    for (const auto& m : measurements) {
        cout << left
        << setw(20) << m.timestamp
        << setw(18) << m.sensorName
        << setw(10) << m.value
        << setw(8) << m.unit
        << "\n";
    }
}