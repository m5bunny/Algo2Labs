#ifndef GRAHAM_SCAN_GRAHAM_SCAN_H
#define GRAHAM_SCAN_GRAHAM_SCAN_H

#include "Heap_sort.h"
#include "Dynamic_array.h"
#include "Point.h"

namespace graham
{
    bool comp(const Point & lhs, const Point & rhs)
    {
        return 0 < (lhs.get_y() * rhs.get_x() - rhs.get_y() * lhs.get_x());
    }

    Dynamic_array<Point> graham_scan(Dynamic_array<Point> _pointArray)
    {
        if (_pointArray.get_size() < 2)
            return _pointArray;

        Point lowestPoint{ _pointArray[0] };
        int lowestIndex = 0;
        for (int i{ 1 }; i < _pointArray.get_size(); ++i)
        {
            if ((lowestPoint.get_y() > _pointArray[i].get_y()) ||
                ((lowestPoint.get_y() == _pointArray[i].get_y()) && (lowestPoint.get_x() > _pointArray[i].get_x())))
            {
                lowestPoint = _pointArray[i];
                lowestIndex = i;
            }
        }
       _pointArray.remove_element(lowestIndex);

        for (int i{}; i < _pointArray.get_size(); ++i)
            _pointArray[i] = _pointArray[i] - lowestPoint;

        heap_sort(_pointArray.get_array_without_remove(), _pointArray.get_size(), true, comp);

        Dynamic_array<Point> ch;
        ch.push_back(lowestPoint);
        ch.push_back(_pointArray[0] + lowestPoint);
        for (int i{ 1 }; i < _pointArray.get_size(); ++i)
        {
            ch.push_back(_pointArray[i] + lowestPoint);
            while (ch.get_size() > 2 && comp(ch[ch.get_size() - 2] - ch[ch.get_size() - 3], ch[ch.get_size() - 1] - ch[ch.get_size() - 2]))
                ch.remove_element(ch.get_size() - 2);
        }
        return ch;
    }
}

#endif //GRAHAM_SCAN_GRAHAM_SCAN_H
