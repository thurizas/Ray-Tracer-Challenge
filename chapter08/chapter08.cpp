// chapter08.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

#include "world.h"
#include "light.h"
#include "sphere.h"
#include "transformations.h"
#include "camera.h"

#include <iostream>

void lightingUnitTests();
void shadowUnitTests();
void shadowedIntersectTests();
void mainTest();

int main()
{
    //lightingUnitTests();
    //shadowUnitTests();
    //shadowedIntersectTests();
    mainTest();
}

// features/material.feature - lighting with surface in shadow
void lightingUnitTests()
{
    material m2;
    point    pt1(0, 0, 0);
    vector   eyev(0, 0, -1);
    vector   normalv(0, 0, -1);
    light    l2(color(1, 1, 1), point(0, 0, -10));
    color    res = l2.lighting(m2, pt1, eyev, normalv, true);
    std::cout << "the color is: (0.1, 0.1, 0.1)" << res << std::endl;
}

void shadowUnitTests()
{
    world* pWorld = world::createWorld();
    material mat1;
    mat1.c(color(0.8f, 1.0f, 0.6f));
    mat1.d(0.7f);
    mat1.sp(0.2f);
    material mat2;

    sphere s1(point(0, 0, 0));
    s1.mat(&mat1);
    sphere s2(point(0, 0, 0));
    s2.mat(&mat2);
    s2.setXform(scale(0.5f, 0.5f, 0.5f));

    light lt(color(1.0, 1.0, 1.0), point(-10, 10, -10));
    pWorld->addObject(&s1);
    pWorld->addObject(&s2);
    pWorld->addLight(&lt);

    // features/world.feature : no shadow when nothing is colinear with point & object
    point p1(0, 10, 0);
    std::cout << "nothing between light and point: is shadowed(false): " << pWorld->isShadowed(p1, &lt) << std::endl;
    
    // features/world.feature : shadow when object between light and point
    point p2(10, -10, 10);
    std::cout << "object between light and point:is shadowed (true): " << pWorld->isShadowed(p2, &lt) << std::endl;
    
    // features/world.feature : no shadow when object is behind light
    point p3(-20, 20, -20);
    std::cout << "object behind light: is shadowed (false): " << pWorld->isShadowed(p3, &lt) << std::endl;
    
    // features/world.feature : no shadow when object is behind point
    point p4(-2, 2, -2);
    std::cout << "object behind point: is shadowed (false): " << pWorld->isShadowed(p4, &lt) << std::endl;

}


void shadowedIntersectTests()
{

    world* pWorld1 = world::createWorld();
    light lt1(color(1.0, 1.0, 1.0), point(0, 0, -10));
    sphere s3(point(0, 0, 0));
    sphere s4(point(0, 0, 0));
    s3.setXform(translation(0, 0, 10.0f));
    pWorld1->addObject(&s3);
    pWorld1->addObject(&s4);
    pWorld1->addLight(&lt1);

    // features / world.feature ​- shade_hit() is given an intersection in shadow
    ray r1(point(0, 0, 5), vector(0, 0, 1));
    color pixelColor = pWorld1->intersect(r1);
    std::cout << "intersection in shadow is (0.1, 0.1, 0.1)" << pixelColor << std::endl;

}


void mainTest()
{
    int   width = /*100*/640;
    int   height = /*50*/480;

    // define the floor
    sphere floor(point(0.0f, 0.0f, 0.0f));
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
    right.setXform(translation(1.5f, 0.5f, -0.5f)*scale(0.5f, 0.5f, 0.5f));
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

    i.writePPM("./chapter08.ppm");

}
