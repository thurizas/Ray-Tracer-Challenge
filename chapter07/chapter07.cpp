// chapter07.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

#include "world.h"
#include "light.h"
#include "sphere.h"
#include "transformations.h"
#include "camera.h"

#include <iostream>

void defaultWorldUnitTest();
void insideUnitTest();
void viewXformUnitTest();
void cameraUnitTests();
void mainTest();

int main()
{
    //defaultWorldUnitTest();
    //insideUnitTest();
    //viewXformUnitTest();
    //cameraUnitTests();

    mainTest();
}


void defaultWorldUnitTest()
{
    // features/world.feature : world creation test
    std::cout << "creating a default(empty) world: " << std::endl;
    world* pWorld = world::createWorld();
    std::cout << *pWorld << std::endl;

    // features/world.feature: the default world
    std::cout << "creating a light and two spheres, add to world: " << std::endl;
    light* pLight = new light(color(1, 1, 1), point(-10, 10, -10));
    material* pMat1 = new material();
    pMat1->c(color(0.8f, 1.0f, 0.6f));
    pMat1->d(0.7f);
    pMat1->sp(0.2f);
    material* pMat2 = new material();
    sphere* pS1 = new sphere(point(0, 0, 0), 1.0f, pMat1);
    sphere* pS2 = new sphere(point(0, 0, 0), 1.0f, pMat2);
    matrix4x4 xform1 = scale(0.5, 0.5, 0.5);
    pS2->setXform(xform1);
    pWorld->addLight(pLight);
    pWorld->addObject(pS1);
    pWorld->addObject(pS2);
    std::cout << "populated world is: " << std::endl;
    std::cout << *pWorld << std::endl;

    // features/world.featurs: intersect a world with a ray
    std::cout << "\n\nintersection a ray with the world" << std::endl;
    ray r1(point(0, 0, -5), vector(0, 0, 1));
    (void)pWorld->intersect(r1);
    std::vector< std::pair<int, float> >* pIntersections = pWorld->getIntersections();
    int cnt= pIntersections->size();
    std::cout << "number of intersections: " << cnt  << std::endl;
    for (int ndx = 0; ndx < cnt; ndx++)
    {
        std::cout << "x[" << ndx << "] is: " << (pIntersections->at(ndx)).second << std::endl;
    }

    // features/world.feature : shading an intersection
    std::cout << "shading an intersection" << std::endl;
    color pixelColor = pWorld->intersect(r1);
    std::cout << "shading an intersection, color (0.38066, 0.47583, 0.2855) is:" << pixelColor << std::endl;

    std::cout << "\n\nshading an intersection from the inside" << std::endl;
    pLight->p(point(0.0f, 0.25f, 0.0f));
    ray r2(point(0.0f, 0.0f, 0.0f), vector(0.0f, 0.0f, 1.0f));
    pixelColor = pWorld->intersect(r2);
    std::cout << "shading an intersection, color (0.90498, 0.90498, 0.90498) is:" << pixelColor << std::endl;

    // features/world.feature : the color when a ray misses
    std::cout << "\n\nray misses objects" << std::endl;
    pLight->p(point(-10, 10, -10));
    ray r3(point(0.0f, 0.0f, -5.0f), vector(0.0f, 1.0f, 0.0f));
    pixelColor = pWorld->intersect(r3);
    std::cout << "shading an intersection, color (0.0, 0.0, 0.25) is:" << pixelColor << std::endl;

    // features/world.feature : the color when a ray hits
    std::cout << "\n\nray his an object" << std::endl; 
    pixelColor = pWorld->intersect(r1);
    std::cout << " * shading an intersection, color (0.38066, 0.47583, 0.2855) is:" << pixelColor << std::endl;
 
    // features/world.feature : the color with an intersection behind the ray
    std::cout << "\n\nintersection behind the ray" << std::endl;
    ray r5(point(0.0f, 0.0f, 0.75f), vector(0.0f, 0.0f, -1.0f));
    pMat1->a(1);
    pMat2->a(1);
    pixelColor = pWorld->intersect(r5);
    std::cout << "shading an intersection, color (1.0, 1.0, 1.0) is:" << pixelColor << std::endl;
 
    world::deleteWorld();
}

void insideUnitTest()
{
    std::cout << "\n\nintersection on inside of shape" << std::endl;
    ray r2(point(0, 0, 0), vector(0, 0, 1));
    world* pWorld1 = world::createWorld();
    sphere* pS3 = new sphere(point(0, 0, 0), 1.0f);
    pWorld1->addObject(pS3);
    pWorld1->intersect(r2);
    world::deleteWorld();
}

void viewXformUnitTest()
{
    matrix4x4 viewXform = viewTransform(point(0, 0, 0), point(0, 0, -1), vector(0, 1, 0));
    std::cout << "transformation matrix for default orientation (should be normal):" << std::endl;
    std::cout << viewXform << std::endl;

    viewXform = viewTransform(point(0, 0, 0), point(0, 0, 1), vector(0, 1, 0));
    std::cout << "transformation matrix looking in +z direction (should be scale by (-1,1,-1)):" << std::endl;
    std::cout << viewXform << std::endl;

    viewXform = viewTransform(point(0, 0, 8), point(0, 0, 0), vector(0, 1, 0));
    std::cout << "transformation matrix for zooming in (should be translation(0,0,-8)" << std::endl;
    std::cout << viewXform << std::endl;

    viewXform = viewTransform(point(1, 3, 2), point(4, -2, 8), vector(1, 1, 0));
    std::cout << "transformation matrix for a arbitrary view transform:" << std::endl;
    std::cout << viewXform << std::endl;
}

void cameraUnitTests()
{
    std::cout << "construction a default camera:" << std::endl;
    camera c1;
    std::cout << "camera c1 is: " << c1 << std::endl;

    std::cout << "\nconstruction a paramertized camera:" << std::endl;
    camera c2(640, 480, PI / 2.0f, matrix4x4::identity());
    std::cout << "camera c2 is: " << c2 << std::endl;

    std::cout << "\ncalculation of pixel size: " << std::endl;
    camera c3(200, 125, PI / 2.0f, matrix4x4::identity());
    std::cout << "camera c3 is: " << c3 << std::endl;

    c3.hsize(125);
    c3.vsize(200);
    std::cout << "camera c3 is: " << c3 << std::endl;

    std::cout << "\ncalculation of ray through a point: " << std::endl;
    c2.hsize(201);
    c2.vsize(101);
    c2.fov(PI / 2.0f);
    ray r1 = c2.rayToPoint(100, 50);
    std::cout << "ray to point (100, 50) is: " << r1 << std::endl;

    r1 = c2.rayToPoint(0, 0);
    std::cout << "ray to point (0, 0) is: " << r1 << std::endl;

    matrix4x4 xform = rotation_y(45.0f) * translation(0, -2, 5);
    c2.t(xform);
    r1 = c2.rayToPoint(100, 50);
    std::cout << "ray to point (100, 50) is: " << r1 << std::endl;

    std::cout << "\nrender test:" << std::endl;
    std::cout << "creating a default(empty) world: " << std::endl;
    world* pDefWorld = world::createWorld();
    light* pLight = new light(color(1, 1, 1), point(-10, 10, -10));
    material* pMat1 = new material();
    pMat1->c(color(0.8f, 1.0f, 0.6f));
    pMat1->d(0.7f);
    pMat1->sp(0.2f);
    material* pMat2 = new material();
    sphere* pS1 = new sphere(point(0, 0, 0), 1.0f, pMat1);
    sphere* pS2 = new sphere(point(0, 0, 0), 1.0f, pMat2);
    matrix4x4 xform1 = scale(0.5, 0.5, 0.5);
    pS2->setXform(xform1);
    pDefWorld->addLight(pLight);
    pDefWorld->addObject(pS1);
    pDefWorld->addObject(pS2);

    camera  c(11, 11, PI / 2.0f, matrix4x4::identity());
    point from(0, 0, -5);
    point to(0, 0, 0);
    vector up(0, 1, 0);
    c.t(viewTransform(from, to, up));
    canvas image(c.hsize(), c.vsize());
    c.render(pDefWorld, &image);
    std::cout << "color at (5,5) expected to be (0.38066, 0.47583, 0.2855) is" << image.getColor(5, 5) << std::endl;
}

void mainTest()
{
    int   width = /*100*/640;
    int   height = /*50*/480;

    // define the floor
    sphere floor(point(0.0f,0.0f,0.0f));
    floor.setXform(scale(10, 0.1f, 10));
    material* pmat = floor.mat();
    pmat->c(color(1.0, 0.9f, 0.9f));
    pmat->sp(0.0f);

    // define the left wall
    sphere lwall(point(0.0f, 0.0f, 0.0f));
    lwall.setXform(translation(0, 0, 5)*rotation_y(-45)*rotation_x(90)*scale(10, 0.1f, 10));
    lwall.mat(floor.mat());

    // define the right wall
    sphere rwall(point(0.0f, 0.0f, 0.0f));
    rwall.setXform(translation(0, 0, 5)*rotation_y(45)*rotation_x(90)*scale(10, 0.1f, 10));
    rwall.mat(floor.mat());

    // define the middle sphere
    sphere middle(point(0.0f, 0.0f, 0.0f));
    middle.setXform(translation(-0.5f, 1.0, 0.5f));
    pmat = middle.mat();
    pmat->c(color(0.1f, 1, 0.5f));
    pmat->d(0.7f);
    pmat->sp(0.3f);

    // define the right sphere
    sphere right(point(0.0f, 0.0f, 0.0f));
    right.setXform(translation(1.5f, 0.5f, -0.5f)*scale(0.5f, 0.5f,0.5f));
    pmat = right.mat();
    pmat->c(color(0.5f, 1, 0.1f));
    pmat->d(0.7f);
    pmat->sp(0.3f);

    // define the left sphere
    sphere left(point(0.0f, 0.0f, 0.0f));
    left.setXform(translation(-1.5f, 0.45f, -0.75f)*scale(0.33f, 0.33f, 0.33f));
    pmat = left.mat();
    pmat->c(color(1.0f, 0.8f, 0.1f));
    pmat->d(0.7f);
    pmat->sp(0.3f);

    // define the light source...  
    light l(color(1, 1, 1), point(-10, 10, -10));

    // define the camera  camera(int, int, float, matrix4x4)
    camera c(width, height, PI / 3, matrix4x4::identity());
    c.t(viewTransform(point(0, 1.5, -5), point(0, 1, 0), vector(0, 1, 0)));

    // build da world...
    world* w = world::createWorld();
    w->addObject(&floor);
    w->addObject(&lwall);
    w->addObject(&rwall);
    w->addObject(&left);
    w->addObject(&middle);
    w->addObject(&right);
    w->addLight(&l);

    // build a canvas and render...
    canvas i(c.hsize(), c.vsize());
    c.render(w, &i);

    i.writePPM("./chapter07a.ppm");

}

/*






features/camera.feature   
 ​  ​
Then​ pixel_at(image, 5, 5) = color(0.38066, 0.47583, 0.2855)

*/
