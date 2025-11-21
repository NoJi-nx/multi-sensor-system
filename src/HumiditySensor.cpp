#include "HumiditySensor.h"
#include <cstdlib>
#include <stdexcept>

HumiditySensor::HumiditySensor(const string& id, double minV, double maxV)
    : _id(id), _minV(minV), _maxV(maxV)
{
    if (minV >= maxV) {
        throw invalid_argument("HumiditySensor: minV must be < maxV");

    }
} 

double HumiditySensor::read() {
    double r = static_cast<double>(rand()) / RAND_MAX;
    return _minV + r * (_maxV - _minV);
}

string HumiditySensor::name() const {
    return _id;
}

string HumiditySensor::unit() const {
    return "%";
}