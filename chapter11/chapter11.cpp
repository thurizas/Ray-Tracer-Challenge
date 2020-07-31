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
#include "transformations.h"
#include "camera.h"
#include "pattern.h"
#include <iostream>



void reflectionTests();
void refractionTests();
void mainTest(int, int);

int main(int argc, char** argv)
{
    int choice;
    int width = 640;
    int height = 400;

    while (-1 != (choice = getopt(argc, argv, "w:h:")))
    {
        switch (choice)
        {
        case 'w':
            width = atoi(optarg);
            break;

        case 'h':
            height = atoi(optarg);
            break;

        default:
            ;
        }
    }

    //reflectionTests();
    refractionTests();
    mainTest(width, height);
}

void reflectionTests()
{
    // features/material.features - reflectivity for the default material
    material m;           // default material
    std::cout << "default material is: \n" << m << std::endl;

    // features/intersections.feature - precomputation of reflection vector
    plane p1;
    interInfo  inter;
    ray r(point(0.0f, 1.0f, 1.0f), vector(0, -(float)sqrt(2) / 2.0f, (float)sqrt(2) / 2.0f));
    inter = r.intersect(&p1);
    std::cout << "reflected normal is [0, 0.707, 0.707]: " << inter << std::endl;

    // features/world.features - reflected color for a nonreflective material
    //world* pw = world::createWorld();                     // default world.
    //sphere s1(point(0, 0, 0));
    //s1.xform(scale(2, 2, 2));
    //material* pmat = s1.getMat();
    //pmat->a(1);
    //sphere s2(point(0, 0, 0));
    //s2.xform(scale(3, 3, 3));
    //pw->addObject(&s1);
    //pw->addObject(&s2);
    //ray r1(point(0.0f, 1.0f, 1.0f), vector(0, -(float)sqrt(2) / 2.0f, (float)sqrt(2) / 2.0f));
    //(void)pw->intersect(r1);
    //std::cout << "\nthe reflected color is (0,0,0) is: " << pw->reflectColor() << std::endl;
    //world::deleteWorld();

    // features/world.features - reflected color for a reflective material
    world* pw1 = world::createWorld();
    light l1(color(1.0f, 1.0f, 1.0f), point(-10.0f, 10.0f, -10.0f));

    sphere s3(point(0, 0, 0));
    material* pmat1 = s3.getMat();
    pmat1->c(color(0.38f, 0.476f, 0.246f));
    pmat1->a(1.0f);
    pmat1->d(0.7f);
    pmat1->sp(0.2f);

    sphere s4(point(0, 0, 0));
    s4.xform(scale(0.5f, 0.5f, 0.5f));

    plane pl;
    pmat1 = pl.getMat();
    pmat1->reflect(0.5f);
    pl.xform(translation(0.0f, -1.0f, 0.0f));

    pw1->addLight(&l1);
    pw1->addObject(&s3);
    pw1->addObject(&s4);
    pw1->addObject(&pl);

    ray r2(point(0, 0, -3.0f), vector(0, -(float)sqrt(2) / 2.0f, (float)sqrt(2) / 2.0f));
    color pixelColor = pw1->intersect(r2);
    std::cout << "\nthe reflected color is (0.87677, 0.92436, 0.82918) is: " << pixelColor << std::endl;
    world::deleteWorld();
}

void refractionTests()
{

}




void mainTest(int width, int height)
{
    std::cout << "creating image " << width << "x" << height << std::endl;

    // define a pattern
    checkeredPattern pat1(WHITE, BLACK);
    pat1.xform(scale(2, 1, 2));


    // define the floor
    plane pl;
    material* pmat = pl.getMat();
    pmat->c(color(0.5019f, 0.2510f, 0.2510f));
    pmat->d(0.7f);
    pmat->sp(0.3f);
    pmat->reflect(0.5f);
    pmat->p(&pat1);

    // define right - backdrop
    plane p2;
    p2.xform(translation(20, 0, 20) * rotation_y(45) * rotation_x(-90));
    material* pmat1 = p2.getMat();
    pmat1->c(color(0.5019f, 0.2510f, 0.2510f));
    pmat1->d(0.7f);
    pmat1->sp(0.3f);
    pmat1->p(&pat1);

    // define left-backdrop
    plane p3;
    p3.xform(translation(-20, 0, 20) * rotation_y(-45) * rotation_x(-90));
    material* pmat2 = p3.getMat();
    pmat2->c(color(0.5019f, 0.2510f, 0.2510f));
    pmat2->d(0.7f);
    pmat2->sp(0.3f);
    pmat2->p(&pat1);

    // define the middle sphere
    sphere middle(point(0.0f, 0.0f, 0.0f));
    middle.xform(translation(-0.5f, 1.0, 0.5f));
    pmat = middle.getMat();
    pmat->c(color(0.1f, 1, 0.5f));
    pmat->d(0.7f);
    pmat->sp(0.3f);

    // define the light source...  
    light l(color(1, 1, 1), point(-10, 10, -10));

    // define the camera  camera(int, int, float, matrix4x4)
    camera c(width, height, PI / 3, matrix4x4::identity());
    c.t(viewTransform(point(0, 1.5, -5), point(0, 1, 0), vector(0, 1, 0)));

    // build da world...
    world* w = world::createWorld();
    w->addObject(&pl);
    w->addObject(&p2);
    w->addObject(&p3);
    w->addObject(&middle);
    w->addLight(&l);

    // build a canvas and render...
    canvas i(c.hsize(), c.vsize());
    c.render(w, &i);

    i.writePPM("./chapter11.ppm");
}
