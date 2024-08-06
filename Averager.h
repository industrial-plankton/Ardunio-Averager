#ifndef AVERAGER_H
#define AVERAGER_H
/*
MIT License

Copyright (c) 2021 industrial-plankton

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#include <stdint.h>

template <typename T, typename S, unsigned int size>
class Averager
{
private:
    T _store[size] = {0};
    S _sum;                      // _sum variable for faster mean calculation
    unsigned char _position = 0; // _position variable for circular buffer
    unsigned char _count = 0;
    const unsigned char _size;
    mutable T average;
    mutable bool AverageCached = false;

public:
    Averager();
    ~Averager() {};
    void push(T entry);
    S Sum() const;
    T Average() const;
    T SectionAverage(uint8_t sectionSelect, uint8_t numerOfSections) const;
    void Clear();
};

template <typename T, typename S, unsigned int size>
Averager<T, S, size>::Averager() : _sum{0}, _size{size} {}

template <typename T, typename S, unsigned int size>
void Averager<T, S, size>::push(T entry)
{
    AverageCached = false;
    if (_count < _size)
    {
        _count++;
    }
    else
    {
        _sum = _sum - _store[_position];
    }
    _store[_position] = entry;
    _sum += entry;
    _position = (_position + 1) % _size; // Use modulo to handle wrap-around
}

template <typename T, typename S, unsigned int size>
S Averager<T, S, size>::Sum() const
{
    return _sum;
}

template <typename T, typename S, unsigned int size>
T Averager<T, S, size>::Average() const
{
    if (_count == 0)
    {
        return 0;
    }
    if (!AverageCached)
    {
        average = _sum / _count;
        AverageCached = true;
    }
    return average;
}

// Returns an average of a section of the rolling average buffer. sectionSelect is the index starting from 0, must be < numerOfSections. Assumes size is divisible by numerOfSections.
// Ex: SectionAverage(0, 4) will return the average of the first quarter of the buffer
template <typename T, typename S, unsigned int size>
T Averager<T, S, size>::SectionAverage(uint8_t sectionSelect, uint8_t numerOfSections) const
{
    if (_count == 0 || numerOfSections == 0 || _count < numerOfSections || sectionSelect > numerOfSections)
    {
        return 0;
    }
    S this_sum = 0;
    unsigned int sectionSize = _count / numerOfSections;
    unsigned int sectionSelectPosition = _count < size ? sectionSize * sectionSelect : (_position + 1 + sectionSize * sectionSelect) % _size;

    for (unsigned int i = 0; i < sectionSize; i++)
    {
        this_sum += _store[sectionSelectPosition];
        sectionSelectPosition = (sectionSelectPosition + 1) % _size; // Use modulo to handle wrap-around
    }

    return this_sum / sectionSize;
}

template <typename T, typename S, unsigned int size>
void Averager<T, S, size>::Clear()
{
    AverageCached = false;
    _position = 0;
    _sum = 0;
    _count = 0;
}

#endif