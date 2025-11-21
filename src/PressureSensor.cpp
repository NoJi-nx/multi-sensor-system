#include "PressureSensor.h"
#include <cstdlib>
#include <stdexcept>

using namespace std;

PressureSensor::PressureSensor(const string& id, double minV, double maxV)
    : _id(id), _minV(minV), _maxV(maxV)
{
    if (minV >= maxV) {
        throw invalid_argument("PressureSensor: minV must be < maxV");

    }
} 
double PressureSensor::read() {
    double r = static_cast<double>(rand()) / RAND_MAX;
    return _minV + r * (_maxV - _minV);
}

string PressureSensor::name() const {
    return _id;
}

string PressureSensor::unit() const {
    return "hPA";
}