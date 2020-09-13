#ifndef PCH_H
#define PCH_H

// to undefine the minor macro being pulled in from sys/sysmacros.h
#undef minor


const float EPSILON = 0.00001f;
const float PI = 3.1415926f;
const float DEG2RAD = 2.0f*PI / 360.0f;     // 360 deg = 2\pi radians => 1 deg = 2\pi/360


#include <iostream>
#include <string>
#include <cmath>



#endif //PCH_H
