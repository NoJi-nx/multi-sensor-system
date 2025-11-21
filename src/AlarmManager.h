#pragma once
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

//definierar data typer för alarm
struct Threshold{
    string sensorName;
    double limit;
    bool over; // if-satser
};

struct AlarmEvent {
    string timeStamp;
    string sensorName;
    string unit;
    double value;
    double limit;
    bool over;
};

//lagrar regler + alarm log
class AlarmManager {
    private:
        vector<Threshold> _thresholds;
        vector<AlarmEvent> _alarms;

    public:
        void setThreshold(const Threshold& t);
        void clearThresholdFor(const string& sensorName);

        bool hasThresholdFor(const string& sensorName) const;  
        const Threshold* getThresholdFor(const string& sensorName) const; 

        void check(const string& sensorName,
                   double value,
                   const string& unit,
                   const string& timeStamp);

        void printAlarms() const;
        bool empty() const { return _alarms.empty(); }           
};