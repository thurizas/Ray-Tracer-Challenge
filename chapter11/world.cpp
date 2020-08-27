#include "pch.h"

#include "world.h"
#include "sphere.h"
#include "plane.h"
#include "ray.h"
#include "util.h"

#include <iostream>
#include <algorithm>
#include <iterator>

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
    int objID = ++m_nObjCnt;

    if (o != nullptr)
    {
        o->setID(objID);         //  inform object of it's name....
        m_mapObjects.insert(std::pair<int, object*>( objID, o));
    }
}

/*
* function: getObject
*
* abstract: This function retreves and object from the world, by index number, if the object is not found, the 
*           function return nullptr.
*
* inputs  : ndx -- [in] index of the object to find
*
* returns : returns an object pointer or nullptr if the object is not found
*
* written : (8/23/2020)
*/
object* world::getObject(unsigned int ndx)
{
    object* obj = nullptr;

    if ((ndx < 0) || (m_mapObjects.size() < ndx))
    {
        std::cout << "index out of range" << std::endl;
    }
    else
    {
        std::map<int, object*>::iterator iter;

        if (m_mapObjects.end() != (iter = m_mapObjects.find(ndx)))
        {
            obj = (*iter).second;
        }
    }
    
    return obj;
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
color world::intersect(ray r, int remaining, int intNdx)
 {
     bool bret = false;
     std::cout << "\nRecursion count " << remaining << " Using ray: " << r << std::endl;
    
    if (m_vecIntersections.size() > 0)             // delete any old intersections, if any
    {
        m_vecIntersections.erase(m_vecIntersections.begin(), m_vecIntersections.end());
    }

    if (nullptr != m_pID)                           // delete any old intersection data, if needed
        delete m_pID;

    m_pID = new intDataT;                           // create a new intersection data structure

    if (m_mapObjects.size() > 0)                    // we have objects to work with
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

    for (auto inter : m_vecIntersections)
    {
        std::cout << inter.second << ":" << inter.first << ", ";
    }
    std::cout << std::endl;

    if (bret)
    {
        std::sort(m_vecIntersections.begin(), m_vecIntersections.end(), [](std::pair<int, float>lhs, std::pair<int, float> rhs) {return lhs.second < rhs.second;});

        prepare(r, m_pID, intNdx);
        
        color objColor = shadeHit(m_pID, remaining);

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
*                     index value with a positive intersection value, i.e. infront of the ray's 
*                     origin.
*
* returns :
*
* written : (7/29/2020)
*/
void world::prepare(ray r, pIntDataT pID, int intNdx)
{
    int cntIntersections = m_vecIntersections.size();

    if (nullptr != pID)
    {
        unsigned int ndx;
        if (-1 == intNdx)                  // defalut value -- use the first positive intersection
        {
            for (ndx = 0; ndx < m_vecIntersections.size(); ndx++)
            {
                if (m_vecIntersections.at(ndx).second > 0)
                    break;
            }
        }
        else                               // use the specified intersection
        {
            ndx = intNdx;
        }

        // get last object the ray was in, 
        std::vector<int>      vecObjects;  // object container
        bool bContinue = true;

        for (int ints = 0; ints < cntIntersections && bContinue; ints++)
        {        
            std::vector<int>::iterator  iter;
            if (ints == ndx)                  // our intersection is the one we are looking at
            {
                if (vecObjects.size() == 0)
                    pID->n1 = 1.00;
                else
                {
                    int objID = vecObjects.back();
                    std::map<int, object*>::iterator mIter = m_mapObjects.find(objID);
                    pID->n1 = (*mIter).second->getMat()->refractIndex();
                }
            }

            if (vecObjects.end() != (iter = std::find(vecObjects.begin(), vecObjects.end(), m_vecIntersections.at(ints).first)))
            {
                vecObjects.erase(iter);
            }
            else
            {
                vecObjects.push_back(m_vecIntersections.at(ints).first);
            }

            if(ints == ndx)
            {
                if (vecObjects.size() == 0)
                    pID->n2 = 1.0f;
                else
                {
                    std::map<int, object*>::iterator mIter = m_mapObjects.find(vecObjects.back());
                    pID->n2 = (*mIter).second->getMat()->refractIndex();
                }
                bContinue = false;   // got both sides of the intersection, exit
            }
        }

        pID->ndx = ndx;
        pID->t = (m_vecIntersections.at(ndx)).second;
        pID->id = (m_vecIntersections.at(ndx)).first;
        pID->inside = false;
        pID->inShadow = false;

        std::map<int, object*>::iterator  iter = m_mapObjects.find(pID->id);

        if (iter != m_mapObjects.end())
        {
            object* o = (*iter).second;      // object we hit
            pID->pObject = (*iter).second;
            pID->pt = r.distance(pID->t);
            pID->eyev = -r.direction();

            if (SPHERE == (o)->getType())
            {
                pID->normalv = (dynamic_cast<sphere*>(o))->normalAt(pID->pt);
                if (pID->normalv.dot(pID->eyev) < 0)
                {
                    pID->inside = true;
                    pID->normalv = -pID->normalv;
                }
                pID->overPt = pID->pt + pID->normalv*EPSILON;
                pID->underPt = pID->pt - pID->normalv*EPSILON;
                pID->reflectv = r.direction().reflect(pID->normalv);


            }
            else if (PLANE == (o)->getType())
            {
                pID->normalv = (dynamic_cast<plane*>(o))->normalAt(pID->pt);
                if (pID->normalv.dot(pID->eyev) < 0)
                {
                    pID->inside = true;
                    pID->normalv = -pID->normalv;
                }
                pID->overPt = pID->pt + pID->normalv*EPSILON;
                pID->underPt = pID->pt - pID->normalv*EPSILON;
                pID->reflectv = r.direction().reflect(pID->normalv);


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
color world::shadeHit(pIntDataT pID, int remaining)
{
    color    clrRet(0.0f, 0.0f, 0.0f);

    std::vector<light*>::iterator    iter = m_vecLights.begin();
    while (m_vecLights.end() != iter)
    {
        bool bShadowed = isShadowed(pID->overPt, (*iter));
        clrRet = clrRet + (*iter)->lighting(*((pID->pObject)->getMat()), pID->pObject, pID->overPt, pID->eyev, pID->normalv, bShadowed);
        iter++;
    }

    color reflected = reflectColor(pID, remaining);
    color refracted = refractColor(pID, remaining);

    return clrRet + reflected +refracted;
}

/*
* function: world::reflectColor
*
* abstract: This color calculates the color of a reflected ray.  If the object is obaque we return the color 
*           BLACK (0,0,0).  We check for total internal reflection by using snells law with sin(\theta_r)^2 > 1
*
* inputs  : pID -- [in] pointer to an intersection data struture that contains the necessary information on the intersection
*
* returns : the color of the refracted ray as a (r,g,b) tuple
*
* written : (7/29/2020)
*/
color world::reflectColor(pIntDataT pID, int remaining)   
{
    color retColor = BLACK;

    float reflective = (pID->pObject)->getMat()->reflect();
    if (fabs(reflective) <= EPSILON)
        return retColor;
    else if (remaining <= 0)
        return retColor;
    else
    {
        ray reflectRay = ray(pID->overPt, pID->reflectv);
        retColor = intersect(reflectRay, (remaining-1));
        retColor = reflective * retColor;

        return retColor;
    }

}

/*
* function: world::refractColor
*
* abstract: This color calculates the color of a refracted ray
*
* inputs  : pID -- [in] pointer to an intersection data struture that contains the necessary information on the intersection
*           depth -- [in] number of recusions we have done for this ray
*
* returns : the color of the refracted ray as a (r,g,b) tuple
*
* written : (8/23/2020)
*/
color world::refractColor(pIntDataT pID, int remaining)
{
    color retColor = BLACK;

    float ratio = pID->n1 / pID->n2;                        // ratio of index of refraction
    float cosI = pID->normalv.dot(pID->eyev);               // cosine of incident angle
    float sinR2 = ratio * ratio*(1 - cosI * cosI);          // sine of refracted angle, squared (sin^2A = 1 - cos^2A)


    float transparent = (pID->pObject)->getMat()->transpar();
    if (fabs(transparent) <= EPSILON)                       // obaque material
        return retColor;
    else if (remaining <= 0)                                // passed recursion depth
        return retColor;
    else if (sinR2 > 1.0f)                                  // total internal reflection
        return retColor;
    else
    {
        float cosR = sqrt((1 - sinR2));                              // cosine of refracted angle
        vector dir = (cosI - cosR)*pID->normalv - ratio * pID->eyev;
        ray refractRay(pID->underPt, dir);

        retColor = intersect(refractRay, (remaining -1), -1);
        retColor = transparent * retColor;

        return retColor;
    }
}

float world::schlick(pIntDataT pID)
{
    float cos = pID->eyev.dot(pID->normalv);

    if (pID->n1 > pID->n2)
    {
        float ratio = pID->n1 / pID->n2;
        float sin2_t = ratio * ratio*(1.0f - cos * cos);
        if (sin2_t > 1.0)
            return 1.0;
        else
        {
            float cos_t = (float)sqrt(1.0 - sin2_t);
            cos = cos_t;
        }
    }

    float r0 = (pID->n1 - pID->n2) / (pID->n1 + pID->n2);
    float r1 = r0 * r0;

    return r1 + (1 - r1)*pow(1 - cos, 5);
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
world::world(color dclr) : m_nObjCnt(0), m_pID(nullptr), m_defaultColor(dclr)
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
                os << "SPHERE:" << std::endl;
                os << *(s);
            }
            else if (PLANE == ((*iter).second)->getType())
            {
                plane* p = dynamic_cast<plane*>((*iter).second);
                os << "PLANE:" << std::endl;
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
// output functions


std::ostream& operator<<(std::ostream& os, const intDataT id)
{
    os << "precomputed intersection data:" << std::endl;
    os << "    time value  : " << id.t << std::endl;
    os << "    object id   : " << id.id << std::endl;
    os << "    inside shape: " << (id.inside ? "true" : "false") << std::endl;
    os << "    in a shadow : " << (id.inShadow ? "true" : "false") << std::endl;
    os << "    over point  : " << id.overPt << std::endl;
    os << "    world point : " << id.pt << std::endl;
    os << "    under point : " << id.underPt << std::endl;
    os << "    eye vector  : " << id.eyev << std::endl;
    os << "    normal vec  : " << id.normalv << std::endl;
    os << "    reflect vec : " << id.reflectv << std::endl;

    return os;
}

//float        n1;             // index of refraction on outside material
//float        n2;             // index of refraction on inside material


