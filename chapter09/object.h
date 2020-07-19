#pragma once
/*
 * abstract class to represent an object in the scene 
 */

#include "pch.h"
#include "material.h"
#include "transformations.h"

typedef struct tag_int
{
    int getCnt() { return m_nCnt; }
    float t1() { return m_nt1; }
    float t2() { return m_nt2; }
    void clear() { m_nCnt = 0; m_nt1 = 0; m_nt2 = 0; }

    int   m_nID;              // ID of object that is hit
    int   m_nCnt;
    float m_nt1;
    float m_nt2;
} interInfo, * pInterInfo;

class ray;

enum OBJ_TYPE { TEST=-1, SPHERE = 0, CUBE, CONE, CYLINDER, PLANE};

class object
{
public:
    object(material* pm, unsigned short t);

    int getNextId() { return this->m_nCount++; }
    int getType() { return m_nType; }
    unsigned int getID() { return m_nID; }
    
    virtual matrix4x4 getXform() = 0;
    virtual void xform(matrix4x4) = 0;

    virtual material* getMat() = 0; 
    virtual void mat(material*) = 0;
    
    virtual bool intersect(const ray&, pInterInfo) = 0;
    virtual vector normalAt(point worldPt) = 0;
    
protected:
    unsigned int   m_nID;                  // object ID;
    material*      m_pMat;                 // objects material; 
    matrix4x4      m_xform;                // set to idenity matrix

private:
    unsigned short m_nType;                // type of object
    static int     m_nCount;
    
};

std::ostream& operator<<(std::ostream& os, const interInfo& info);


