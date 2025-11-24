#include <vector>
#include <memory>
#include <string>
#include "sensor.h"
#include "MeasurementStorage.h"
#include "AlarmManager.h"
#pragma once

using namespace std;

class MenuController {
public: 
//Standard ingångspunkt som kör meny loop
static void runMenu(
        vector<unique_ptr<Sensor>>& sensors, 
        MeasurementStorage& storage,
        AlarmManager& alarms
    );

private:
//helper funkttioner
static int menuChoice(int min, int max);
static string readLine(const string& prompt);

static int chooseSensorByNumber(const vector<unique_ptr<Sensor>>& sensors);

static void readAllSensors(const vector<unique_ptr<Sensor>>& sensors, MeasurementStorage& storage, AlarmManager& alarms);

static void showStatsForChosenSensor(const vector<unique_ptr<Sensor>>& sensors, const MeasurementStorage& storage);

static void saveCSV(MeasurementStorage& storage);
static void loadCSV(MeasurementStorage& storage);

//automatist mätläge som läser alla sensorer var sekund & totalt gånger
static void autoMeasureInterval(const vector<unique_ptr<Sensor>>& sensors, MeasurementStorage& storage, AlarmManager& alarms);


//användaren använder sensorer, sätter begräsning och riktning
static void configureThreshold(vector<unique_ptr<Sensor>>& sensors, AlarmManager& alarms);

static void searchMeasurements(const vector<unique_ptr<Sensor>>& sensors, const MeasurementStorage& storage);


//helper för alarm
static void showAlarms(const AlarmManager& alarms);



};