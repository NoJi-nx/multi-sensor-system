#include <iostream>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <chrono>
#include "utils.h"


using namespace std;

//genererar och hämtar nuvarande tidsstämpel
string currentTimeStamp() {
    using namespace chrono;
    auto now = system_clock::now(); //Nuvarande tidsspunkt
    time_t t = system_clock::to_time_t(now); // Konverttera till time_t 
    tm tm{};   //Tidsstruktur
#if defined(_WIN32) 
localtime_s(&tm, &t);
#else 
localtime_r(&t, &tm);
#endif 
ostringstream oss; 
oss << put_time(&tm, "%Y-%m-%d %H:%M:%S");
return oss.str();

}