#pragma once
#include "Curve.hpp"
class Bezier :
    public Curve
{
public:
    Bezier();
    void generateCurve(int pointsPerSegment);
};

