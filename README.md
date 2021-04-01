# Ardunio-Averager

Simple Class to handle rolling average of any type and size.
Ex:
```C++
// average, 10 unsiged ints, sum in a double to prevent overflow
Averager<unsigned int, double> Temperature(10);

unsigned int ave = Temperature.push(10);
```

