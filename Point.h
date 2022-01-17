#ifndef GRAHAM_SCAN_POINT_H
#define GRAHAM_SCAN_POINT_H

class Point
{
private:
    int index;
    float x, y;
public:
    Point() = default;
    Point(int _index, float _x, float _y);
    int get_index() const { return index; }
    float get_x() const { return x; }
    float get_y() const { return y; }
    Point operator-(Point rhs) const;
    Point operator+(Point rhs) const;
};

Point::Point(int _index, float _x, float _y) : index(_index), x(_x), y(_y) {}

Point Point::operator-(Point rhs) const
{
    return Point{index, x - rhs.x, y - rhs.y};
}

Point Point::operator+(Point rhs) const
{
    return Point{index, x + rhs.x, y + rhs.y};
}

#include "Dynamic_array.h"
#include <fstream>
#include <stdexcept>

Dynamic_array<Point> get_points_from_file(const std::string & _file_name)
{
    std::ifstream input(_file_name);
    if (!input)
        throw std::runtime_error("Unable to open file: " + _file_name);
    int n;
    float x, y;
    input >> n;
    Dynamic_array<Point> pointArray(n);
    for (int i{}; i < n; ++i)
    {
        input >> x >> y;
        pointArray.push_back(Point{i, x, y});
    }
    input.close();
    return pointArray;
}

#endif //GRAHAM_SCAN_POINT_H
