// chapter04.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "matrix4x4.h"
#include "point.h"
#include "vector.h"
#include "transformations.h"

#include <iostream>

int main()
{
    std::cout << "Translation test: moving point(-3, 4,5) by (5,-3,2)" << std::endl;
    point  pt(-3, 4, 5);
    vector vec(-3, 4, 5);
    matrix4x4 xform = translation(5, -3, 2);
    std::cout << "moving a point:\n" << (xform * pt) << std::endl;
    std::cout << "moving a vector:\n" << xform * vec << std::endl;

    std::cout << "\nscaling test: scaling point(2,3,4)" << std::endl;
    point pt1(-4, 6, 8);
    point pt2(2, 3, 4);
    vector vec1(-4, 6, 8);
    matrix4x4 xform1 = scale(2, 3, 4);
    matrix4x4 xform2 = xform1.inverse();
    matrix4x4 xform3 = scale(-1, 1, 1);
    std::cout << "scaling a point:\n" << (xform1*pt1) << std::endl;
    std::cout << "scaling a vector:\n" << (xform1*vec1) << std::endl;
    std::cout << "scaling by inverse:\n" << (xform2*vec1) << std::endl;
    std::cout << "reflection about x-axis" << (xform3*pt2) << std::endl;

    std::cout << "\nRotation tests:\n" << std::endl;
    point pt3(0, 1, 0);
    point pt4(0, 0, 1);
    matrix4x4 xform4 = rotation_x(90.0f);
    matrix4x4 xform5 = xform4.inverse();
    matrix4x4 xform6 = rotation_y(45.0f);
    matrix4x4 xform7 = rotation_z(45.0f);
    std::cout << "full quarter about x-axis:" << xform4 * pt3 << std::endl;
    std::cout << "inverse of full quarter about x-axis: " << xform5 * pt3 << std::endl;
    std::cout << "half quarter about y-axis:" << xform6 * pt4 << std::endl;
    std::cout << "half quarter about z-axis:" << xform7 * pt3 << std::endl;

    std::cout << "\nShearing transforms" << std::endl;
    point pt5(2, 3, 4);
    matrix4x4 xform8 = shear(1, 0, 0, 0, 0, 0);
    std::cout << "shear is:" << xform8 * pt5 << std::endl;
    xform8 = shear(0, 1, 0, 0, 0, 0);
    std::cout << "shear is:" << xform8 * pt5 << std::endl;
    xform8 = shear(0, 0, 1, 0, 0, 0);
    std::cout << "shear is:" << xform8 * pt5 << std::endl;
    xform8 = shear(0, 0, 0, 1, 0, 0);
    std::cout << "shear is:" << xform8 * pt5 << std::endl;
    xform8 = shear(0, 0, 0, 0, 1, 0);
    std::cout << "shear is:" << xform8 * pt5 << std::endl;    
    xform8 = shear(0, 0, 0, 0, 0, 1);
    std::cout << "shear is:" << xform8 * pt5 << std::endl;

    std::cout << "chaining test:" << std::endl;
    point pt6(1, 0, 1);
    matrix4x4 xformA = rotation_x(90);
    matrix4x4 xformB = scale(5, 5, 5);
    matrix4x4 xformC = translation(10, 5, 7);
    std::cout << "applying rotation: " << (pt = xformA * pt6) << std::endl;
    std::cout << "applying scaling: " << (pt = xformB * pt) << std::endl;
    std::cout << "applying translation: " << (pt = xformC * pt) << std::endl;
    matrix4x4 xformT = xformC * xformB* xformA;
    std::cout << "applying composite transformation: " << xformT * pt6 << std::endl;

}
