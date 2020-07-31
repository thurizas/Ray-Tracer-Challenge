#include "pch.h"

#include "world.h"
#include "sphere.h"
#include "plane.h"
#include "ray.h"

#include <iostream>
#include <algorithm>

world* world::m_pThis = nullptr;


world* world::createWorld(color defColor)
{
    if (m_pThis == nullptr)
    {
        m_pThis = new world(defColor);
    }

    return m_pThis;
}


// TODO : if objects are created on the stack, then deleting them is an error
// TODO : however, we do need to be able to detele the materials (as the default 
// TODO : is created on the heap)
void world::deleteWorld()
{
    if (m_pThis != nullptr)
    {
        delete m_pThis;
        m_pThis = nullptr;
    }
}

void world::addLight(light* l)
{
    if (l != nullptr)
    {
        m_vecLights.push_back(l);
    }
}


void world::addObject(object* o)
{
    if (o != nullptr)
    {
        m_mapObjects.insert(std::pair<int, object*>( o->getID(), o));
    }
}


/*
* function: intersect
*
* abstract: This function determines the color that will be used if an intersection has occured.  This function
*           is called from camera::render(world* canvas*) and calls world::prepare and world::shadeHit to 
*           calculate the color of the pixel.
*
* inputs  : r -- [in] ray that is being tested
*
* returns : color - the color that the pixel will be drawn in
*
* written : (7/29/2020)
*/
color world::intersect(ray r, int intNdx)
{
    bool bret = false;
    // delete any old intersections, if any
    if (m_vecIntersections.size() > 0)
    {
        m_vecIntersections.erase(m_vecIntersections.begin(), m_vecIntersections.end());
    }

    if (m_mapObjects.size() > 0)         // we have objects to work with
    {
        std::map<int, object*>::iterator    iter = m_mapObjects.begin();
        while (m_mapObjects.end() != iter)
        {
            interInfo intInfo;

            if (SPHERE == (*iter).second->getType())       // intersect a sphere
            {
                sphere* pSphere = dynamic_cast<sphere*>((*iter).second);
                if (pSphere->intersect(r, &intInfo))
                {
                    m_vecIntersections.push_back ( std::pair<int, float>(pSphere->getID(), intInfo.t1()));
                    m_vecIntersections.push_back ( std::pair<int, float>(pSphere->getID(), intInfo.t2()));

                    bret = true;
                }
            }
            else if(PLANE == (*iter).second->getType())
            {
                plane* pPlane = dynamic_cast<plane*>((*iter).second);
                if (pPlane->intersect(r, &intInfo))
                {
                    m_vecIntersections.push_back(std::pair<int, float>(pPlane->getID(), intInfo.t1()));
                    bret = true;
                }
            }
            else
            {
                std::cerr << "unknown geometry type" << std::endl;
            }
            iter++;
        }

    }

    if (bret)
    {
        intDataT  id;
        std::sort(m_vecIntersections.begin(), m_vecIntersections.end(), [](std::pair<int, float>lhs, std::pair<int, float> rhs) {return lhs.second < rhs.second;});
        prepare(r, &id, intNdx);
        color objColor = shadeHit(&id);
        if (id.pObject->getMat()->reflect() != 0)    // compute the reflection
        {
            objColor = objColor + reflectColor(&id);            
        }

        return objColor;
    }
    else
    {
        return m_defaultColor;
    }
}


/*
* function: prepare
*
* abstract: This function prepares the intersection information that is needed to determine the color of
*           the pixel.  We use the intersection with the lowest `t' value, ie the earliest intersection
*           based on the parameter in the paramertized form of the ray.  This function is called from
*           world::intersect(ray, int), which in turn is called from camera::render(world* w, canvas* i)
*
* inputs  : r -- [in] the ray we are working with.
*           pID -- [out] pointer to the intersection data we need (this is defined in world.h)
*           intNdx -- [in] the intersection index to use, it defaults to -1, which is to use the lowest
*                     index value (i.e. lowest t value).
*
* returns :
*
* written : (7/29/2020)
*/
void world::prepare(ray r, pIntDataT pID, int intNdx)
{
    if (nullptr != pID)
    {
        unsigned int ndx;
        if (-1 == intNdx)
        {
            for (ndx = 0; ndx < m_vecIntersections.size(); ndx++)
            {
                if (m_vecIntersections.at(ndx).second > 0)
                    break;
            }
        }
        else
        {
            ndx = intNdx;
        }

        pID->ndx = ndx;
        pID->t = (m_vecIntersections.at(ndx)).second;
        pID->id = (m_vecIntersections.at(ndx)).first;
        pID->inside = false;
        pID->inShadow = false;

        std::map<int, object*>::iterator  iter = m_mapObjects.find(pID->id);
        if (iter != m_mapObjects.end())
        {
            object* o = (*iter).second;
            pID->pObject = (*iter).second;
            pID->pt = r.distance(pID->t);
            pID->eyev = -r.direction();

            if (SPHERE == (o)->getType())
            {
                pID->normalv = (dynamic_cast<sphere*>(o))->normalAt(pID->pt);
                pID->overPt = pID->pt + pID->normalv*EPSILON;
                pID->reflectv = r.direction().reflect(pID->normalv);

                if (pID->normalv.dot(pID->eyev) < 0)
                {
                    pID->inside = true;
                    pID->normalv = -pID->normalv;
                }
            }
            else if (PLANE == (o)->getType())
            {
                pID->normalv = (dynamic_cast<plane*>(o))->normalAt(pID->pt);
                pID->overPt = pID->pt + pID->normalv*EPSILON;
                pID->reflectv = r.direction().reflect(pID->normalv);

                if (pID->normalv.dot(pID->eyev) < 0)
                {
                    pID->inside = true;
                    pID->normalv = -pID->normalv;
                }
            }
            else
            {
                std::cout << "unsupported shape type" << std::endl;
            }
        }
    }
}


/*
* function: world::shadeHit
*
* abstract: This function determines the color of a pixel, based on the lighting, material and if the point is in
*           a shadow.
*
* inputs  : pID - [in] pointer to a intDataT (see world.h) with the information of where the intersection took 
*                 place
*
* returns : color of the pixel.
*
* written : (7/29/2020)
*/
color world::shadeHit(pIntDataT pID)
{
    color    clrRet(0.0f, 0.0f, 0.0f);

    std::vector<light*>::iterator    iter = m_vecLights.begin();
    while (m_vecLights.end() != iter)
    {
        bool bShadowed = isShadowed(pID->overPt, (*iter));
        clrRet = clrRet + (*iter)->lighting(*((pID->pObject)->getMat()), pID->pObject, pID->overPt, pID->eyev, pID->normalv, bShadowed);
        iter++;
    }

    return clrRet;
}

/*
* function: world::reflectColor
*
* abstract:
*
* inputs  :
*
* returns : 
*
* written : (7/29/2020)
*/
color world::reflectColor(pIntDataT pID)
{
    color retColor = BLACK;

    float reflective = (pID->pObject)->getMat()->reflect();
    if (fabs(reflective) <= EPSILON)
        return retColor;
    else
    {
        ray reflectRay = ray(pID->overPt, pID->reflectv);
        retColor = intersect(reflectRay);

        retColor = reflective * retColor;

        return retColor;
    }

}


/*
* function: world::isShadowed
*
* abstract: This function determines if given a point and a light source is in the shadow of some object in
*           the world.
*
* inputs  : pt -- [in] point to test
*           l -- [in] light source in the world
*
* returns : true if the point is in a shadow, flase otherwise.
*
* written : (7/29/2020)
*/
bool world::isShadowed(point pt, light* l)
{
    vector shadowV = l->p() - pt;
    float distance = shadowV.norm();
    vector direction = shadowV;
    direction.normalize();

    ray shadowR(pt, direction);
    return intersectShadow(shadowR, distance);

}


/*
* function: world::intersectShadow
*
* abstract:
*
* inputs  : r -- [in] ray to intersect with objects in the world
*           d -- [in] point of the intersection
*
* returns : returns true if an intersection happened, false otherwise.
*
* written : (7/29/2020)
*/
bool world::intersectShadow(ray r, float d)
{
    bool bhit = false;
    bool bret = false;
    std::vector < std::pair<int, float> > vecShadowHits;

    if (m_mapObjects.size() > 0)         // we have objects to work with
    {
        std::map<int, object*>::iterator    iter = m_mapObjects.begin();
        while (m_mapObjects.end() != iter)
        {
            interInfo intInfo;

            if (SPHERE == (*iter).second->getType())       // intersect a sphere
            {
                sphere* pSphere = dynamic_cast<sphere*>((*iter).second);
                if (pSphere->intersect(r, &intInfo))
                {
                    vecShadowHits.push_back(std::pair<int, float>(pSphere->getID(), intInfo.t1()));
                    vecShadowHits.push_back(std::pair<int, float>(pSphere->getID(), intInfo.t2()));

                    bhit = true;
                }
            }
            else if (PLANE == (*iter).second->getType())
            {

                plane* pPlane = dynamic_cast<plane*>((*iter).second);
                if (pPlane->intersect(r, &intInfo))
                {
                    vecShadowHits.push_back(std::pair<int, float>(pPlane->getID(), intInfo.t1()));
                    bhit = true;
                }
            }
            else
            {
                std::cerr << "unknown geometry type" << std::endl;
            }
            iter++;
        }

        if (bhit)
        {
            intDataT  id;
            int       ndx = 0;
            int       count = vecShadowHits.size();
            std::sort(vecShadowHits.begin(), vecShadowHits.end(), [](std::pair<int, float>lhs, std::pair<int, float> rhs) { return lhs.second < rhs.second; });

            while ((ndx < count - 1) && (vecShadowHits.at(ndx).second < 0))
            {
                ndx = ndx + 1;
            }

            if ((ndx < count - 1) && (vecShadowHits.at(ndx).second < d))
                bret = true;

        }
    }
    return bret;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// these functions are hidden so we can only manipulate the world object the the m_pThis pointer
world::world(color dclr) : m_defaultColor(dclr)
{ }

world::~world()
{
    // delete all the lights in the world....
    if (m_vecLights.size() > 0)
    {
        std::vector<light*>::iterator  iter = m_vecLights.begin();
        while (m_vecLights.end() != iter)
        {
            if (*iter != nullptr)
            {
                //delete *iter;
                *iter = nullptr;
            }

            iter++;
        }
    }

    // delete objects in the world....
    if (m_mapObjects.size() > 0)
    {
        std::map<int, object*>::iterator iter = m_mapObjects.begin();
        while (m_mapObjects.end() != iter)
        {
            //delete (*iter).second;
            (*iter).second = nullptr;
            iter++;
        }
    }
}

world::world(const world& rhs)
{  }

const world& world::operator=(const world& rhs)
{  
    return *m_pThis;
}




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// output functions.
std::ostream& operator<<(std::ostream& os, const world& rhs)
{
    os << "\nLights: " << std::endl;
    if (rhs.m_vecLights.size() > 0)
    {
        std::vector<light*>::iterator  iter = const_cast<world&>(rhs).m_vecLights.begin();
        while (rhs.m_vecLights.end() != iter)
        {
            os << *(*iter);
            ++iter;
        }
    }
    else
    {
        os << "No lights defined for this world!" << std::endl;
    }
    
    os << "\nObjects: " << std::endl;
    if (rhs.m_mapObjects.size() > 0)
    {
        //std::vector<object*>::iterator iter = const_cast<world&>(rhs).m_vecObjects.begin();
        std::map<int, object*>::iterator iter = const_cast<world&>(rhs).m_mapObjects.begin();
        while (rhs.m_mapObjects.end() != iter)
        {
            if (SPHERE == ((*iter).second)->getType())
            {
                sphere* s = dynamic_cast<sphere*>((*iter).second);
                os << *(s);
            }
            else if (PLANE == ((*iter).second)->getType())
            {
                plane* p = dynamic_cast<plane*>((*iter).second);
                os << *(p);
            }
            else
            {
                os << "unsupported object type" << std::endl;
            }
            
            iter++;
        }
    }
    else
    {
        os << "No objects defined in this world!" << std::endl;
    }

    return os;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
// debugging functions
void world::printIntersections()
{
    std::cout << "intersections:" << std::endl;
    std::vector<std::pair<int, float> >::iterator iter = m_vecIntersections.begin();
    while (m_vecIntersections.end() != iter)
    {
        std::cout << "Object: " << (*iter).first << " at distance: " << (*iter).second << std::endl;
        ++iter;
    }
}



void world::printIntersectionData(pIntDataT id)
{
    std::cout << "precomputed intersection data:" << std::endl;
    std::cout << "    time value  : " << id->t << std::endl;
    std::cout << "    object id   : " << id->id << std::endl;
    std::cout << "    inside shape: " << (id->inside ? "true" : "false") << std::endl;
    std::cout << "    world point : " << id->pt << std::endl;
    std::cout << "    eye vector  : " << id->eyev << std::endl;
    std::cout << "    normal vec  : " << id->normalv << std::endl;


}
