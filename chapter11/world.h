#pragma once

#include <vector>
#include <map>

#include "object.h"
#include "light.h"
#include "point.h"

typedef struct _idintData
{
    int          ndx;            // index into vector of intersections.
    //int          prevNdx;        // index of previous intersections
    float        t;              // parameter along light ray to hit point 
    float        n1;             // index of refraction on outside material
    float        n2;             // index of refraction on inside material
    unsigned int id;             //
    bool         inside;
    bool         inShadow;
    object*      pObject;        // pointer to object hit
    point        pt;             // world coordinates where the hit happened
    point        overPt;         // point slightly above intersection point.
    point        underPt;        // point slightly below intersection point.
    vector       eyev;           // vector between eye and intersection point
    vector       normalv;        // surface normal of object hit
    vector       reflectv;       // direction of reflected vector
}intDataT, *pIntDataT;

class world
{
public:
    static world* createWorld(bool bDebug = false, color defColor = color(0.0f, 0.0f, 0.25f));
    static void   deleteWorld();

    void addLight(light*);
    void addObject(object*);
    object* getObject(unsigned int);
    color intersect(ray, int remaining = 5, int intNdx = -1);

    bool intersectShadow(ray,float);
    bool isShadowed(point pt, light* l);

    // TODO: should these be private? ,,, currently public for testing 
    color reflectColor(pIntDataT, int);
    color refractColor(pIntDataT, int);
    float schlick(pIntDataT);
    // testing functions
    std::vector < std::pair<int, float>> getIntersections() { return m_vecIntersections; } // get intersections
    pIntDataT getIntData() { return m_pID; }                                               // get data on an intersection 
    //color*    getColors() { return m_colors; }           // get normal, reflected, refracted colors 

    friend std::ostream& operator<< (std::ostream&, const world&);
    friend std::ostream& operator<<(std::ostream&, const intDataT);

private:
    world(bool, color);
    ~world();
    world(const world& rhs);
    const world& operator=(const world& rhs);

    void prepare(ray, pIntDataT, int intNdx = -1);
    color shadeHit(pIntDataT, int);
    //color reflectColor(pIntDataT, int);
    //color refractColor(pIntDataT, int);
    
    static world*                         m_pThis;
    int                                   m_nObjCnt;           // objects in the world

    std::map<int, object* >               m_mapObjects;        // map of objects, first - index, second - object pointer
    std::vector<light*>                   m_vecLights;
    std::vector < std::pair<int, float> > m_vecIntersections;  // vector of object IDs and t values
    pIntDataT                             m_pID;
    color                                 m_defaultColor;
    //color                                 m_colors[3];       // colors for normal, reflected and refracted colors
    //int                                   m_maxRecursion;
    bool                                  m_bDebug;
};

std::ostream& operator<<(std::ostream&, const intDataT);

