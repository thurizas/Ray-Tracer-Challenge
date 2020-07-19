#pragma once

#include "pch.h"
#include "color.h"

class canvas
{
public:
    canvas(int, int);
    canvas(int, int, color);
    ~canvas();

    int width() { return m_width; }
    int height(){ return m_height; }

    void print();
    void setColor(int, int, color);
    void writePPM(const char*);

private:
    canvas() {}                        // defalut construct set to private to force public ctors
    canvas(const canvas&) {}           // copy constuctor hidden to prevent use
    canvas& operator=(const canvas&) {}// assignment constructor hidden to preven use

    int     m_width;
    int     m_height;

    color*  m_pCanvas;

};