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




void reflectionTest1(bool flag = false);    // material modification
void reflectionTest2(bool flag = false);    // compute the reflectv vector
void reflectionTest3(bool flag = false);    // color striking a non-reflective surface
void reflectionTest4(bool flag = false);    // color striking a reflective surface
void reflectionTest5(bool flag = false);    // update shadeHit cal'cs
void reflectionTest6(bool flag = false);    // testing for unbounded recursion
void reflectionTest7(bool flag = false);    // bounding reflection recursion

void refractionTest1(bool flag = false);    // material modification and def. of glass sphere
void refractionTest2(bool flag = false);    // n1 and n2 at intersection points
void refractionTest3(bool flag = false);    // calculation of under-point
void refractionTest4(bool flag = false);    // color for refracted ray with opaque material
void refractionTest5(bool flag = false);    // color at limit of recursion
void refractionTest6(bool flag = false);    // test for internal reflection      
void refractionTest7(bool flag = false);    // refracted color with refracted ray
void refractionTest8(bool flag = false);    // shadeHit cal'cs correct color

void fresnelTest1(bool flag = false);       // reflectance under total internal reflectance
void fresnelTest2(bool flag = false);       // ray hits surface at 90 degree angle
void fresnelTest3(bool flag = false);       // n2 > n1 and angle is small
void fresnetTest4(bool flag = false);       // shadeHit cal'cs correct

void reflectionTest(int, int);
void refractionTest(int, int);
void mainTest(int, int);


int main(int argc, char** argv)
{
    int choice;
    bool   bDebug;
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

    //reflectionTest1(bDebug);                            
    //reflectionTest2(bDebug);                            
    //reflectionTest3(bDebug);                            
    //reflectionTest4(bDebug); // test5 will casue this to fail
    //reflectionTest5(bDebug);                            
    //reflectionTest6(bDebug);                            
    //reflectionTest7(bDebug);                            

    //refractionTest1(bDebug);                            
    //refractionTest2(bDebug);                            
    //refractionTest3(bDebug);                            
    //refractionTest4(bDebug);                            
    //refractionTest5(bDebug);                            
    //refractionTest6(bDebug);                            
    //refractionTest7(bDebug);                            
    refractionTest8(bDebug);          // shadeHit cal'cs correct color

    //fresnelTest1(g_bDebug);                                 
    //fresnelTest2(g_bDebug);                                 
    //fresnelTest3(g_bDebug);                                 
    //fresnetTest4(g_bDebug);                                 

    //reflectionTest(width, height);                // main reflection test
    //refractionTest(width, height);                // main refraction test
    //mainTest(width, height);
}

void reflectionTest1(bool flag)
{
    // features/material.features - reflectivity for the default material
    material m;                                                          // default material
    std::cout << "*** test #1 :  ************************" << std::endl;
    std::cout << "default material is: \n" << m << std::endl;
}

void reflectionTest2(bool flag)
{
    // features/intersections.feature - precomputation of reflection vector
    world* pW = world::createWorld();
    
    plane p1;
    pW->addObject(&p1);

    ray r(point(0.0f, 1.0f, -1.0f), vector(0, -(float)sqrt(2) / 2.0f, (float)sqrt(2) / 2.0f));
    pW->intersect(r);
 
    //std::cout << "reflected normal is [0, 0.707, 0.707]: " << pID->reflectv << std::endl;

    world::deleteWorld();
}

void reflectionTest3(bool flag)
{
    world* pW = defaultWorld();
    pW->getObject(1)->getMat()->a(1.0);
    ray r(point(0, 0, 0), vector(0, 0, 1));
    pW->intersect(r,3);                       // use a specific intersection for test
    
    std::cout << "\n*** test #3 :  ************************" << std::endl;
    //std::cout << "the reflected color is (0, 0, 0): " << pW->reflectColor(pID, 0) << std::endl;

    world::deleteWorld();
}

// N.B.  This test will fail once reflectionTest5 is working
//       shadeHit now calls refectColor
void reflectionTest4(bool flag)
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
    
    //std::cout << "the reflected color is (0.19032, 0.2379, 0.14274) : " << pW->reflectColor(pID, 0) << std::endl;


    world::deleteWorld();
}

void reflectionTest5(bool flag)
{
    world* pw1 = defaultWorld(true);
    
    plane pl;
    material* pmat1 = pl.getMat();
    pmat1->reflect(0.5f);
    pl.xform(translation(0.0f, -1.0f, 0.0f));

    pw1->addObject(&pl);

    ray r2(point(0, 0, -3.0f), vector(0, -(float)sqrt(2) / 2.0f, (float)sqrt(2) / 2.0f));


    std::cout << "\n*** test #5 :  ************************" << std::endl;
    
    world::deleteWorld();
}

// N.B. This test should fail with stack overflow
//      Once test 7 is working, this should succeeded.
void reflectionTest6(bool flag)
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
    
	std::cout << "\n*** test #5 :  ************************" << std::endl;

    world::deleteWorld();
}

void reflectionTest7(bool flag)
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
    
    //std::cout << "the reflected color is (0, 0, 0): " << pw1->reflectColor(pID, 0) << std::endl;
    world::deleteWorld();

}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// refraction unit tests....
void refractionTest1(bool flag)
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

void refractionTest2(bool flag)
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

    delete pS1;
    delete pS2;
    delete pS3;
    world::deleteWorld();
}

void refractionTest3(bool flag)
{
    // test #3 - feature/intersection.features - under point is offset below surface
    world* w1 = world::createWorld();

    sphere* pS4 = glassSphere();
    pS4->xform(translation(0, 0, 1));
    ray r2(point(0.0f, 0.0f, -5.0f), vector(0.0f, 0.0f, 1.0f));

    w1->addObject(pS4);

    std::cout << "\n*** test #3 ************************" << std::endl;

    w1->intersect(r2);

    world::deleteWorld();
}

void refractionTest4(bool flag)
{
    // test #4 - features/world.feature - the refracted color of an opaque surface is black
    world* w2 = defaultWorld();
    ray r3(point(0.0f, 0.0f, -5.0f), vector(0.0f, 0.0f, 1.0f));

    std::cout << "\n*** test #4 ************************" << std::endl;
    w2->intersect(r3);
 
    //std::cout << "\nrefracted color is (0,0,0): " << w2->refractColor(pID, 5) << std::endl;

    world::deleteWorld();
}

void refractionTest5(bool flag)
{
    // test #5 - features/world.feature - the refracted color at maximum recursive depth
    world* pW = defaultWorld();
    pW->getObject(1)->getMat()->transpar(1.0f);
    pW->getObject(1)->getMat()->refractIndex(1.5f);

    ray r(point(0.0f, 0.0f, -5.0f), vector(0.0f, 0.0f, 1.0f));
    pW->intersect(r);
    std::cout << "\n*** test #5 ************************" << std::endl;
 
    //std::cout << "\nthe refracted color is (0,0,0): " << pW->refractColor(pID, 0) << std::endl;

    world::deleteWorld();
}

void refractionTest6(bool flag)
{
    // test #6 - features/world.feature - the refracted color under total internal reflection
    world* pW = defaultWorld();
    pW->getObject(1)->getMat()->transpar(1.0f);
    pW->getObject(1)->getMat()->refractIndex(1.5f);

    ray r(point(0.0f, 0.0f, sqrt(2.0f) / 2.0f), vector(0.0f, 1.0f, 0.0f));
    pW->intersect(r);

    std::cout << "\n*** test #6 ************************" << std::endl;
 
    //std::cout << "\nthe refracted color is (0, 0, 0):" << pW->refractColor(pID, 5) << std::endl;
    
    world::deleteWorld();
}

// TODO : check defPattern is working correctly, check refracted direction and intersection point.
void refractionTest7(bool flag)
{
    // test #7 - features/world.feature - the refracted color with a refracted ray
    world* pW = defaultWorld(flag);
    pW->getObject(1)->getMat()->a(1.0f);
    pW->getObject(1)->getMat()->p(new defPattern());   
    pW->getObject(2)->getMat()->transpar(1.0);
    pW->getObject(2)->getMat()->refractIndex(1.5);
    ray r(point(0, 0, 0.1f), vector(0, 1.0f, 0));
    
    pW->intersect(r);

    std::cout << "\n*** test #7 ************************" << std::endl;
 
    world::deleteWorld();
}

void refractionTest8(bool flag)
{
    // test #8 - features/world.feature - shade_hit() with a transparent material
    world* pW = defaultWorld(true);

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
    std::cout << "intersections are (1.41421:3) : " << std::endl;
    std::cout << "intersections are (1.92237:4) : " << std::endl;

    std::cout << "\n The final color is (0.93642, 0.68642, 0.68642): " << cl << std::endl;
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
    //float     reflectance = pW->schlick(pID);
    //std::cout << "\n The reflectance is: (1.0)" << reflectance << std::endl;
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


    //float     reflectance = pW->schlick(pID);
    //std::cout << "\n The reflectance is (0.04): " << reflectance << std::endl;
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

    //float     reflectance = pW->schlick(pID);
    //std::cout << "\n The reflectance is (0.48873): " << reflectance << std::endl;
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

    world::deleteWorld();
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

    // define the light source...  
    light l(color(1, 1, 1), point(-10, 10, -10));

    // define the camera  camera(int, int, float, matrix4x4)
    camera c(width, height, PI / 3, matrix4x4::identity());
    c.t(viewTransform(point(0, 1.5, -10), point(0, 1, 0), vector(0, 1, 0)));

    // build da world...
    world* w = world::createWorld();
    w->addObject(&pl);
    w->addObject(&outer);
    w->addLight(&l);

    // build a canvas and render...
    canvas i(c.hsize(), c.vsize());
    c.render(w, &i);

    i.writePPM("./chapter11b.ppm");
}

void mainTest(int width, int height)
{
    width = 300;
    height = 150;
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

    // create red ball
    sphere sp1(point(0.0f, 0.0f, 0.0f));
    sp1.xform(translation(4.0f, 1.0f, 4.0f));
    sp1.getMat()->c(color(0.8f, 0.1f, 0.3f));
    sp1.getMat()->sp(0.0f);

    // create green ball
    sphere sp2(point(0.0f, 0.0f, 0.0f));
    sp2.xform(scale(0.4f, 0.4f, 0.4f));
    sp2.xform(translation(4.6f, 0.4f, 2.9f));
    //sp2.xform(scale(0.4f, 0.4f, 0.4f)*translation(4.6f, 0.5f, 2.9f));
    sp2.getMat()->c(color(0.1f, 0.8f, 0.2f));
    sp2.getMat()->sh(200.0f);

    // create blue ball
    sphere sp3(point(0.0f, 0.0f, 0.0f));
    sp3.xform(scale(0.6f, 0.6f, 0.6f));
    sp3.xform(translation(2.6f, 0.6f, 4.4f));
    sp3.getMat()->c(color(0.2f, 0.1f, 0.8f));
    sp3.getMat()->sh(10.0f);
    sp3.getMat()->sp(0.4f);

    // create glass ball
    sphere sp4(point(0.0f, 0.0f, 0.0f));
    sp4.xform(translation(0.25f, 1.0f, 0.0f));
    sp4.getMat()->c(color(0.8f, 0.8f, 0.9f));
    sp4.getMat()->a(0.0f);
    sp4.getMat()->d(0.9f);
    sp4.getMat()->sp(0.9f);
    sp4.getMat()->sh(300.0f);
    sp4.getMat()->transpar(0.8f);
    sp4.getMat()->refractIndex(1.57f);

    // define the light source...  
    light l(color(1, 1, 1), point(-4.9f, 4.9f, 1));

    // define the camera  camera(int, int, float, matrix4x4)
    camera c(width, height, 0.5, matrix4x4::identity());
    c.t(viewTransform(point(-4.5f, 0.85f, -4), point(0, 0.85f, 0), vector(0, 1, 0)));

    // build da world...
    world* w = world::createWorld();
    w->addObject(&pl);
    w->addObject(&p2);
    w->addObject(&p3);
    w->addObject(&sp1);
    w->addObject(&sp2);
    w->addObject(&sp3);
    w->addObject(&sp4);
    w->addLight(&l);

    // build a canvas and render...
    canvas i(c.hsize(), c.vsize());
    c.render(w, &i);

    i.writePPM("./chapter11c.ppm");

    world::deleteWorld();
}


/*
`main.cpp`


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

