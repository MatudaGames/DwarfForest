//
//  Utils.h
//  DwarfForest
//
//  Created by Elviss Strazdins on 24.02.2013.
//
//

#pragma once

#include <cmath>
#include <string>

const double TWO_PI = 2 * 3.14159265358979323846264338327950288; // 2 * pi


void vibrate();
std::string toString(int i);
std::string toString(unsigned int i);

namespace Time
{
    //milliseconds
    unsigned long getTime();
    int dayDifference(time_t t1, time_t t2 = time(0));
    bool isToday(time_t day);
    uint32_t getYWeek();
    uint32_t getYDay();
}

std::string getSoundFx(const char* theName);

inline double mod(double x, double y)
{
    if (0 == y)
        return x;
	
    return x - y * floor(x/y);
}

// wrap [rad] angle to [-PI..PI)
inline double wrapPosNegPI(double fAng)
{
    return mod(fAng + M_PI, TWO_PI) - M_PI;
}

// wrap [rad] angle to [0..TWO_PI)
inline double wrapTwoPI(double fAng)
{
    return mod(fAng, TWO_PI);
}

// wrap [deg] angle to [-180..180)
inline double wrapPosNeg180(double fAng)
{
    return mod(fAng + 180., 360.) - 180.;
}

// wrap [deg] angle to [0..360)
inline double wrap360(double fAng)
{
    return mod(fAng ,360.);
}