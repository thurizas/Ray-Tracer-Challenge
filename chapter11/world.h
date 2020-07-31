#pragma once

#include <vector>
#include <map>

#include "object.h"
#include "light.h"
#include "point.h"

typedef struct _idintData
{
    int          ndx;            // index into vector of intersections.
    float        t;              // parameter along light ray to hit point 
    unsigned int id;             //
    bool         inside;
    bool         inShadow;
    object*      pObject;        // pointer to object hit
    point        pt;             // world coordinates where the hit happened
    point        overPt;         // point slightly above intersection point.
    vector       eyev;           // vector between eye and intersection point
    vector       normalv;        // surface normal of object hit
    vector       reflectv;       // direction of reflected vector
}intDataT, *pIntDataT;

class world
{
public:
    static world* createWorld(color defColor = color(0.0f, 0.0f, 0.25f));
    static void   deleteWorld();

    void addLight(light*);
    void addObject(object*);
    color intersect(ray,int intNdx = -1);
    bool intersectShadow(ray,float);
    bool isShadowed(point pt, light* l);

    friend std::ostream& operator<< (std::ostream&, const world&);

private:
    world(color);
    ~world();
    world(const world& rhs);
    const world& operator=(const world& rhs);

    void prepare(ray, pIntDataT, int intNdx = -1);
    color shadeHit(pIntDataT);
    color reflectColor(pIntDataT);
    

    void printIntersections();
    void printIntersectionData(pIntDataT id);

    static world*             m_pThis;

    std::map<int, object* >               m_mapObjects;
    std::vector<light*>                   m_vecLights;
    std::vector < std::pair<int, float> > m_vecIntersections;
    color                                 m_defaultColor;
};

