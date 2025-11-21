#include "AlarmManager.h"
#include <iostream>
#include <iomanip>

using namespace std;

//implementera alarm funktioner
void AlarmManager::setThreshold(const Threshold & t) {
    //en tröskelvärde per sensor
    for (auto& existing : _thresholds) {
        if (existing.sensorName == t.sensorName) {
            existing = t;
            return;
        }
    }
    _thresholds.push_back(t);
}

void AlarmManager::clearThresholdFor(const string& sensorName) {
    _thresholds.erase(
        remove_if(_thresholds.begin(), _thresholds.end(), 
            [&](const Threshold& th){ return th.sensorName == sensorName; }),
            _thresholds.end()
    );
}

bool AlarmManager::hasThresholdFor(const string& sensorName) const {
    return getThresholdFor(sensorName) != nullptr;
}

const Threshold* AlarmManager::getThresholdFor(const string & sensorName) const {
    for (const auto& t: _thresholds) {
        if (t.sensorName == sensorName) return &t;
    }
    return nullptr;

void AlarmManager::check(const string& sensorName,
                         double value,
                         const string& unit,
                         const string timeStamp)
    {
        const Threshold* t = getThresholdFor(sensorName);
        if (!t) return;

        bool triggered = (t->over && value > t->limit) ||
                         (!t-> over && value < t->limit);
         
        if (triggered){
            _alarms.push_back(AlarmEvent{
                timeStamp,
                sensorName,
                unit,
                value,
                t->limit,
                t->over
            });
        }
    }                        
}

void AlarmManager::printAlarms() const {
    if (_alarms.empty()) {
        cout << "No alarms triggered\n";
        return;
    }

    cout << "\n---- Recent Alarms ----\n";
    cout << left
         << setw(20) << "Timestamp"
         << setw(18) << "Sensor"
         << right
         << setw(10) << "Value"
         << setw(8)  << "Unit"
         << "  Rule\n";

    cout << string(20+18+10+8+8, '-') << "\n";
    cout << fixed << setprecision(2);

    for (const auto& a : _alarms) {
    cout << left
         << setw(20) << a.timeStamp
         << setw(18) << a.sensorName
         << right
         << setw(10) << a.value
         << setw(8)  << a.unit
         << "  "
         << (a.over ? ">" : "<")
         << a.limit
         << "\n";
    }     
}