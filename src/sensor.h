#include <string>
#include <stdexcept>
#include <iostream>
#pragma once

using namespace std;


class Sensor
{

public:
    //virtuell destruktor
    virtual ~Sensor() = default;

    //virtuell interface, subklasser för implementation
    virtual double read() = 0;
    virtual string name() const = 0;
    virtual string unit() const = 0;

    
};
