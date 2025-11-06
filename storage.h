#include <vector>
#include "measurement.h"
#pragma once

using namespace std;

//hanterar lagring och visar flera sensor matvärden
class MeasurementStorage {
    private:
    vector<Measurement> measurements; //lager som hålls alla matvärden

    public: 
    //lägger till nya matvärden
    void addMeasurement(const Measurement& m);

    //visar alla lagrad matvärden
    void printAll() const;

    //analys, read.only tillgång
    const vector<Measurement>& data() 
    const { 
        return measurements; 
        }
};