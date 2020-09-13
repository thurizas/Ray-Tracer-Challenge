#pragma once

class vector;

class point
{
public:
    point();
    point(float x, float y, float z, float w = 1);
    point(const point& rhs);                       // copy constucture
    point& operator=(const point& rhs);            // assignment operator

    bool isPoint() { return m_w == 1; }
    bool isVector() { return m_w != 1; }

    const float  x() { return m_x; }
    void x(float v) { m_x = v; }
    const float y() { return m_y; }
    void y(float v) {m_y = v;}
    const float z() { return m_z; }
    void z(float v) { m_z = v; }
    const float w() { return m_w; }
    void w(float v) { m_w = v; }

    bool operator==(const point& rhs);
    point operator-();                   //uniary negation operator
    point operator+(const vector& rhs);
    point operator-(const vector& rhs);
    vector operator-(const point& rhs);


private:
    float m_x;
    float m_y;
    float m_z;
    float m_w;   // must be 1 for a point 
};


std::ostream& operator<<(std::ostream&, const point&);
