// chapter12.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

#ifdef __WIN32
#include "xgetopt.h"
#else
#include <unistd.h>
#endif


#include "world.h"
#include "light.h"
#include "color.h"
#include "sphere.h"
#include "plane.h"
#include "cube.h"
#include "transformations.h"
#include "camera.h"
#include "pattern.h"
#include "util.h"
#include <iostream>

void cubeTest0(bool flag = false);                             // testing creation and printing of cube
void cubeTest1(bool flag = false);                             // testing cube/ray intersection -- ray hitting cube
void cubeTest2(bool flag = false);                             // testing cube/ray intersection -- ray missing cube
void cubeTest3(bool flag = false);                             // finding the normal
void cubeMain(int, int, bool flag = false);                    // main test

int main(int argc, char** argv)
{
    int choice;
    bool   bDebug=false;
    int width = 640;
    int height = 400;

    while (-1 != (choice = getopt(argc, argv, "w:h:d")))
    {
        switch (choice)
        {
        case 'w':
            width = atoi(optarg);
            break;

        case 'h':
            height = atoi(optarg);
            break;

        case 'd':
            bDebug = true;

        default:
            ;
        }
    }

    //cubeTest0(bDebug);
    //cubeTest1(bDebug);
    //cubeTest2(bDebug);
    //cubeTest3(bDebug);
    cubeMain(width, height, bDebug);
}

// This test construction of an AABB and printing of a cube
void cubeTest0(bool bDebug)
{
    cube C1;

    std::cout << "\n*** Test0 **************************************************" << std::endl;
    std::cout << "The cube is: \n" << C1 << std::endl;
}

// This test ray-cube intersection on any face parallel to an axis
void cubeTest1(bool bDebug)
{
    cube       C1;
    interInfo  ID;                 // intersection infomation

    std::cout << "\n*** Test1 **************************************************" << std::endl;

    ray r1(point(5, 0.5, 0), vector(-1, 0, 0));        // t1 = 4, t2 = 6
    ray r2(point(-5, 0.5, 0), vector(1, 0, 0));        // t1 = 4, t2 = 6
    ray r3(point(0.5, 5, 0), vector(0, -1, 0));        // t1 = 4, t2 = 6
    ray r4(point(0.5, -5, 0), vector(0, 1, 0));        // t1 = 4, t2 = 6
    ray r5(point(0.5, 0, 5), vector(0, 0, -1));        // t1 = 4, t2 = 6
    ray r6(point(0.5, 0, -5), vector(0, 0, 1));        // t1 = 4, t2 = 6
    ray r7(point(0, 0.5, 0), vector(0, 0, 1));         // t1 = -1, t2 = 1

    C1.intersect(r1, &ID);
    std::cout << "intersection points: " << ID.m_nt1 << " and " << ID.m_nt2 << std::endl;
    C1.intersect(r2, &ID);
    std::cout << "intersection points: " << ID.m_nt1 << " and " << ID.m_nt2 << std::endl;
    C1.intersect(r3, &ID);
    std::cout << "intersection points: " << ID.m_nt1 << " and " << ID.m_nt2 << std::endl;
    C1.intersect(r4, &ID);
    std::cout << "intersection points: " << ID.m_nt1 << " and " << ID.m_nt2 << std::endl;
    C1.intersect(r5, &ID);
    std::cout << "intersection points: " << ID.m_nt1 << " and " << ID.m_nt2 << std::endl;
    C1.intersect(r6, &ID);
    std::cout << "intersection points: " << ID.m_nt1 << " and " << ID.m_nt2 << std::endl;
    C1.intersect(r7, &ID);
    std::cout << "intersection points: " << ID.m_nt1 << " and " << ID.m_nt2 << std::endl;
}

// This tests when ray misses cube
void cubeTest2(bool f)
{
    cube       C1;
    interInfo  ID;                 // intersection infomation

    std::cout << "\n*** Test2 **************************************************" << std::endl;

    ray r1(point(-2.0f, 0.0f, 0.0f), vector(0.2673f, 0.5345f, 0.8018f));
    ray r2(point(0.0f, -2.0f, 0.0f), vector(0.8018f, 0.2673f, 0.5345f));
    ray r3(point(0.0f, 0.0f, -2.0f), vector(0.5345f, 0.8018f, 0.2673f));
    ray r4(point(2.0f, 0.0f, 2.0f), vector(0.0f, 0.0f, -1.0f));
    ray r5(point(0.0f, 2.0f, 2.0f), vector(0.0f, -1.0f, 0.0f));
    ray r6(point(2.0f, 2.0f, 0.0f), vector(-1.0f, 0.0f, 0.0f));

    if (C1.intersect(r1, &ID)) std::cout << "intersection" << std::endl; else std::cout << "miss" << std::endl;
    if (C1.intersect(r2, &ID)) std::cout << "intersection" << std::endl; else std::cout << "miss" << std::endl;
    if (C1.intersect(r3, &ID)) std::cout << "intersection" << std::endl; else std::cout << "miss" << std::endl;
    if (C1.intersect(r4, &ID)) std::cout << "intersection" << std::endl; else std::cout << "miss" << std::endl;
    if (C1.intersect(r5, &ID)) std::cout << "intersection" << std::endl; else std::cout << "miss" << std::endl;
    if (C1.intersect(r6, &ID)) std::cout << "intersection" << std::endl; else std::cout << "miss" << std::endl;
}

void cubeTest3(bool f)
{
    cube       C1;

    std::cout << "\n*** Test3 **************************************************" << std::endl;

    point pt1(1.0f, 0.5f, -0.8f);
    std::cout << "local normal is [1, 0, 0] " << C1.localNormalAt(pt1) << std::endl;
    point pt2(-1.0f, -0.2f, 0.9f);
    std::cout << "local normal is [-1, 0, 0] " << C1.localNormalAt(pt2) << std::endl;
    point pt3(-0.4f, 1.0f, -0.1f);
    std::cout << "local normal is [0, 1, 0] " << C1.localNormalAt(pt3) << std::endl;
    point pt4(0.3f, -1.0f, -0.7f);
    std::cout << "local normal is [0, -1, 0] " << C1.localNormalAt(pt4) << std::endl;
    point pt5(-0.6f, 0.3f, 1.0f);
    std::cout << "local normal is [0, 0, 1] " << C1.localNormalAt(pt5) << std::endl;
    point pt6(0.4f, 0.4f, -1.0f);
    std::cout << "local normal is [0, 0, -1] " << C1.localNormalAt(pt6) << std::endl;
    point pt7(1.0f, 1.0f, 1.0f);
    std::cout << "local normal is (1, 0, 0)" << C1.localNormalAt(pt1) << std::endl;
}

void cubeMain(int width, int height, bool f)
{
    std::cout << "creating image " << width << "x" << height << std::endl;

    // define a pattern
    checkeredPattern pat1(WHITE, BLACK);
    pat1.xform(scale(2, 1, 2));

    // define the floor
    plane pl;;
    pl.getMat()->d(0.7f);
    pl.getMat()->sp(0.3f);
    pl.getMat()->reflect(0.5f);
    pl.getMat()->p(&pat1);

    // define right - backdrop
    plane p2;
    p2.xform(translation(20, 0, 20) * rotation_y(45) * rotation_x(-90));
    material* pmat1 = p2.getMat();
    pmat1->c(color(176.0f / 255.0f, 232.0f / 255.0f, 236.0f / 255.0f));
    pmat1->d(0.7f);
    pmat1->sp(0.3f);

    // define left-backdrop
    plane p3;
    p3.xform(translation(-20, 0, 20) * rotation_y(-45) * rotation_x(-90));
    material* pmat2 = p3.getMat();
    pmat2->c(color(176.0f / 255.0f, 232.0f / 255.0f, 236.0f / 255.0f));
    pmat2->d(0.7f);
    pmat2->sp(0.3f);

    // red cube
    cube c1;
    c1.xform(translation(0.0f, 1.0f, 35.0f)*rotation_y(-45));
    c1.getMat()->c(color(1.0f, 0.0f, 0.0f));
    c1.getMat()->d(0.7f);
    c1.getMat()->sp(0.3f);

    // table top
    cube c2;
    //c2.xform(translation(-10.0f, 0.5f, 10.0f)*scale(2.5f, 0.1f, 1.0f)*rotation_y(-45));
    c2.xform(translation(1.0f, 0.5f, 1.0f)*scale(2.5f, 0.1f, 1.0f));
    c2.getMat()->c(color(185.0f/255.0f, 115.0f/255.0f, 115.0f/255.0f));
    c2.getMat()->d(0.7f);
    c2.getMat()->sp(0.3f);
    c2.getMat()->sh(50);

    // back left leg
    cube c3;
    //c3.xform(translation(-8.8f, 0.125f, 9.6f)*scale(0.05f, 0.25f, 0.05f)*rotation_y(-45));
    c3.xform(translation(-1.25f, 0.125f, 0.45f)*scale(0.05f, 0.25f, 0.05f));
    c3.getMat()->c(color(185.0f / 255.0f, 115.0f / 255.0f, 115.0f / 255.0f));
    c3.getMat()->d(0.7f);
    c3.getMat()->sp(0.3f);
    c3.getMat()->sh(50);

    // back right leg
    cube c4;
    //c4.xform(translation(-8.8f, 0.125f, 10.8f)*scale(0.05f, 0.25f, 0.05f)*rotation_y(-45));
    c4.xform(translation(1.25f, 0.125f, 0.45f)*scale(0.05f, 0.25f, 0.05f));
    c4.getMat()->c(color(185.0f / 255.0f, 115.0f / 255.0f, 115.0f / 255.0f));
    c4.getMat()->d(0.7f);
    c4.getMat()->sp(0.3f);
    c4.getMat()->sh(50);

    // front left leg
    cube c5;
    //c5.xform(translation(-11.2f, 0.125f, 9.6f)*scale(0.05f, 0.25f, 0.05f)*rotation_y(-45));
    c5.xform(translation(-1.25f, 0.125f, -0.45f)*scale(0.05f, 0.25f, 0.05f));
    c5.getMat()->c(color(185.0f / 255.0f, 115.0f / 255.0f, 115.0f / 255.0f));
    c5.getMat()->d(0.7f);
    c5.getMat()->sp(0.3f);
    c5.getMat()->sh(50);

    // front right leg
    cube c6;
    //c6.xform(translation(-11.2f, 0.125f, 10.4f)*scale(0.05f, 0.25f, 0.05f)*rotation_y(-45));
    c6.xform(translation(1.25f, 0.125f, -0.45f)*scale(0.05f, 0.25f, 0.05f));
    c6.getMat()->c(color(185.0f / 255.0f, 115.0f / 255.0f, 115.0f / 255.0f));
    c6.getMat()->d(0.7f);
    c6.getMat()->sp(0.3f);
    c6.getMat()->sh(50);

    // light green sphere
    sphere sp1(point(0.0f, 0.0f, 0.0f));
    sp1.xform(translation(5.0f, 1.0, 5.0f));
    sp1.getMat()->c(color(0.1f, 1, 0.5f));
    sp1.getMat()->a(0.9f);
    sp1.getMat()->d(0.7f);
    sp1.getMat()->sp(0.3f);

    // blue sphere
    sphere sp2(point(0.0f, 0.0f, 0.0f));
    sp2.xform(translation(1.0f, 1.5, 1.0f));
    sp2.getMat()->c(color(0.1f, 0.5f, 1.0f));
    sp2.getMat()->d(0.7f);
    sp2.getMat()->sp(0.3f);

    // define the light source...  
    light l(color(1, 1, 1), point(-10.0f, 10.0f, -10.0));

    // define the camera  camera(int, int, float, matrix4x4)
    camera c(width, height, PI/3.0f, matrix4x4::identity());
    // move the camera - viewTransform(from, to, up)
    c.t(viewTransform(point(-0.0f, 1.5f, -10.0f), point(0.0f, 1.0f, 0.0f), vector(0.0f, 1.0f, 0.0f)));
    //c.t(viewTransform(point(-3.0f, 1.5f, -3.0f), point(-10.0f, 1.0f, 20.0f), vector(0.0f, 1.0f, 0.0f)));

    // build da world...
    world* w = world::createWorld();
    w->addObject(&pl);
    w->addObject(&p2);
    w->addObject(&p3);
    w->addObject(&c1);     // red cube
    w->addObject(&c2);     // table top
    w->addObject(&c3);     // leg
    w->addObject(&c4);     // leg
    w->addObject(&c3);     // leg
    w->addObject(&c4);     // leg
    w->addObject(&sp1);
    w->addObject(&sp2);
    //w->addObject(&sp3);
    w->addLight(&l);

    // build a canvas and render...
    canvas i(c.hsize(), c.vsize());    
    
    // render the image
    c.render(w, &i);

    i.writePPM("./chapter12a.ppm");

    world::deleteWorld();
}
