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


void patternTests();
void mainTest(int, int);

int main(int argc, char** argv)
{
    int choice;
    int width = 640;
    int height = 400;

    while (-1 != (choice = getopt(argc, argv, "w:h:")))
    {
        switch(choice)
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

    //patternTests();
    mainTest(width, height);
}

void patternTests()
{
    // features/patterns.features
    stripePattern pat1(WHITE, BLACK);
    std::cout << "stripe pattern is: ";
    pat1.print();

    // features/patterns.features - stripe pattern is constant in y
    color cl = pat1.stripeAt(point(0, 0, 0));
    std::cout << "color at point(0,0,0) is " << cl << std::endl;
    cl = pat1.stripeAt(point(0, 1, 0));
    std::cout << "color at point (0,1,0) is " << cl << std::endl;
    cl = pat1.stripeAt(point(0, 2, 0));
    std::cout << "color at point (0,2,0) is " << cl << std::endl;

    // features/patterns.features - stripe pattern is constant in z
    cl = pat1.stripeAt(point(0, 0, 1));
    std::cout << "color at point (0,0,1) is " << cl << std::endl;
    cl = pat1.stripeAt(point(0, 1, 0));
    std::cout << "color at point (0,0,2) is " << cl << std::endl;

    // features/patterns.features - stripe pattern alternates in x
    cl = pat1.stripeAt(point(0.9f, 0, 0));
    std::cout << "color at point (0.9, 0, 0) is white " << cl << std::endl;
    cl = pat1.stripeAt(point(1, 0, 0));
    std::cout << "color at point (1, 0, 0) is black " << cl << std::endl;
    cl = pat1.stripeAt(point(1.1f, 0, 0));
    std::cout << "color at point (1.1, 0, 0) is black " << cl << std::endl;
    cl = pat1.stripeAt(point(-0.9f, 0, 0));
    std::cout << "color at point (-0.9, 0, 0) is black " << cl << std::endl;
    cl = pat1.stripeAt(point(-1.0f, 0, 0));
    std::cout << "color at point (-1.0, 0, 0) is black " << cl << std::endl;
    cl = pat1.stripeAt(point(-1.1f, 0, 0));
    std::cout << "color at point (-1.1, 0, 0) is white " << cl << std::endl;

    // features/materials.feature - lighting with a pattern applied
    material mat1(color(0.5f, 0.5f, 0.5f), 1, 0, 0, 0, static_cast<pattern*>(&pat1));
    vector eyev(0, 0, -1);
    vector normalv(0, 0, -1);
    light lt(color(1, 1, 1), point(0, 0, -10));
    cl = lt.lighting(mat1, nullptr, point(0.9f, 0, 0), eyev, normalv, false);
    std::cout << "\n\ncolor at point (0.9,0,0) is white" << cl << std::endl;
    cl = lt.lighting(mat1, nullptr, point(1.1f, 0, 0), eyev, normalv, false);
    std::cout << "color at point (1.1, 0, 0) is black" << cl << std::endl;

    // features/patterns.features - pattern with an object transormation
    sphere s1(point(0, 0, 0), 1.0f);
    s1.xform(scale(2, 2, 2));
    material* mat = s1.getMat();
    mat->p(&pat1);
    cl = pat1.stripeAt(point(1.5, 0, 0), &s1);
    std::cout << "\n\ncolor at point (1.5, 0, 0) with object scaling(2,2,2) is white " << cl << std::endl;

    // features/patterns.features - pattern with an pattern transormation
    pat1.xform(scale(2, 2, 2));
    cl = pat1.stripeAt(point(1.5, 0, 0), nullptr);
    std::cout << "color at point (1.5.0.0) with patternscaling(2,2,2) is white " << cl << std::endl;

    // features/patterns.features - pattern with both object and pattern transformation
    cl = pat1.stripeAt(point(1.5, 0, 0), nullptr);
    std::cout << "color at point (1.5.0.0) with scaling(2,2,2) is white " << cl << std::endl;

    // features/patterns.feature - a gradient linear interpolates
    gradientPattern pat2(WHITE, BLACK);
    std::cout << "\n\ncolor at point (0,0,0) is (white) " << pat2.stripeAt(point(0, 0, 0), nullptr) << std::endl;
    std::cout << "color at point (0.25,0,0) is (0.75,0.75,0.75) " << pat2.stripeAt(point(0.25f, 0, 0), nullptr) << std::endl;
    std::cout << "color at point (0.50,0,0) is (0.50,0.50,0.50) " << pat2.stripeAt(point(0.50f, 0, 0), nullptr) << std::endl;
    std::cout << "color at point (0.75,0,0) is (0.25,0.25,0.25) " << pat2.stripeAt(point(0.75f, 0, 0), nullptr) << std::endl;

    //features/patterns.feature - a ring should extend in both x and z
    radialPattern pat3(WHITE, BLACK);
    std::cout << "\n\ncolor at point (0,0,0) is (white) " << pat3.stripeAt(point(0, 0, 0), nullptr) << std::endl;
    std::cout << "color at point (1,0,0) is (black) " << pat3.stripeAt(point(1, 0, 0), nullptr) << std::endl;
    std::cout << "color at point (0,0,1) is (black) " << pat3.stripeAt(point(0, 0, 1), nullptr) << std::endl;
    std::cout << "color at point (0.708f, 0, 0.785f) is (black) " << pat3.stripeAt(point(0.708f, 0, 0.708f), nullptr) << std::endl;

    // features/oatterns.feature - checkers should repeat in x
    checkeredPattern pat4(WHITE, BLACK);
    std::cout << "\n\ncolor at point(0,0,0) is (white) " << pat4.stripeAt(point(0, 0, 0)) << std::endl;
    std::cout << "color at point(0.99f,0,0) is (white) " << pat4.stripeAt(point(0.99f, 0, 0)) << std::endl;
    std::cout << "color at point(1.01f,0,0) is (black) " << pat4.stripeAt(point(1.01f, 0, 0)) << std::endl;

    // features/patterns.feature - checkers should repeat in y
    std::cout << "\ncolor at point(0,0.99f,0) is (white) " << pat4.stripeAt(point(0, 0.99f, 0)) << std::endl;
    std::cout << "color at point(0, 1.01f,0) is (black) " << pat4.stripeAt(point(0, 1.01f, 0)) << std::endl;

    // features/patterns.feature - checkers should repeat in z
    std::cout << "\ncolor at point(0,0,0.99) is (white) " << pat4.stripeAt(point(0, 0, 0.99f)) << std::endl;
    std::cout << "color at point(0,0, 1.01) is (black) " << pat4.stripeAt(point(0, 0, 1.01f)) << std::endl;
}




void mainTest(int width, int height)
{
    std::cout << "creating image " << width << "x" << height << std::endl;

    // define a pattern
    //stripePattern pat1(color(1, 0, 0), color(1, 1, 0.5));
    //gradientPattern pat1(color(1, 0, 0), color(0, 0, 1));
    //radialPattern pat1(WHITE, BLACK);
    checkeredPattern pat1(WHITE, BLACK);
    pat1.xform(scale(2, 1, 2));
    //pat1.xform(rotation_x(90));


    // define the floor
    plane pl;
    //pl.xform(scale(40, 1, 40));
    material* pmat = pl.getMat();
    pmat->c(color(0.5019f, 0.2510f, 0.2510f));
    pmat->d(0.7f);
    pmat->sp(0.3f);
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

    i.writePPM("./chapter10.ppm");

}


