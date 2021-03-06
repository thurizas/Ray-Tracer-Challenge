// chapter06.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "point.h"
#include "vector.h"
#include "sphere.h"
#include "light.h"
#include "canvas.h"

#include "transformations.h"
#include <iostream>

void unitTests();
void mainTest();

int main()
{
    //unitTests(); 
    mainTest();
}

void unitTests()
{
    std::cout << "normal of an axis: " << std::endl;
    sphere s1(point(0.0f, 0.0f, 0.0f), 1);
    vector n = s1.normalAt(point(1, 0, 0));
    std::cout << "   normal at (1,0,0) is: " << n << std::endl;

    n = s1.normalAt(point(0, 1, 0));
    std::cout << "   normal at (0, 1, 0) is: " << n << std::endl;

    n = s1.normalAt(point(0, 0, 1));
    std::cout << "   normal at (0, 0, 1) is: " << n << std::endl;

    std::cout << "\nnormal off axis: " << std::endl;
    float temp = sqrt(3.0f) / 3.0f;
    n = s1.normalAt(point(temp, temp, temp));
    std::cout << "   normal at (0.577, 0.577, 0.577) is: " << n << std::endl;
    std::cout << "   the length of the normal is: " << n.norm() << std::endl;

    std::cout << "\nnorm of a translated sphere:" << std::endl;
    sphere s2(point(0, 0, 0), 1);
    s2.setXform(translation(0, 1, 0));
    n = s2.normalAt(point(0.0f, 1.70711f, -0.70711f));
    std::cout << "normal at (0, 1.70711, -0.70711): " << n << std::endl;

    std::cout << "\nnormal on transformed sphere:" << std::endl;
    sphere s3(point(0.0f, 0.0f, 0.0f), 1);
    matrix4x4 m = scale(1.0f, 0.5f, 1.0f) * rotation_z(36.0f);
    s3.setXform(m);
    temp = sqrt(2.0f) / 2.0f;
    n = s3.normalAt(point(0, temp, -temp));
    std::cout << "normal at (0, 0.70711, -0.70711) is: " << n << std::endl;

    std::cout << "\nreflecting a vector about a normal" << std::endl;
    vector v1(1.0f, -1.0f, 0.0f);
    vector v2(0.0f, -1.0f, 0.0f);
    std::cout << "   reflection when angle of incidence is 45: " << v1.reflect(vector(0.0f, 1.0f, 0.0f)) << std::endl;
    std::cout << "   reflection of slanged surface: " << v2.reflect(vector(temp, temp, 0.0f)) << std::endl;

    light l1(color(1, 1, 1), point(0, 0, 0));
    std::cout << "lights intensity: " << l1.i() << std::endl;
    std::cout << "lights position: " << l1.p() << std::endl;

    std::cout << "\ntesting material class:" << std::endl;
    material m1;
    std::cout << "defalut material: " << m1 << std::endl;
    s1.mat(&m1);
    std::cout << "sphere 1 is: " << s1 << std::endl;

    std::cout << "\ntesting lighting function:" << std::endl;
    material m2;
    point    pt1(0, 0, 0);
    vector   eyev(0, 0, -1);
    vector   normalv(0, 0, -1);
    light    l2(color(1, 1, 1), point(0, 0, -10));
    color    res = l2.lighting(m2, pt1, eyev, normalv);
    std::cout << "eye between lignt and object shading is: " << res << std::endl;

    eyev = vector(0, temp, temp);
    res = l2.lighting(m2, pt1, eyev, normalv);
    std::cout << "eye between light and object, at 45 angle: " << res << std::endl;

    eyev = vector(0, 0, -1);
    l2 = light(color(1, 1, 1), point(0, 10, -10));
    res = l2.lighting(m2, pt1, eyev, normalv);
    std::cout << "light at 45 angle: " << res << std::endl;

    eyev = vector(0, -temp, -temp);
    res = l2.lighting(m2, pt1, eyev, normalv);
    std::cout << "eye in path of reflection vector: " << res << std::endl;

    eyev = vector(0, 0, -1);
    l2 = light(color(1.0f, 1.0f, 1.0f), point(0.0f, 0.0f, 10.0f));
    res = l2.lighting(m2, pt1, eyev, normalv);
    std::cout << "eye behind object: " << res << std::endl;
}

void mainTest()
{
    point ptRayOrigin(0.0f, 0.0f, -5.0f);
    float wallZ = 10.0f;
    float wallSize = 7.0f;
    int canvasSize = 400; /* 100 */;
    float pixelSize = wallSize / (float)canvasSize;
    float halfWall = wallSize / 2;

    canvas  theCanvas(canvasSize, canvasSize);

    // define colors we are using...
    color   black(0, 0, 0);                   // black

    // intersection information..
    interInfo intInfo;

    // define the shapes we are using...
    sphere s1(point(0, 0, 0), 1.0);              // unit sphere at origin
    material m1;                                 // default material
    m1.c(color(1.0f, 0.2f, 1.0f));               // set material color
    s1.mat(&m1);                                 // assign material to sphere

    // define the light source...
    light l1(color(1.0f, 1.0f, 1.0f), point(10.0f, 10.0f, -10.0f));

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
            vector d = r1.direction();
            d.normalize();
            r1.direction(d);

            if (s1.intersect(r1, &intInfo))
            {
                point pt = r1.distance(intInfo.t1());
                vector vecN = s1.normalAt(pt);
                vector eyeV = -r1.direction();
                color c1 = l1.lighting(*(s1.mat()), pt, eyeV, vecN);

                theCanvas.setColor(x, y, c1);
            }
                
            else
                theCanvas.setColor(x, y, black);

        }
    }
    std::cout << "wrote ppm file " << std::endl;
    theCanvas.writePPM("./chapter06.ppm");
}
