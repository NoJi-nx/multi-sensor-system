# Patch Notes

## Part B

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