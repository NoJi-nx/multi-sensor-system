#include "sensor.h"
#include <string>
#pragma once

using namespace std;
class PressureSensor : public Sensor {
private: 
    string _id;
    double _minV;
    double _maxV;

public:
    PressureSensor(const string& id, double minV, double maxV);

    double read() override; //implementerar Sensor::read()
    string  name() const override;
    string unit() const override;
};
