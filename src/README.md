# multi-sensor-system
A prpgram that handles multiple sensors of different types


//DOMINIK CODE REVIEW BEGIN//
I have run the code and it compiles.
All functions in the menu work properly.
I like the idea that you can choose filename for the save and load file.

A good thing to do would be to name all private members with _ before the name like this:

private:
string name;
    string _unit;
    double _minValue;
    double _maxValue;

then it becomes more clear what kind of variable we are dealing with.

Storage class should not do anything but store, but it does calculations.

The method computeStats in storage.h/cpp should be private as it is not used outside of the class.

For being more user friendly, make the user choose a sensor by number instead of typing
exactly the name and risking misspellings.

The file and class should have the same name, MeasurementStorage.h/cpp


    Comments that I added:
    main.cpp    LINE 26 |  74  |  75  |  77  |  80  |  82
    sensor.h    LINE 14
    storage.cpp LINE 23 |  37  |  128 |

    //DOMINIK CODE REVIEW END//