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
#include "util.h"
#include <iostream>




void reflectionTest1();             // material modification
void reflectionTest2();             // compute the reflectv vector
void reflectionTest3();             // color striking a non-reflective surface
void reflectionTest4();             // color striking a reflective surface
void reflectionTest5();             // update shadeHit cal'cs
void reflectionTest6();             // testing for unbounded recursion
void reflectionTest7();             // bounding reflection recursion

void refractionTest1();             // material modification and def. of glass sphere
void refractionTest2();             // n1 and n2 at intersection points
void refractionTest3();             // calculation of under-point
void refractionTest4();             // color for refracted ray with opaque material
void refractionTest5();             // color at limit of recursion
void refractionTest6();             // test for internal reflection      
void refractionTest7();             // refracted color with refracted ray
void refractionTest8();             // shadeHit cal'cs correct color

void fresnelTest1();                // reflectance under total internal reflectance
void fresnelTest2();                // ray hits surface at 90 degree angle
void fresnelTest3();                // n2 > n1 and angle is small
void fresnetTest4();                // shadeHit cal'cs correct

void test();

void reflectionTest(int, int);
void refractionTest(int, int);

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

    //reflectionTest1();                            //1. material modification
    //reflectionTest2();                            //2. compute the reflectv vector
    //reflectionTest3();                            //3. color striking a non-reflective surface
    //reflectionTest4();                            //4. color striking a reflective surface - test5 will casue this to fail
    //reflectionTest5();                            //5. update shadeHit cal'cs
    //reflectionTest6();                            //6. avoid infinite recursion
    //reflectionTest7();                            //7. limiting depth of recursion

    //refractionTest1();                            // material modification and def. of glass sphere
    //refractionTest2();                            // n1 and n2 at intersection points
    //refractionTest3();                            // calculation of under-point
    //refractionTest4();                            // color for refracted ray with opaque material
    //refractionTest5();                            // color at limit of recurrsion
    //refractionTest6();                            // test for internal reflection      
    //refractionTest7(); //TODO: check              // refracted color with refracted ray
    //refractionTest8();                            // shadeHit cal'cs correct color

    test();

    //fresnelTest1();                                 // reflectance under total internal reflectance
    //fresnelTest2();                                 // ray hits surface at 90 degree angle
    //fresnelTest3();                                 // n2 > n1 and angle is small
    //fresnetTest4();                                 // shadeHit cal'cs correct

    //reflectionTest(width, height);                // main reflection test
    //refractionTest(width, height);                // main refraction test
}

void reflectionTest1()
{
    // features/material.features - reflectivity for the default material
    material m;                                                          // default material
    std::cout << "*** test #1 :  ************************" << std::endl;
    std::cout << "default material is: \n" << m << std::endl;
}

void reflectionTest2()
{
    // features/intersections.feature - precomputation of reflection vector
    world* pW = world::createWorld();
    
    plane p1;
    pW->addObject(&p1);

    ray r(point(0.0f, 1.0f, -1.0f), vector(0, -(float)sqrt(2) / 2.0f, (float)sqrt(2) / 2.0f));
    pW->intersect(r);
    std::vector<std::pair<int, float>> inters = pW->getIntersections();
    std::cout << "\n\n*** test #2 :  ************************" << std::endl;
    std::cout << "The intersections are: " << std::endl;
    for (auto inter : inters)
    {
        std::cout << inter.second << ":" << inter.first << std::endl;
    }
    pIntDataT pID = pW->getIntData();
    std::cout << "reflected normal is [0, 0.707, 0.707]: " << pID->reflectv << std::endl;

    world::deleteWorld();
}

void reflectionTest3()
{
    world* pW = defaultWorld();
    pW->getObject(1)->getMat()->a(1.0);
    ray r(point(0, 0, 0), vector(0, 0, 1));
    pW->intersect(r,3);                       // use a specific intersection for test
    
    std::cout << "\n*** test #3 :  ************************" << std::endl;
    std::cout << "The intersections are:" << std::endl;
    std::vector<std::pair<int, float>> inters = pW->getIntersections();
    for (auto inter : inters)
    {
        std::cout << inter.second << ":" << inter.first << std::endl;
    }
    pIntDataT pID = pW->getIntData();
    std::cout << *pID << std::endl;
    
    std::cout << "the reflected color is (0, 0, 0): " << pW->reflectColor(pID, 0) << std::endl;

    world::deleteWorld();
}

// N.B.  This test will fail once reflectionTest5 is working
//       shadeHit now calls refectColor
void reflectionTest4()
{
    // features/world.features - reflected color for a reflective material
    world* pW = defaultWorld();

    plane pl;
    material* pmat1 = pl.getMat();
    pmat1->reflect(0.5f);
    pl.xform(translation(0.0f, -1.0f, 0.0f)); 
    
    pW->addObject(&pl);

    ray r2(point(0, 0, -3.0f), vector(0, -(float)sqrt(2) / 2.0f, (float)sqrt(2) / 2.0f));
    pW->intersect(r2);                             

    std::cout << "\n*** test #4 :  ************************" << std::endl;
    std::cout << "the world is: \n" << *pW << std::endl;
    std::cout << "the intersections are: " << std::endl;
    std::vector<std::pair<int, float>> inters = pW->getIntersections();
    for (auto inter : inters)
    {
        std::cout << inter.second << ":" << inter.first << ", " << std::endl;
    }
    pIntDataT pID = pW->getIntData();
    std::cout << *pID << std::endl;

    std::cout << "the reflected color is (0.19032, 0.2379, 0.14274) : " << pW->reflectColor(pID, 0) << std::endl;


    world::deleteWorld();
}

void reflectionTest5()
{
    world* pw1 = defaultWorld();
    
    plane pl;
    material* pmat1 = pl.getMat();
    pmat1->reflect(0.5f);
    pl.xform(translation(0.0f, -1.0f, 0.0f));

    pw1->addObject(&pl);

    ray r2(point(0, 0, -3.0f), vector(0, -(float)sqrt(2) / 2.0f, (float)sqrt(2) / 2.0f));
    color pixel = pw1->intersect(r2);

    std::cout << "\n*** test #5 :  ************************" << std::endl;;
    std::cout << "the intersections are: " << std::endl;;
    std::vector<std::pair<int, float>> inters = pw1->getIntersections();
    for (auto inter : inters)
    {
        std::cout << inter.second << ":" << inter.first << ", " << std::endl;;
    }
    std::cout << "the reflected color is (0.87677, 0.92436, 0.82918): " << pixel << std::endl;
    world::deleteWorld();
}

// N.B. This test should fail with stack overflow
//      Once test 7 is working, this should succeeded.
void reflectionTest6()
{
    world* pW = world::createWorld();

    light l1(color(1.0f, 1.0f, 1.0f), point(0, 0, 0));

    plane p1;
    p1.getMat()->reflect(1.0f);
    p1.xform(translation(0, -1, 0));

    plane p2;
    p2.getMat()->reflect(1.0f);
    p2.xform(translation(0, 1, 0));

    pW->addLight(&l1);
    pW->addObject(&p1);
    pW->addObject(&p2);

    ray r1(point(0, 0, 0), vector(0, 1, 0));
    color pixed = pW->intersect(r1);

    world::deleteWorld();
}

void reflectionTest7()
{
    world* pw1 = defaultWorld();

    plane pl;
    material* pmat1 = pl.getMat();
    pmat1->reflect(0.5f);
    pl.xform(translation(0.0f, -1.0f, 0.0f));

    pw1->addObject(&pl);

    ray r2(point(0, 0, -3.0f), vector(0, -(float)sqrt(2) / 2.0f, (float)sqrt(2) / 2.0f));
    pw1->intersect(r2);

    std::cout << "\n*** test #7 :  ************************" << std::endl;;
    std::cout << "the intersections are: " << std::endl;;
    std::vector<std::pair<int, float>> inters = pw1->getIntersections();
    for (auto inter : inters)
    {
        std::cout << inter.second << ":" << inter.first << ", " << std::endl;;
    }
    pIntDataT pID = pw1->getIntData();
    std::cout << "the reflected color is (0, 0, 0): " << pw1->reflectColor(pID, 0) << std::endl;
    world::deleteWorld();

}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// refraction unit tests....
void refractionTest1()
{
    std::cout << "\n*** test #1 ************************" << std::endl;
    // test #1 - features / material.features - trasparency and refractive index for the default material
    material mat;
    std::cout << "default material: " << mat << std::endl;

    //features / sphere.feature - producint a default glass sphere
    sphere* pS = glassSphere();
    std::cout << "\nglass sphere is: " << *pS << std::endl;
    delete pS;
}

void refractionTest2()
{
    // test #2 - features / intersections.features -- finding n_1 and n_2 at intersection points.
    world* w = world::createWorld();
    sphere* pS1 = glassSphere();
    pS1->xform(scale(2.0f, 2.0, 2.0f));
    pS1->getMat()->refractIndex(1.5f);

    sphere* pS2 = glassSphere();
    pS2->xform(translation(0.0f, 0.0f, -0.25f));
    pS2->getMat()->refractIndex(2.0f);

    sphere* pS3 = glassSphere();
    pS3->xform(translation(0.0f, 0.0f, 0.25f));
    pS3->getMat()->refractIndex(2.5f);

    w->addObject(pS1);
    w->addObject(pS2);
    w->addObject(pS3);

    std::cout << "\n*** test #2 ************************" << std::endl;

    ray r1(point(0.0f, 0.0f, -4.0f), vector(0.0f, 0.0f, 1.0f));
    std::cout << "intersections are at [2, 2.75, 3.25, 4.75, 5.25, 6]:" << std::endl;
    printf("%3s\t%3s\t%8s\t%8s\t%8s\n","ndx", "obj", "---t----", "---n1---", "---n2---");
    for (int ndx = 0; ndx < 6; ndx++)
    {
        w->intersect(r1, 5, ndx);
        pIntDataT pID = w->getIntData();
        if (nullptr != pID)
        {
            int objId = pID->pObject->getID();
            printf("%3d\t%3d\t%6.2f\t       %6.2f\t        %6.2f\n", ndx, objId, pID->t, pID->n1, pID->n2);
        }
    }

    delete pS1;
    delete pS2;
    delete pS3;
    world::deleteWorld();
}

void refractionTest3()
{
    // test #3 - feature/intersection.features - under point is offset below surface
    world* w1 = world::createWorld();

    sphere* pS4 = glassSphere();
    pS4->xform(translation(0, 0, 1));
    ray r2(point(0.0f, 0.0f, -5.0f), vector(0.0f, 0.0f, 1.0f));

    w1->addObject(pS4);

    std::cout << "\n*** test #3 ************************" << std::endl;

    w1->intersect(r2);
    std::cout << "intersections are at [5, 7]:";
    std::vector<std::pair<int, float>> ints = w1->getIntersections();
    for (auto inter : ints)
    {
        std::cout << inter.second << ":" << inter.first << ", ";
    }
    std::cout << std::endl;
    pIntDataT  pID = w1->getIntData();
    std::cout << "precomputed intersection data:\n" << *pID << std::endl;

    world::deleteWorld();
}

void refractionTest4()
{
    // test #4 - features/world.feature - the refracted color of an opaque surface is black
    world* w2 = defaultWorld();
    ray r3(point(0.0f, 0.0f, -5.0f), vector(0.0f, 0.0f, 1.0f));

    std::cout << "\n*** test #4 ************************" << std::endl;
    w2->intersect(r3);
    std::vector<std::pair<int, float>> inters = w2->getIntersections();
    std::cout << "intersections at (4:1, 4.5:2, 5.5:2, 6:1): \n";
    for (auto inter : inters)
    {
        std::cout << inter.second << ":" << inter.first << ", ";
    }
    pIntDataT pID = w2->getIntData();
    std::cout << "\nrefracted color is (0,0,0): " << w2->refractColor(pID, 5) << std::endl;

    world::deleteWorld();
}

void refractionTest5()
{
    // test #5 - features/world.feature - the refracted color at maximum recursive depth
    world* pW = defaultWorld();
    pW->getObject(1)->getMat()->transpar(1.0f);
    pW->getObject(1)->getMat()->refractIndex(1.5f);

    ray r(point(0.0f, 0.0f, -5.0f), vector(0.0f, 0.0f, 1.0f));
    pW->intersect(r);
    std::cout << "\n*** test #5 ************************" << std::endl;
    std::cout << "intersections at (4:1, 4.5:2, 5.5:2, 6:1):" << std::endl;
    std::vector<std::pair<int, float>> inters = pW->getIntersections();
    for (auto inter : inters)
    {
        std::cout << inter.second << ":" << inter.first << ", ";
    }
    pIntDataT pID = pW->getIntData();
    std::cout << "\nthe refracted color is (0,0,0): " << pW->refractColor(pID, 0) << std::endl;

    world::deleteWorld();
}

void refractionTest6()
{
    // test #6 - features/world.feature - the refracted color under total internal reflection
    world* pW = defaultWorld();
    pW->getObject(1)->getMat()->transpar(1.0f);
    pW->getObject(1)->getMat()->refractIndex(1.5f);

    ray r(point(0.0f, 0.0f, sqrt(2.0f) / 2.0f), vector(0.0f, 1.0f, 0.0f));
    pW->intersect(r);

    std::cout << "\n*** test #6 ************************" << std::endl;
    std::cout << "The intersections (-0.70711:1, 0.70711:1):" << std::endl;
    std::vector<std::pair<int, float>> inters = pW->getIntersections();
    for (auto inter : inters)
    {
        std::cout << inter.second << ":" << inter.first << ", ";
    }
    pIntDataT pID = pW->getIntData();
    std::cout << "\nthe refracted color is (0, 0, 0):" << pW->refractColor(pID, 5) << std::endl;
    
    world::deleteWorld();
}

// TODO : check defPattern is working correctly, check refracted direction and intersection point.
void refractionTest7()
{
    // test #7 - features/world.feature - the refracted color with a refracted ray
    world* pW = defaultWorld();
    pW->getObject(1)->getMat()->a(1.0f);
    pW->getObject(1)->getMat()->p(new defPattern());   
    pW->getObject(2)->getMat()->transpar(1.0);
    pW->getObject(2)->getMat()->refractIndex(1.5);
    ray r(point(0, 0, 0.1f), vector(0, 1.0f, 0));
    
    pW->intersect(r);

    std::cout << "\n*** test #7 ************************" << std::endl;
    std::cout << "the intersections are [-0.9950:0, -0.4899:1, 0.4899:1, 0.9950:0] : " << std::endl;
    std::vector<std::pair<int, float>> inters = pW->getIntersections();
    for (auto inter : inters)
    {
        std::cout << inter.second << ":" << inter.first << ", ";
    }
    pIntDataT pID = pW->getIntData();
    std::cout << "\nThe refracted color (0, 0.99888, 0.04725) is: " << pW->refractColor(pID, 5) << std::endl;

    world::deleteWorld();
}

void refractionTest8()
{
    // test #8 - features/world.feature - shade_hit() with a transparent material
    world* pW = defaultWorld();

    plane floor;
    floor.xform(translation(0.0, -1.0f, 0.0));
    floor.getMat()->transpar(0.5);
    floor.getMat()->refractIndex(1.5);

    sphere ball(point(0, 0, 0));
    ball.xform(translation(0.0f, -3.5f, -0.5f));
    ball.getMat()->c(color(1.0f, 0.0f, 0.0f));
    ball.getMat()->a(0.5);

    pW->addObject(&floor);
    pW->addObject(&ball);

    ray r(point(0.0f, 0.0f, -3.0f), vector(0.0f, -(float)sqrt(2) / 2.0f, (float)sqrt(2) / 2.0f));
    color cl = pW->intersect(r);

    std::cout << "\n*** test #8 ************************" << std::endl;
    std::cout << "intersections are (1.40411:3) : " << std::endl;
    std::vector < std::pair<int, float>> inters = pW->getIntersections();
    for (auto inter : inters)
    {
        std::cout << inter.second << ":" << inter.first << std::endl;
    }
    std::cout << "\n The final color is (0.93642, 0.68642, 0.68642): " << cl << std::endl;
}
// remember to uncomment lines in shadeHit
void test()
{
    world* pW = world::createWorld();

    sphere s1(point(0, 0, 0), 1.0);
    s1.getMat()->transpar(1.0f);
    s1.getMat()->refractIndex(1.1f);

    // define the backdrop
    plane pl;
    pl.xform(translation(20, 0, 20) *  rotation_x(-90));
    pl.getMat()->c(color(1.0f, 0.0f, 0.0f));
    pl.getMat()->a(1.0f);

    // define the light source...  
    light l(color(1, 1, 1), point(-10, 10, -10));


    pW->addObject(&s1);
    pW->addLight(&l);
    pW->addObject(&pl);

    ray r1(point(0, 1.0f/(float)sqrt(2.0), -3.0f), vector(0.0f, 0.0f, 1.0f));
    //ray r1(point(0, 0.5, -3.0f), vector(0.0f, 0.0f, 1.0f));
    //ray r1(point(0, (float)sqrt(3.0f) / 2.0f, -3.0f), vector(0.0f, 0.0f, 1.0f));
    //ray r1(point(0, 0, 3.0f), vector(0.0f, 0.0f, 1.0f));
    color pixel = pW->intersect(r1);

    pIntDataT pID = pW->getIntData();
    std::cout << "\n\n" << *pID << std::endl;
    std::cout << "\npixel color is: " << pixel << std::endl;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Fresnel tests
void fresnelTest1()
{
    world*  pW = world::createWorld();

    sphere* s1 = glassSphere();
    pW->addObject(s1);

    ray r(point(0.0f, 0.0f, (float)sqrt(2) / 2), vector(0.0f, 1.0f, 0.0f));

    pW->intersect(r);

    std::cout << "\n*** test #1 ************************" << std::endl;
    std::cout << "intersections are: " << std::endl;
    std::vector < std::pair<int, float>> inters = pW->getIntersections();
    for (auto inter : inters)
    {
        std::cout << inter.second << ":" << inter.first << std::endl;
    }

    pIntDataT pID = pW->getIntData();
    float     reflectance = pW->schlick(pID);
    std::cout << "\n The reflectance is: (1.0)" << reflectance << std::endl;
    std::cout << "\n*************************************" << std::endl;

    world::deleteWorld();
    delete s1;
}

void fresnelTest2()
{
    world* pW = world::createWorld();

    sphere* s1 = glassSphere();
    pW->addObject(s1);

    ray r(point(0.0f, 0.0f, 0.0f), vector(0.0f, 1.0f, 0.0f));

    pW->intersect(r);

    std::cout << "intersections are: " << std::endl;
    std::vector < std::pair<int, float>> inters = pW->getIntersections();
    for (auto inter : inters)
    {
        std::cout << inter.second << ":" << inter.first << std::endl;
    }

    pIntDataT pID = pW->getIntData();
    float     reflectance = pW->schlick(pID);
    std::cout << "\n The reflectance is (0.04): " << reflectance << std::endl;
    std::cout << "\n*************************************" << std::endl;

    world::deleteWorld();
    delete s1;
}

void fresnelTest3()
{
    world* pW = world::createWorld();

    sphere* s1 = glassSphere();
    pW->addObject(s1);

    ray r(point(0.0f, 0.99f, -2.0f), vector(0.0f, 0.0f, 1.0f));

    pW->intersect(r);

    std::cout << "intersections are: " << std::endl;
    std::vector < std::pair<int, float>> inters = pW->getIntersections();
    for (auto inter : inters)
    {
        std::cout << inter.second << ":" << inter.first << std::endl;
    }

    pIntDataT pID = pW->getIntData();
    float     reflectance = pW->schlick(pID);
    std::cout << "\n The reflectance is (0.48873): " << reflectance << std::endl;
    std::cout << "\n*************************************" << std::endl;

    world::deleteWorld();
    delete s1;
}

void fresnetTest4()
{}


void reflectionTest(int width, int height)
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

    i.writePPM("./chapter11a.ppm");
}

void refractionTest(int width, int height)
{
    std::cout << "creating image " << width << "x" << height << std::endl;

    // define a pattern
    checkeredPattern pat1(RED, YELLOW);
    pat1.xform(scale(2, 1, 2));

    // define the backdrop
    plane pl;
    pl.xform(translation(20, 0, 20) *  rotation_x(-90));
    material* pmat = pl.getMat();
    pmat->d(0.7f);
    pmat->sp(0.3f);
    pmat->p(&pat1);

    // define the outer sphere
    sphere outer(point(0.0f, 0.0f, 0.0f));
    outer.xform(scale(2.0f, 2.0f, 2.0f));
    material* pmat3 = outer.getMat();
    //pmat3->c(color(254.0f/255.0f, 254.0f / 255.0f, 252.0f / 255.0f));
    pmat3->a(1.0f);
    pmat3->transpar(1.0f);
    pmat3->refractIndex(1.5f);

    // define the inner sphere
    //sphere inner(point(0.0f, 0.0f, 0.0f));
    //material* pmat4 = inner.getMat();
    //pmat4->transpar(1.0f);
    //pmat4->refractIndex(1.5);

    // define the light source...  
    light l(color(1, 1, 1), point(-10, 10, -10));

    // define the camera  camera(int, int, float, matrix4x4)
    camera c(width, height, PI / 3, matrix4x4::identity());
    c.t(viewTransform(point(0, 1.5, -10), point(0, 1, 0), vector(0, 1, 0)));

    // build da world...
    world* w = world::createWorld();
    w->addObject(&pl);
    w->addObject(&outer);
    //w->addObject(&inner);
    w->addLight(&l);

    // build a canvas and render...
    canvas i(c.hsize(), c.vsize());
    c.render(w, &i);

    i.writePPM("./chapter11b.ppm");
}

/*
    `monsters.yaml`

```
- name: Ogre
  position: [0, 5, 0]
  powers:
    - name: Club
      damage: 10
    - name: Fist
      damage: 8
- name: Dragon
  position: [1, 0, 10]
  powers:
    - name: Fire Breath
      damage: 25
    - name: Claws
      damage: 15
- name: Wizard
  position: [5, -3, 0]
  powers:
    - name: Acid Rain
      damage: 50
    - name: Staff
      damage: 3
```

`main.cpp`

```
#include "yaml-cpp/yaml.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

// our data types
struct Vec3 {
   float x, y, z;
};

struct Power {
   std::string name;
   int damage;
};

struct Monster {
   std::string name;
   Vec3 position;
   std::vector <Power> powers;
};

// now the extraction operators for these types
void operator >> (const YAML::Node& node, Vec3& v) {
   node[0] >> v.x;
   node[1] >> v.y;
   node[2] >> v.z;
}

void operator >> (const YAML::Node& node, Power& power) {
   node["name"] >> power.name;
   node["damage"] >> power.damage;
}

void operator >> (const YAML::Node& node, Monster& monster) {
   node["name"] >> monster.name;
   node["position"] >> monster.position;
   const YAML::Node& powers = node["powers"];
   for(unsigned i=0;i<powers.size();i++) {
      Power power;
      powers[i] >> power;
      monster.powers.push_back(power);
   }
}

int main()
{
   std::ifstream fin("monsters.yaml");
   YAML::Parser parser(fin);
   YAML::Node doc;
   parser.GetNextDocument(doc);
   for(unsigned i=0;i<doc.size();i++) {
      Monster monster;
      doc[i] >> monster;
      std::cout << monster.name << "\n";
   }

   return 0;
}
    
    */

