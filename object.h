#pragma once
/*
 * abstract class to represent an object in the scene 
 */

#include "pch.h"
#include "material.h"
#include "vector.h"

#include <vector>

/*
 * structure: interInfo
 *
 * abstract : Used to record the intersection points of a ray with an object.  Will support up to two
 *            points of intersection with a given object.  This structure is used in the following 
 *            places: 
 *               virtual bool object::intersect(const ray&, pInterInfo)=0;
 *               bool sphere::intersect(const ray&, pInterInfo);
 *               bool plane::intersect(const ray& r, pInterInfo pInter)
 *               bool ray::intersect(const ray&, pInterInfo);
 *               interInfo ray::intersect(object*);
 *               std::ostream& operator<<(std::ostream& os, const interInfo& info);
 */
typedef struct tag_int
{
    int    getCnt() { return m_nCnt; }
    float  t1() { return m_nt1; }
    float  t2() { return m_nt2; }
    vector reflectv(){ return m_reflectv; }
    void clear() { m_nCnt = 0; m_nt1 = 0; m_nt2 = 0; }

    int    m_nID;              // ID of object that is hit
    int    m_nCnt;
    float  m_nt1;
    float  m_nt2;
    vector m_reflectv;
} interInfo, * pInterInfo;

class ray;
class material;

enum OBJ_TYPE { TEST=-1, SPHERE = 0, PLANE=1, CUBE=2, CONE=3, CYLINDER=4 };

class object
{
public:
    object(material* pm, unsigned short t);
    virtual ~object();

    //int getNextId() { return this->m_nCount++; }
    void setID(int id) { m_nID = id; }
    int getType() { return m_nType; }
    unsigned int getID() { return m_nID; }
    
    virtual matrix4x4 xform() = 0;
    virtual void xform(matrix4x4) = 0;

    virtual material* getMat() = 0; 
    virtual void mat(material*) = 0;
    
    virtual bool intersect(const ray&, pInterInfo)=0;
    virtual vector normalAt(point worldPt) = 0;
    
protected:
    unsigned int   m_nID;                  // object ID;
    material*      m_pMat;                 // objects material; 
    //matrix4x4      m_xform;                // set to idenity matrix

private:
    unsigned short m_nType;                // type of object
    static int     m_nCount;               // sets the order that the objects are created in.
    
};

std::ostream& operator<<(std::ostream& os, const interInfo& info);


