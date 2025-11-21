# Patch Notes

## Ver. 2 Task 1.1: Create abstract base class Sensor

```cpp
public:
    //virtuell destruktor
    virtual ~Sensor() = default;

    //virtuell interface, subklasser för implementation
    virtual double read() = 0;
    virtual string name() const = 0;
    virtual string unit() const = 0;
```
## Part E2 Threshold (VG)

#### E2.1 Add private members


```cpp
rivate:
    SensorType _type;
    string _name;
    string _unit;
    double _minValue;
    double _maxValue;
   

    //privata egenskaper för tröskelvärde
    double _threshold = 0.0;
    bool _hasThreshold = false; // om anvvändaren har satt ett tröskel 

```

## Part D
#### D.4 Build menu + sensors list + loop

```cpp
int main()
{
      //slumpmässigt genereras värden
    srand(static_cast<unsigned int>(time(nullptr)));

    //konfigurera sesnorerna -- Del A
    //definiera sensorerna
    vector<Sensor> sensors;
    sensors.emplace_back("Temperature", "°C", -10.0, 40.0);
    sensors.emplace_back("Humidity", "%", 0.0, 100.0);

    //central lagring för alla mätvärden -- Del B, C & D
    MeasurementStorage storage;

    //meny loop -- Del D
    while (true) {
        cout << "\n------ MENU ------\n"
             << "1. Read new measurements from all sensors\n"
             << "2. Display statistics for a sensor\n"
             << "3. Display all measurements\n"
             << "4. Save measurements to CSV\n"
             << "5. Load measurements from CSV\n"
             << "6. Exit\n";

      int choice = menuChoice(1, 6);

      if (choice == 1) {
          //läser nya mätvärde från varje sensor och läggs till lagring
          string ts = currentTimeStamp();
        for (auto& s : sensors) {
            double val = s.read();
            storage.addReading(s.getName(), s.getUnit(), val, ts);
        }
         cout << "OK Read " << sensors.size() << " new measurement(s) at " << ts << ".\n";
      }
      else if (choice == 2) {
          //visar statistik för en utvald sensor med exakt namn
          cout << "Available sensors:\n";
          for (const auto& s : sensors) cout << " - " << s.getName() << "\n";

          string name = readLine("Enter the exact sensor name: ");
          storage.printStats(name);

      }
      else  if (choice == 3) {
          //skriver ut  mätvärde i en tabell
          storage.printAll();
      }
      else if (choice == 4) {
          //sparar nuvarande mätvärde till CSV
          string fname = readLine("Filename to save (measurements.csv): ");
          if (fname.empty()) { cout << "Canceled!\n"; continue; }
          if (storage.saveToCSV(fname)) cout << "Saved. " << storage.size() << " rows to " << fname << "\n";
      }
      else if (choice == 5) {
          //laddar mätvärde från CSV till listan/lager
          string fname = readLine("Filename to load (measurements.csv): ");
          if (fname.empty()) { cout << "Canceled!\n"; continue; }
          storage.loadFromCSV(fname); // skriver ut meddelandet

      }

      else if (choice == 6) {
          cout << "Goodbye!\n";
          break;
      }
    }

    return 0;
}
```

#### D.3 Add a menu UI

```cpp
//läser menyval med min,max
//rensar och återhämtar från fel input
int menuChoice(int min, int max) {
    while (true) {
        cout << "\nChoose (" << min << "-" << max << "): ";
        int choice;
        if (cin >> choice && choice >= min && choice <= max) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return choice;
        }

        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input!. Please enter a number between " << min << " and " << max << ".\n";
    }
}

//prompt för text (filnamn, sensor namn etc.)
string readLine(const string& prompt){
    cout << prompt;
    string s;
    getline(cin, s);
    return s;
}

```
#### D.2 Implement CSV functions (save & load) + helpers



```cpp
//tar bort utrymme från början och slutet av texten
static inline string trim(string s){
    auto isspace_ = [](unsigned char c) {return isspace(c); };

    //tar bort från start
    s.erase(s.begin(), find_if(s.begin(), s.end(), [&](unsigned char c){return !isspace_(c);}));

    //tar bort från slutet
    s.erase(find_if(s.rbegin(), s.rend(), [&](unsigned char c){ return !isspace_(c);}).base(), s.end());
    return s;
}

//sparar till CSV fil förhandgranskar
bool MeasurementStorage::saveToCSV(const string& filename) const {
    ofstream out(filename);
    if (!out) {
        cerr << "Error. Could not open file for writing: " << filename << "\n";
        return false;
    }
    //skriver i varje mätvärde i en linje
    for (const auto& m: measurements) {
        out << m.timestamp << ", "
            << m.sensorName << ", "
            << m.value << ", "
            << m.unit << "\n";
    }

    return true;

}

//laddar från en CSV file och lägger de ttill nuvarande lista
bool MeasurementStorage::loadFromCSV(const string& filename) {
    ifstream in(filename);
    if (!in) {
        cerr << "Error! Could not open fil for reading: " << filename << "\n";
        return false;
    }

    string line;
    size_t added = 0, skipped = 0;

    //läser filen linje från linje
    while (getline(in, line)) {
        if (line.empty()) {skipped++; continue;}

        istringstream ss(line);
        string ts, name, valueStr, unit;


        //delar varje linje till 4 delar genom komma tecken
        if (!getline(ss, ts, ',')) {skipped++; continue;}
        if (!getline(ss, name, ',')) {skipped++; continue;}
        if (!getline(ss, valueStr, ',')) {skipped++; continue;}
        if (!getline(ss, unit)) {skipped++; continue;}

        //rensar extra utrymme
        ts = trim(ts);
        name = trim(name);
        valueStr= trim(valueStr);
        unit = trim(unit);

        //konvertera värde från text till nummer
        double v = 0.0;
        try {
            v = stod(valueStr);
        } catch (...) {
            skipped++;
            continue; //ignorera nummerisk fält

        }

        //lägger till i listan/lager
        measurements.push_back(Measurement{name, unit, v, ts});
        added++;
    }

    //visar hur många linjer var i process
    if (skipped > 0) {
        cerr << "Info Loaded " << added << " rows; skipped" << skipped << " malformed rows.\n";
    }
    return true;
}
```
#### D.1 Add file I/O API for CSV management

```cpp
//lägger resulterad statisitk  i en struktur  för deklaration - Del C
    struct Stats{
        size_t count = 0;
        double mean = 0.0;
        double min = 0.0;
        double max = 0.0;
        double stddev = 0.0;
        string unit;
        bool hasData = false;
    };

    //statistik för en typ av sensor (skrivs inte ut)
    Stats computeStats(const string& sensorName) const;

    //skrivs ut statistik 
    void printStats(const string& sensorName) const;

    //filtrerat resultat per sensor
    void printAll(const string& sensorName) const;

    //antal lagrade mätvärden
    size_t size() const noexcept {return measurements.size(); }

    //read-only för få tillgång till rådata
    const vector<Measurement>& data() const { return measurements; }


   //file I/O (CSV) 
   //sparar till CSV
   bool saveToCSV(const string& filename) const;

   //laddar mätvärden & ignorerar linjer
   bool loadFromCSV(const string& filename);


    //bygger rader för bifogning
   void addReading(
       const string& sensorName, const string& unit,
       double value, const string& ts
       ) {
           measurements.push_back(Measurement{sensorName, unit, value, ts});
       }
```

## Part C

#### C.4 Implement a new filtered function on the header

```cpp
void MeasurementStorage::printAll(const string& sensorName) const {
    bool any = false;
      
      const int tsW = 20;
   int sensorW = max<int>(18, static_cast<int>(sensorName.size()));
    const int valW = 10;
    const int unitW = 8;

    cout << left
         << setw(tsW)     << "Timestamp"
         << setw(sensorW) << "Sensor"
         << right 
         << setw(valW)    << "Value"
         << setw(unitW)   << "Unit"
         << "\n";

         cout << string(tsW + sensorW + valW + unitW, '-') << "\n";
         cout << fixed << setprecision(2);

         for (const auto& m : measurements) {
             if (m.sensorName != sensorName) continue;
             any = true;
             cout << left
            << setw(tsW) << m.timestamp
            << setw(sensorW) << m.sensorName
            << setw(valW) << m.value
            << right
            << setw(unitW) << m.unit
            << "\n";
    }
    if (!any) {
        cout << "[No measurements stored for sensor: " << sensorName << "]\n";
    }
}
```
#### C.3 Call stats in main.cpp

```cpp
  //skriver ut värden
   storage.printAll();

   cout << "\n";
   storage.printStats("Temperature");
   cout << "\n";
   storage.printStats("Humidity");
```
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