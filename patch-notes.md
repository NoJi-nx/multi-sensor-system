# Patch Notes

## Part A

### **A.3 Create & recieve values **

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