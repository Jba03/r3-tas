#ifndef route_h
#define route_h

#include <vector>
#include "stVector3D.hh"
#include "stVector4D.hh"

struct route
{
    std::string name;
    std::vector<stVector3D> points;
    stVector4D color;
    
    route(std::string name) : name(name) {}
    ~route() { points.clear(); }
    
    void push(stVector3D point)
    {
        points.push_back(point);
    }
};

#endif /* route_h */
