// chapter09.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "pch.h"
#include "world.h"
#include "light.h"
#include "sphere.h"
#include "plane.h"
#include "transformations.h"
#include "camera.h"
#include <iostream>

class testShape : public object
{
public:
    testShape(material* pm = new material, unsigned short t = TEST) : object(pm, t), m_matXform(m_xform) 
    { }
    bool intersect(const ray& r, pInterInfo pint)
    { 
        m_r1 = const_cast<ray&>(r);
        m_r1.xform(m_matXform.inverse());
        return false;
    }
    vector normalAt(point pt)
    {
        point localPoint = m_matXform.inverse()*pt;
        vector localNormal(localPoint.x(), localPoint.y(), localPoint.z());
        vector worldNormal = m_matXform.inverse().transpose() * localNormal;
        worldNormal.w(0);
        worldNormal.normalize();

        return worldNormal;
    }
    
    material* getMat() { return m_pMat; }
    virtual void mat(material*) {}

    matrix4x4 getXform() { return m_matXform; }
    void xform(matrix4x4 T)
    {
        m_matXform = T * m_matXform;            // start chain with the default matrix
    }

    ray m_r1;


private:
    matrix4x4    m_matXform;
    
};

void objectTests();
void planeTests();
void mainTest();

int main()
{
    //objectTests();
    //planeTests();
    mainTest();
}

void objectTests()
{
    // features/shape.features - default shape has default transform matrix
    testShape ts;
    std::cout << "default shape, transform matrix: " << ts.getXform();

    // feature/shape.features - adding a transformation
    ts.xform(translation(2, 3, 4));
    std::cout << "default shape, set transform matrix: " << ts.getXform();

    // features/shapes.feature ​  ​Scenario​: The default material
    material* pmat = ts.getMat();
    std::cout << "default shape, material: " << *pmat << std::endl;

    // features/shapes.feature Scenario: assigning a material
    pmat->a(1);
    ts.mat(pmat);
    std::cout << "default shape, setting material: " << (*ts.getMat()) << std::endl;

    // features/shape.feature Scenario​: Intersecting a scaled shape with a ray
    interInfo intInfo;
    testShape ts1;
    ray r1(point(0, 0, -5), vector(0, 0, 01));
    ts1.xform(scale(2, 2, 2));
    ts1.intersect(r1, &intInfo);
    std::cout << "default shape, transformed ray is ((0, 0, -2.5), [0, 0, 0.5])" << ts1.m_r1 << std::endl;

    // features/shape.feature Scenario:Intersecting a translated shape with a ray
    interInfo intInfo1;
    testShape ts2;
    ts2.xform(translation(5, 0, 0));
    ts2.intersect(r1, &intInfo1);
    std::cout << "default shape, transformed ray is ((-5, 0, -5), [0,0,1])" << ts2.m_r1 << std::endl;

    //features/shapes.feature ​  ​Scenario​: Computing the normal on a translated shape
    testShape ts3;
    ts3.xform(translation(0, 1, 0));
    vector normal = ts3.normalAt(point(0, 1.70711, -0.70711));
    std::cout << "default shape, translated normal is [0, 1.70711, -0.70711]" << normal;

}

void planeTests()
{
    // features / planes.feature ​  ​Scenario​ : The normal of a plane is constant everywhere ​  
    plane pl1;
    vector n1 = pl1.localNormalAt(point(0, 0, 0));
    vector n2 = pl1.localNormalAt(point(10, 0, -10));
    vector n3 = pl1.localNormalAt(point(-5, 0, 150));

    std::cout << "normal at point (0,0,0) is: [0,1,0] " << n1 << std::endl;
    std::cout << "normal at point (10, 0, -10) is: [0,1,0] " << n2 << std::endl;
    std::cout << "normal at point (-5, 0, 150) is: [0,1,0] " << n3 << std::endl;

    
    //features/planes.feature ​  ​Scenario​: Intersect with a ray parallel to the plane
    interInfo   inter;
    ray r1(point(0, 10, 0), vector(0, 0, 1));
    bool bInter = pl1.intersect(r1, &inter);
    std::cout << "\n\nray r1 intersects plane pl1 at: (empty)" << (bInter ? " intersection at": " no intersection") << std::endl;

    //​Scenario​: Intersect with a coplanar ray ​  
    ray r2(point(0, 0, 0), vector(0, 0, 1));
    bInter = pl1.intersect(r2, &inter);
    std::cout << "ray r2 intersects plane pl1 at: (empty)" << (bInter ? " intersection at" : " no intersection") << std::endl;

   //Scenario​: A ray intersecting a plane from above
    ray r3(point(0, 1, 0), vector(0, -1, 0));
    bInter = pl1.intersect(r3, &inter);
    std::cout << "ray r3 intersects plane pl1 at: ";
    if (bInter)
        std::cout << "count " << inter.getCnt() << " " << inter.m_nt1 << std::endl;
    else
        std::cout << " no intersection"; 
        
   //Scenario​ : A ray intersecting a plane from below
    inter.clear();
    ray r4(point(0, -1, 0), vector(0, 1, 0));
    bInter = pl1.intersect(r4, &inter);
    std::cout << "ray r4 intersets plane pl1 at: ";
    if (bInter)
        std::cout << "count " << inter.getCnt() << " " << inter.m_nt1 << std::endl;
    else
        std::cout << " no intersection";

}





void mainTest()
{
    int   width = 640;//640 /*100*/;
    int   height = 480;//480/*50*/;

    // define the floor
    plane pl;
    pl.xform(scale(40, 1, 40));
    //pl.xform(translation(1, 1, 1));
    material* pmat = pl.getMat();
    pmat->c(color(0.5019f, 0.2510f, 0.2510f));
    pmat->d(0.7f);
    pmat->sp(0.3f);

    // define the middle sphere
    sphere middle(point(0.0f, 0.0f, 0.0f));
    middle.xform(translation(-0.5f, 1.0, 0.5f));
    pmat = middle.getMat();
    pmat->c(color(0.1f, 1, 0.5f));
    pmat->d(0.7f);
    pmat->sp(0.3f);

    // define the right sphere
    sphere right(point(0.0f, 0.0f, 0.0f));
    right.xform(translation(1.5f, 0.5f, -0.5f)*scale(0.5f, 0.5f, 0.5f));
    pmat = right.getMat();
    pmat->c(color(0.5f, 1, 0.1f));
    pmat->d(0.7f);
    pmat->sp(0.3f);

    // define the left sphere
    sphere left(point(0.0f, 0.0f, 0.0f));
    left.xform(translation(-1.5f, 0.45f, -0.75f)*scale(0.33f, 0.33f, 0.33f));
    pmat = left.getMat();
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
    w->addObject(&pl);
    w->addObject(&left);
    w->addObject(&middle);
    w->addObject(&right);
    w->addLight(&l);

    std::cout << "world is: " << *w << std::endl;
    
    // build a canvas and render...
    canvas i(c.hsize(), c.vsize());
    c.render(w, &i);

    i.writePPM("./chapter09a.ppm");
}
