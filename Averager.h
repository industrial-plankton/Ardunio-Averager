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

#include <malloc.h>

template <typename T, typename S>
class Averager
{
private:
    T *_store;
    S _sum;                  // _sum variable for faster mean calculation
    unsigned char _position; // _position variable for circular buffer
    unsigned char _count;
    const unsigned char _size;

public:
    explicit Averager(const unsigned char _size);
    ~Averager();
    T push(T entry);
    S Sum() const;
    T Average() const;
};

template <typename T, typename S>
Averager<T, S>::Averager(const unsigned char size) : _sum{0}, _position{0}, _count{0}, _size{size}
{
    // _size = size;
    _store = (T *)malloc(sizeof(T) * _size);
    for (unsigned char i = 0; i < _size; i++)
    {
        _store[i] = 0;
    }
}

template <typename T, typename S>
Averager<T, S>::~Averager()
{
    free(_store);
}

template <typename T, typename S>
T Averager<T, S>::push(T entry)
{
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
    _position += 1;
    if (_position >= _size)
        _position = 0;
    return (_sum / _count);
}

template <typename T, typename S>
S Averager<T, S>::Sum() const
{
    return _sum;
}

template <typename T, typename S>
T Averager<T, S>::Average() const
{
    return (_sum / _count);
}

#endif