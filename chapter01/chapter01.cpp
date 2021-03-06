// chapter01.cpp : This file unit tests for point and vector manipulations
//

#include "pch.h"
#include "point.h"
#include "vector.h"
#include <iostream>

int main()
{
    // check point creation...
    std:: cout << "testing point creation (4.3, -4.2, 3.1)" << std::endl;
    point a = point(4.3f, -4.2f, 3.1f);
    std::cout << "x-component    " << a.x() << std::endl;
    std::cout << "y-component    " << a.y() << std::endl;
    std::cout << "z-component    " << a.z() << std::endl;
    std::cout << "a is a point:  " << a.isPoint() << std::endl;
    std::cout << "a is a vector: " << a.isVector() << std::endl;

    // check vector creation...

    std:: cout << "\ntesting vector creation (4.3, -4.2, 3.1)" << std::endl;
    vector v = vector(4.3f, -4.2f, 3.1f);
    std::cout << "x-component    " << v.x() << std::endl;
    std::cout << "y-component    " << v.y() << std::endl;
    std::cout << "z-component    " << v.z() << std::endl;
    std::cout << "a is a point:  " << v.isPoint() << std::endl;
    std::cout << "a is a vector: " << v.isVector() << std::endl; 

    std::cout << "\ntesting point (3,-2,5,1) + vector (-2, 3, 1, 0)" << std::endl;
    point  pt1 = point(3, -2, 5, 1);
    vector vec1 = vector(-2, 3, 1, 0);
    point pt2 = pt1 + vec1;  //point.operator+(vector)
    std::cout << "x-component: " << pt2.x() << std::endl;
    std::cout << "y-component: " << pt2.y() << std::endl;
    std::cout << "z-component: " << pt2.z() << std::endl;
    std::cout << "w-component: " << pt2.w() << std::endl;

    std::cout << "\ntesting point (3, 2, 1, 1) - point (5, 6, 7, 1)" << std::endl;
    point pt3 = point(3, 2, 1);
    point pt4 = point(5, 6, 7);
    vector vec2 = pt3 - pt4;
    std::cout << "x-component: " << vec2.x() << std::endl;
    std::cout << "y-component: " << vec2.y() << std::endl;
    std::cout << "z-component: " << vec2.z() << std::endl;
    std::cout << "w-component: " << vec2.w() << std::endl;

    std::cout << "\nsubtraction vector (5, 6, 7) from a point (3, 2, 1)" << std::endl;
    point pt5 = point(3, 2, 1);
    vector vec3 = vector(5, 6, 7);
    point pt6 = pt5 - vec3;
    std::cout << "x-component: " << pt6.x() << std::endl;
    std::cout << "y-component: " << pt6.y() << std::endl;
    std::cout << "z-component: " << pt6.z() << std::endl;
    std::cout << "w-component: " << pt6.w() << std::endl;

    std::cout << "\nsubtraction vector (5, 6, 7) from a vector (3, 2, 1)" << std::endl;
    vector vec4 = vector(3, 2, 1);
    vector vec5 = vec4 - vec3;
    std::cout << "x-component: " << vec5.x() << std::endl;
    std::cout << "y-component: " << vec5.y() << std::endl;
    std::cout << "z-component: " << vec5.z() << std::endl;
    std::cout << "w-component: " << vec5.w() << std::endl;

    std::cout << "\nnegation of a vector (3,2,1)" << std::endl;
    vector vec6 = -vec4;
    std::cout << "x-component: " << vec6.x() << std::endl;
    std::cout << "y-component: " << vec6.y() << std::endl;
    std::cout << "z-component: " << vec6.z() << std::endl;
    std::cout << "w-component: " << vec6.w() << std::endl;

    std::cout << "\nscaling a vector (3,2,1) by 3" << std::endl;
    vector vec7 = vec4 * 3.0f;
    std::cout << "x-component: " << vec7.x() << std::endl;
    std::cout << "y-component: " << vec7.y() << std::endl;
    std::cout << "z-component: " << vec7.z() << std::endl;
    std::cout << "w-component: " << vec7.w() << std::endl;

    std::cout << "reversing order of operands" << std::endl;
    vec7 = 3.0f * vec4;
    std::cout << "x-component: " << vec7.x() << std::endl;
    std::cout << "y-component: " << vec7.y() << std::endl;
    std::cout << "z-component: " << vec7.z() << std::endl;
    std::cout << "w-component: " << vec7.w() << std::endl;

    std::cout << "\n computing magnitude of vector (3, 2, 1)" << std::endl;
    std::cout << "norm-squared is " << vec4.norm2() << std::endl;
    std::cout << "magnitude: " << vec4.norm() << std::endl;

    std::cout << "\n normalizing a vector (1,2,3)" << std::endl;
    vec4.normalize();
    std::cout << "magnitude: " << vec4.norm() << std::endl;
    std::cout << "x-component: " << vec4.x() << std::endl;
    std::cout << "y-component: " << vec4.y() << std::endl;
    std::cout << "z-component: " << vec4.z() << std::endl;
    std::cout << "w-component: " << vec4.w() << std::endl;

    std::cout << "\ncalculation of dot product of (1,2,3) and (2,3,4)" << std::endl;
    vector vec8 = vector(1, 2, 3);
    vector vec9 = vector(2, 3, 4);
    float fdot = vec8.dot(vec9);
    std::cout << "x-component: " << fdot << std::endl;

    std::cout << "\ncalculation of cross product of (1,2,3) and (2,3,4)" << std::endl;
    vector vec11 = vec8.cross(vec9);
    std::cout << "x-component: " << vec11.x() << std::endl;
    std::cout << "y-component: " << vec11.y() << std::endl;
    std::cout << "z-component: " << vec11.z() << std::endl;
    std::cout << "w-component: " << vec11.w() << std::endl;

    std::cout << "reversing vector order" << std::endl;
    vec11 = vec9.cross(vec8);
    std::cout << "x-component: " << vec11.x() << std::endl;
    std::cout << "y-component: " << vec11.y() << std::endl;
    std::cout << "z-component: " << vec11.z() << std::endl;
    std::cout << "w-component: " << vec11.w() << std::endl;
}
