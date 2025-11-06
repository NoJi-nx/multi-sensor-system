# Patch Notes

## Part C

#### C.2 Implement numerically stable (computeStats)

```cpp
MeasurementStorage::Stats
MeasurementStorage::computeStats(const string& sensorName) const {
    Stats s;


    //filtrerar värden 
    double minv = numeric_limits<double>::infinity();
    double maxv = -numeric_limits<double>::infinity();

    //Welford's online algoritm för  medel/varians
    //https://en.wikipedia.org/wiki/Algorithms_for_calculating_variance#Welford's_online_algorithm
    size_t n = 0;
    double mean = 0.0;
    double M2 = 0.0;

    for (const auto& m : measurements) {
        if (m.sensorName != sensorName) continue;

        if (!s.hasData) {
            s.unit = m.unit; //antal enhet per sensor
            s.hasData = true;
        }

        //uppdaterar min/max
        if (m.value < minv) minv = m.value;
        if (m.value > maxv) maxv = m.value;

        //uppdaterar Welford
        ++n;
        double delta = m.value - mean;
        mean += delta / static_cast<double>(n);
        double delta2 = m.value - mean;
        M2 += delta * delta2;

    }

        if (!s.hasData) { //ingen mätvärde för sensor
            return s; // hasdata = false

        }

        s.count = n;
        s.mean = mean;
        s.min = minv;
        s.max = maxv;

        if (n >= 2) {
            double variance_sample = M2 / static_cast<double>(n - 1); // varians (n-1)
            s.stddev = sqrt(variance_sample);
        } else {
            s.stddev = 0.0; // definiera som 0.0 vid ett värde
        }

        return s;

    }

    void MeasurementStorage::printStats(const string& sensorName) const {
        auto s = computeStats(sensorName);

        if (!s.hasData) {
            cout << "No measurements: " << sensorName << "\n";
            return;
        }

        cout << "Statistics for sensor: " << sensorName << "\n";
        cout << string(26 + sensorName.size(), '-') << "\n";

        cout << fixed << setprecision(2);
        cout << "Count     : " << s.count << "\n";
        cout << "Average   : " << s.mean  << " " << s.unit << "\n";
        cout << "Min       : " << s.min   << " " << s.unit << "\n";
        cout << "Max       : " << s.max   << " " << s.unit << "\n";
        cout << "Std Dev   : " << s.stddev << " " << s.unit << "\n";
    }




//läser all lagrade mätvärden
void MeasurementStorage::printAll() const
{
    if (measurements.empty()) {
        cout << "[No measurements stored]\n";
        return;
    }

    const int tsW = 20;
    const int sensorW =18;
    const int valW = 10;
    const int unitW = 8;

    //visar headern
    cout << left
    << setw(tsW) << "Timestamp"
        << setw(sensorW) << "Sensor"
        << right
        << setw(valW) << "Value"
        << setw(unitW) << "Unit"
        << "\n";

     //visar seperat linje   
    cout << string(tsW + sensorW + valW + unitW, '-') << "\n";
    cout << fixed << setprecision(2);

    //visar varje rad inom matvärdena
    for (const auto& m : measurements) {
        cout << left
        << setw(tsW) << m.timestamp
        << setw(sensorW) << m.sensorName
        << setw(valW) << m.value
        << right
        << setw(unitW) << m.unit
        << "\n";
    }
}
```
#### C.1  Stats result type & function declaration

```cpp
//hanterar lagring och visar mätvärden
class MeasurementStorage {
    private:
    vector<Measurement> measurements; //lagrar alla mätvärden

    public: 
    //lägger till nya mätvärden
    void addMeasurement(const Measurement& m);

    //visar alla lagrade mätvärden
    void printAll() const;

    // lägger statisitk resultat  för deklaration - Del C
    struct Stats{
        size_t count = 0;
        double mean = 0.0;
        double min = 0.0;
        double max = 0.0;
        double stddev = 0.0;
        string unit;
        bool hasData = false;
    };

    //statistik för en sensor (namn)
    Stats computeStats(const string& sensorName) const;

    //skrivs ut statistik 
    void printStats(const string& sensorName) const;

    //filtrerat resultat per sensor
    void printAll(const string& sensorName) const;

    //antal lagrade mätvärden
    size_t size() const noexcept {return measurements.size(); }

    //read-only för hjälpmedel i analy/testt
    const vector<Measurement>& data() 
    const { 
        return measurements; 
        }
};

```
## Part B


#### B.3 Implement measurement storage in main.cpp

```cpp
int main()
{
      //slumpmässigt genereras värden
    srand(static_cast<unsigned int>(time(nullptr)));

    //skapa två sensor
    Sensor tempSensor("TempSensor 1", "°C", -10.0, 40.0);
    Sensor humiditySensor("HumiditySensor 1", "%", 0.0, 100.0);

    //Lagring 
    MeasurementStorage storage;

    //Fel sensor
    //Sensor brokenSensor("FaultySensor", "°C", 50.0, -10.0); //Ta bort för testa

//simulerar & hämtar värden
    double tempValue = tempSensor.read();
    double humidityValue = humiditySensor.read();

    //skapar matvärde för lagring
    Measurement m1 { tempSensor.getName(), "°C", tempValue, currentTimeStamp() };
    Measurement m2 { humiditySensor.getName(), "%", humidityValue, currentTimeStamp() };

  storage.addMeasurement(m1);
   storage.addMeasurement(m2);

   //demo
   storage.printAll();

    /*//visar värden
    cout << tempSensor.getName() <<":°C" << tempValue << " °C" << endl;
    cout << humiditySensor.getName() << ": " << humidityValue << " %\n" << endl;*/

    return 0;
}

```
#### B.2 Create storage manager class & implement

```cpp
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
```

```cpp
//lägger nya matvärden
void MeasurementStorage::addMeasurement(const Measurement& m) 
{
    measurements.push_back(m);

}

//läser all lagrad matvärden
void MeasurementStorage::printAll() const
{
    if (measurements.empty()) {
        cout << "[No measurements stored]\n";
        return;
    }

    //visar headern
    cout << left
    << setw(20) << "Timestamp"
        << setw(18) << "Sensor"
        << setw(10) << "Value"
        << setw(8) << "Unit"
        << "\n";

     //visar seperat linje   
    cout << string(20 + 18 + 10 + 8, '-') << "\n";

    //visar varje rad inom matvärdena
    for (const auto& m : measurements) {
        cout << left
        << setw(20) << m.timestamp
        << setw(18) << m.sensorName
        << setw(10) << m.value
        << setw(8) << m.unit
        << "\n";
    }
}
```

#### B.1 Create measurement container & timestamp

```cpp
//matvärden tagen från en sensor. Lagrar data från klassen
struct Measurement {
    string sensorName;
    string unit;
    double value;
    string timestamp; 
};
```

```cpp
//nuvarande system för formatterad tidsstämpel
string currentTimeStamp(); // YYY--MM--DD HH:MM:SS
```

```cpp
//genererar och hämtar nuvvarande tidsstämpel
string currentTimeStamp() {
    using namespace chrono;
    auto now = system_clock::now(); //Nuvarande tidsspunkt
    time_t t = system_clock::to_time_t(now); // Konverttera till time_t 
    tm tm{};     // Tidsstruktur
#if defined(_WIN32) 
localtime_s(&tm, &t);
#else 
localtime_r(&t, &tm);
#endif 
ostringstream oss; 
oss << put_time(&tm, "%Y-%m-%d %H:%M:%S");
return oss.str();

}
```
* Using currentTimestamp to get a formatted string (YYY--MM--DD HH:MM:SS)
## Part A

#### A.4 Range Validation (Optional)

```cpp
Sensor::Sensor(const string& name, const string& unit, double minValue, double maxValue) 
{
    //validering 
    if (minValue > maxValue) {
        throw invalid_argument("Error: minValue cannot be greater than maxValue '" + name + "'");
    }
    
    if (minValue == maxValue) {
        throw invalid_argument("Error: minValue & maxValue cannot be equal '" + name + "'");
    }

    //lägger till värden efter validering
    this->name = name;
    this->unit = unit;
    this->minValue = minValue;
    this->maxValue = maxValue;
}
```

#### **A.3 Create & recieve values **

```cpp
//slumpmässigt genereras värden
    srand(static_cast<unsigned int>(time(nullptr)));

    //skapa två sensor
    Sensor tempSensor("TempSensor 1", "°C", -10.0, 40.0);
    Sensor humiditySensor("HumiditySensor 1", "%", 0.0, 100.0);


    //simulerar & hämtar värden
    double tempValue = tempSensor.read();
    double humidityValue = humiditySensor.read();

    //visar värden
    cout << tempSensor.getName() <<": " << tempValue << " °C" << endl;
    cout << humiditySensor.getName() << ": " << humidityValue << " %\n" << endl;

    return 0;
```
* Using srand(time(nullptr)) to get different random numbers each run

#### **A.2 Implement sensor class**

``` cpp
//implementerar konstrukttör
Sensor::Sensor(const string& name, const string& unit, double minValue, double maxValue) 
: name(name), unit(unit), minValue(minValue), maxValue(maxValue)
{}

//simulerar slumpmässigt värde
double Sensor::read() const {
    double randomValue = minValue + static_cast<double>(rand()) / RAND_MAX * (maxValue - minValue);
    return randomValue;
}

string Sensor::getName() const {
    return name;
}
`````` cpp
//implementerar konstrukttör
Sensor::Sensor(const string& name, const string& unit, double minValue, double maxValue) 
: name(name), unit(unit), minValue(minValue), maxValue(maxValue)
{}

//simulerar slumpmässigt värde
double Sensor::read() const {
    double randomValue = minValue + static_cast<double>(rand()) / RAND_MAX * (maxValue - minValue);
    return randomValue;
}

string Sensor::getName() const {
    return name;
}
```
* rand() / RAND_MAX generates random value between 0 & 1
* Using (maxValue - minValue) for multiplication so it can have range
#### **A.1 Setup class declaration**


```cpp
//deklarera data 
class Sensor {
    private:
    string name;
    string unit;
    double minValue;
    double maxValue;

    public:
    //konstruktor
    Sensor(const string& name, const string& unit, double minValue, double maxValue);

    //metod för simulera sensor
    double read() const;

    //getter metod för hjälp
    string getName() const;
};
```