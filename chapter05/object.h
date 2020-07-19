#pragma once
/*
 * abstract class to represent an object in the scene 
 */

#include "pch.h"

typedef struct tag_int
{
    int getCnt() { return m_nCnt; }
    float t1() { return m_nt1; }
    float t2() { return m_nt2; }

    int   m_nID;              // ID of object that is hit
    int   m_nCnt;
    float m_nt1;
    float m_nt2;
} interInfo, * pInterInfo;

class ray;

enum OBJ_TYPE { SPHERE = 0, CUBE, CONE, CYLINDER, SURFACE};
class object
{
public:
    object(unsigned short t);

    virtual bool intersect(const ray&, pInterInfo) = 0;
    int getNextId() { return this->m_nCount++; }

protected:
    unsigned int   m_nID;                  // object ID;

private:
    unsigned short m_nType;                // type of object
    static int     m_nCount;
    
};

std::ostream& operator<<(std::ostream& os, const interInfo& info);


