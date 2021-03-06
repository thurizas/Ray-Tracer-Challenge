// chapter05.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>

#include "canvas.h"
#include "object.h"
#include "ray.h"
#include "sphere.h"
#include "transformations.h"

void unitTest();
void mainTest();

int main()
{
    //unitTest();
    mainTest();
}

void unitTest()
{
    std::cout << "Creating a ray:" << std::endl;
    ray r1(point(1, 2, 3), vector(2, 5, 6));
    std::cout << r1 << std::endl;
    
    std::cout << "\nCreating spheres: " << std::endl;
    sphere s1(point(0, 0, 0), 1);
    sphere s2(point(3, 3, 3), 2);
    std::cout << s2 << std::endl;

    std::cout << "\ntransforming a sphere, translating to (2,3,4)" << std::endl;
    matrix4x4 xform = translation(2.0f, 3.0f, 4.0f);
    s1.setXform(xform);
    std::cout << s1 << std::endl;

    std::cout << "\nTransforming a ray, translating to (3,4,5)" << std::endl;
    ray ray2(point(1, 2, 3), vector(0, 1, 0));
    std::cout << ray2 << std::endl;
    ray2.xform(translation(3.0f, 4.0f, 5.0f));
    std::cout << "after translation:  " << ray2 << std::endl;

    std::cout << "\nTransforming a ray, scaling to (2,3,4)" << std::endl;
    ray ray3(point(1, 2, 3), vector(0, 1, 0));
    std::cout << ray3 << std::endl;
    ray3.xform(scale(2.0f, 3.0f, 4.0f));
    std::cout << "after translation:  " << ray3 << std::endl;

    std::cout << "\nPoints along a ray:" << std::endl;
    ray r2(point(2, 3, 4), vector(1, 0, 0));
    std::cout << "0 units along ray: " << r2.distance(0.0f) << std::endl;
    std::cout << "1 unit along ray: " << r2.distance(1.0f) << std::endl;
    std::cout << "-1 unit along ray: " << r2.distance(-1.0f) << std::endl;
    std::cout << "2.5 units along ray: " << r2.distance(2.5f) << std::endl;

    std::cout << "\nray-sphere intersection:" << std::endl;
    interInfo intInfo;
    sphere s10(point(0, 0, 0), 1);

    std::cout << "two intersection at 4, and 6" << std::endl;
    ray r3(point(0, 0, -5), vector(0, 0, 1));
    s10.intersect(r3, &intInfo);
    std::cout << const_cast<const interInfo&>(intInfo) << std::endl;

    std::cout << "\none tangent intersection at 5 " << std::endl;
    ray r4(point(0, 1, -5), vector(0, 0, 1));
    s10.intersect(r4, &intInfo);
    std::cout << const_cast<const interInfo&>(intInfo) << std::endl;

    std::cout << "\nray misses sphere " << std::endl;
    ray r5(point(0, 2, -5), vector(0, 0, 1));
    s10.intersect(r5, &intInfo);
    std::cout << const_cast<const interInfo&>(intInfo) << std::endl;

    std::cout << "\nray starts inside sphere" << std::endl;
    ray r6(point(0, 0, 0), vector(0, 0, 1));
    s10.intersect(r6, &intInfo);
    std::cout << const_cast<const interInfo&>(intInfo) << std::endl;

    std::cout << "\nray is behind sphere" << std::endl;
    ray r7(point(0, 0, 5), vector(0, 0, 1));
    s10.intersect(r7, &intInfo);
    std::cout << const_cast<const interInfo&>(intInfo) << std::endl;

    std::cout << "\nintersection with transformed spheres (3,7)" << std::endl;
    sphere s4(point(0, 0, 0), 1);
    ray r8(point(0, 0, -5), vector(0, 0, 1));
    s4.setXform(scale(2.0f, 2.0f, 2.0f));
    s4.intersect(r8, &intInfo);
    std::cout << const_cast<const interInfo&>(intInfo) << std::endl;

    std::cout << "\nintersection with transformed spheres (miss)" << std::endl;
    sphere s5(point(0, 0, 0), 1);
    ray r9(point(0, 0, -5), vector(0, 0, 1));
    s5.setXform(translation(5.0f, 0.0f, 0.0f));
    s5.intersect(r9, &intInfo);
    std::cout << const_cast<const interInfo&>(intInfo) << std::endl;
}


void mainTest()
{
    point ptRayOrigin(0.0f, 0.0f, -5.0f);
    float wallZ = 10.0f;
    float wallSize = 7.0f;
    int canvasSize = 400;
    float pixelSize = wallSize / (float)canvasSize;
    float halfWall = wallSize / 2;

    canvas  theCanvas(canvasSize, canvasSize);

    // define colors we are using...
    color   red(1, 0, 0);                     // solid red
    color   black(0, 0, 0);                   // black

    // intersection information..
    interInfo intInfo;

    // define the shapes we are using...
    sphere s1(point(0, 0, 0), 1.0);              // unit sphere at origin

    for (int y = 0; y < canvasSize - 1; y++)     // process a row
    {
        float worldY = halfWall - pixelSize * y;
        for (int x = 0; x < canvasSize - 1; x++)  // process pixels in row
        {
            float worldX = -halfWall + pixelSize * x;
            point target(worldX, worldY, wallZ);
            vector direction = target - ptRayOrigin;
            direction.normalize();
            ray r1(ptRayOrigin, direction);

            if (s1.intersect(r1, &intInfo))
                theCanvas.setColor(x, y, red);
            else
                theCanvas.setColor(x, y, black);

        }
    }

    theCanvas.writePPM("./chapter05.ppm");
}
 

