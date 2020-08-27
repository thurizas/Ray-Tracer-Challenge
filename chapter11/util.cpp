#include "util.h"

#include "sphere.h"
//#include "pattern.h"
#include "matrix4x4.h"
#include "transformations.h"

std::ostream& operator<<(std::ostream& os, const std::pair<int, float> t)
{
    os << "[" << t.first << ", " << t.second << "]";
    return os;
}


sphere* glassSphere()
{
    sphere* pSphere = new sphere(point(0, 0, 0));
    material* mat = pSphere->getMat();
    mat->transpar(1.0f);
    mat->refractIndex(1.5);

    return pSphere;
}

world* defaultWorld()
{
    world* pWorld = world::createWorld();

    light* pLight = new light(color(1, 1, 1), point(-10, 10, -10));

    sphere* pS1 = new sphere(point(0, 0, 0));
    material* pMat1 = pS1->getMat();
    pMat1->c(color(0.8f, 1.0f, 0.6f));
    pMat1->d(0.7f);
    pMat1->sp(0.2f);

    sphere* pS2 = new sphere(point(0, 0, 0));
    pS2->xform(scale(0.5, 0.5, 0.5));

    pWorld->addLight(pLight);
    pWorld->addObject(pS1);
    pWorld->addObject(pS2);

    return pWorld;
}

//pattern* testPattern()
//{
//    defPattern* pat = new defPattern(WHITE, BLACK);
//    //stripePattern* pat = new stripePattern(WHITE, BLACK);
//
//    pat->xform(translation(1, 2, 3));
//
//    return pat;
//}