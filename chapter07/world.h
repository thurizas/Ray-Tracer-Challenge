#pragma once

#include <vector>
#include <map>

#include "object.h"
#include "light.h"
#include "point.h"

typedef struct _idintData
{
    int          ndx;            // index into vector of intersections.
    float        t;
    unsigned int id;
    bool         inside;
    object*      pObject;
    point        pt;
    vector       eyev;
    vector       normalv;
}intDataT, *pIntDataT;

class world
{
public:
    static world* createWorld(color defColor = color(0.0f, 0.0f, 0.25f));
    static void   deleteWorld();

    void addLight(light*);
    void addObject(object*);
    std::vector< std::pair<int, float> >* getIntersections() { return &m_vecIntersections; }
    color intersect(ray,int intNdx = -1);

    friend std::ostream& operator<< (std::ostream&, const world&);

private:
    world(color);
    ~world();
    world(const world& rhs);
    const world& operator=(const world& rhs);

    void prepare(ray, pIntDataT, int intNdx = -1);
    color shadeHit(pIntDataT);

    void printIntersections();
    void printIntersectionData(pIntDataT id);

    static world*             m_pThis;

    std::map<int, object* >               m_mapObjects;
    std::vector<light*>                   m_vecLights;
    std::vector < std::pair<int, float> > m_vecIntersections;
    color                                 m_defaultColor;
};

