#include "pch.h"

#include "canvas.h"

#include <fstream>

#define MAKENDX(r,c,w) (c) + (r)*(w)


canvas::canvas(int w, int h) : m_width(w), m_height(h), m_pCanvas(nullptr)
{
    m_pCanvas = new color[m_width*m_height];

    for (int ndx = 0; ndx < m_width*m_height; ndx++)
        m_pCanvas[ndx] = color(0.0f, 0.0f, 0.0f);
        //m_pCanvas[ndx] = color(ndx, ndx + 1, ndx + 2);
}



canvas::canvas(int w, int h, color c) :m_width(w), m_height(h), m_pCanvas(nullptr)
{
    m_pCanvas = new color[m_width*m_height];

    for (int ndx = 0; ndx < m_width*m_height; ndx++)
        m_pCanvas[ndx] = c;

}


canvas::~canvas()
{
    if (nullptr != m_pCanvas)
    {
        delete[] m_pCanvas;
        m_pCanvas = nullptr;
    }
}


void canvas::print()
{
    for (int row = 0; row < m_height; row++)
    {
        for (int col = 0; col < m_width; col++)
        {
            std::cout << m_pCanvas[MAKENDX(row, col, m_width)];
        }
        std::cout << std::endl;
    }

}

                   //      x        y
void canvas::setColor(int col, int row, color c)
{
    if ((0 <= row) && (row < m_height))     
    {
        if ((0 <= col) && (col < m_width))
        {
            m_pCanvas[MAKENDX(row, col, m_width)] = c;
        }
        else
        {
            std::cerr << "y-coordinate out of bounds" << std::endl;
        }
    }
    else
    {
        std::cerr << "x-coordinate out of bounds" << std::endl;
    }
}

                    //      x        y
color canvas::getColor(int col, int row)
{
    color c(0.0f, 0.0f, 0.0f);
    if ((0 <= row) && (row < m_height))
    {
        if ((0 <= col) && (col < m_width))
        {
            c = m_pCanvas[MAKENDX(row, col, m_width)];
        }
        else
        {
            std::cerr << "y-coordinate out of bounds" << std::endl;
        }
    }
    else
    {
        std::cerr << "x-coordinate out of bounds" << std::endl;
    }

    return c;
}


void canvas::writePPM(const char* fn)
{
    if (nullptr != fn)
    {
        std::ofstream    outfs;
        outfs.open(fn, std::ofstream::out);

        // TODO : check if open was successful...

        // max component to be written is '255 ', i.e. 4 characters
        // in stream position is > 64 from start we need a new line.
        const int maxLine = 64;

        std::cout << "writing to file: " << fn;

        // write the header
        outfs << "P3" << std::endl;                        // write magic number...
        outfs << m_width << " " << m_height << std::endl;  // write dimensions...
        outfs << "255" << std::endl;                       // write max color size, should be configurable...
                                                           
        // write the pixel data....
        for (int row = 0; row < m_height; row++)
        {
            std::cout << "..";
            std::streamoff lnBegin = outfs.tellp();
            for (int col = 0; col < m_width; col++)
            {
                float fComp[3];
                int   nComp[3];
                fComp[0] = m_pCanvas[MAKENDX(row, col, m_width)].r();
                fComp[1] = m_pCanvas[MAKENDX(row, col, m_width)].g();
                fComp[2] = m_pCanvas[MAKENDX(row, col, m_width)].b();

                // convert data to int, and clamp to range [0,255]
                for (int ndx = 0; ndx < 3; ndx++)
                {
                    nComp[ndx] = (int)(255.0f * fComp[ndx]);
                    if (nComp[ndx] > 255) nComp[ndx] = 255;
                    if (nComp[ndx] < 0) nComp[ndx] = 0;
                }

                outfs << nComp[0]; 
                std::streamoff lnCurrent = outfs.tellp();
                if (lnCurrent - lnBegin > maxLine)
                {
                    outfs << std::endl;
                    lnBegin = outfs.tellp();
                }
                outfs << " " << nComp[1];
                lnCurrent = outfs.tellp();
                if (lnCurrent - lnBegin > maxLine)
                {
                    outfs << std::endl;
                    lnBegin = outfs.tellp();
                }
                outfs << " " << nComp[2];
                lnCurrent = outfs.tellp();
                if (lnCurrent - lnBegin > maxLine)
                {
                    outfs << std::endl;
                    lnBegin = outfs.tellp();
                }
                outfs << " ";
                lnCurrent = outfs.tellp();
                if (lnCurrent - lnBegin > maxLine)
                {
                    outfs << std::endl;            // move to next line....
                    lnBegin = outfs.tellp();       // reset the start pointer...
                }
            }
            outfs << std::endl;
        }

        outfs << std::endl;                                 // insure file ends with a new line
        outfs.close();
        std::cout << std::endl;
    }
    else
    {
        std::cerr << "file name parameter is null" << std::endl;
    }
}