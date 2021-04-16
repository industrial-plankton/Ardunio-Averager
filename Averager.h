/*
*  Copyright (C) 2020 Industrial Plankton IndustrialPlankton.com
*  
*  This program is free software: you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

template <typename T, typename S>
class Averager
{
private:
    T *_store;
    S _sum;            // _sum variable for faster mean calculation
    uint8_t _position; // _position variable for circular buffer
    uint8_t _count;
    uint8_t _size;

public:
    Averager(uint32_t size);
    T push(T entry);
};

template <typename T, typename S>
Averager<T, S>::Averager(uint32_t size)
{
    _size = size;
    _count = 0;
    _store = (T *)malloc(sizeof(T) * size);
    _position = 0;
    _sum = 0;
    for (uint32_t i = 0; i < size; i++)
    {
        _store[i] = 0;
    }
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