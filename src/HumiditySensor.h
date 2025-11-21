#include "sensor.h"
#include <string>
#pragma once

using namespace std;

class HumiditySensor : public Sensor {
private: 
    string _id;
    double _minV;
    double _maxV;


public:
    HumiditySensor(const string& id, double minV, double maxV);

    double read() override;
    string name() const override;
    string unit() const override;
    
};