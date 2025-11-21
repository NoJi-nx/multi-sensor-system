#include "TemperatureSensor.h"
#include <cstdlib>  
#include <stdexcept>

using namespace std;

TemperatureSensor::TemperatureSensor(const string& id, double minV, double maxV)
    : _id(id), _minV(minV), _maxV(maxV)
{
    if (minV >= maxV) {
        throw invalid_argument("TemperatureSensor: minV must be < maxV");

    }
}

double TemperatureSensor::read() {
     double r = static_cast<double>(rand()) / RAND_MAX; // 0.1
     return _minV + r * (_maxV - _minV);
}

string TemperatureSensor::name() const {
    return _id;
}

string TemperatureSensor::unit() const {
    return "°C";
}
  