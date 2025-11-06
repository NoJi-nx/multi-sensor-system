# Patch Notes

## Part A
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