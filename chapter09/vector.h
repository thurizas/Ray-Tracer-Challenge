#pragma once

class point;

class vector
{
public:
    vector();
    vector(float x, float y, float z, float w = 0);
    vector(const vector& rhs);
    vector& operator=(const vector& rhs);            // assignment operator

    bool isPoint() { return m_w != 0; }
    bool isVector() { return m_w == 0; }

    float  x() { return m_x; }
    void x(float v) { m_x = v; }
    float y() { return m_y; }
    void y(float v) { m_y = v; }
    float z() { return m_z; }
    void z(float v) { m_z = v; }
    const float w() { return m_w; }
    void w(float v) { m_w = v; }

    bool operator==(const vector& rhs);

    vector operator-();                   // negation operator
    point operator+(const point& rhs);
    vector operator+(const vector& rhs);
    vector operator-(const vector& rhs);
    vector operator*(const float);

    float norm();                     // magnitude
    float norm2();                    // square of the norm
    void normalize();                 // normalization, will modify object
    float dot(const vector& op2);     // dot product
    vector cross(const vector& op2);  // cross product
    vector reflect(const vector& n);  // reflect vector about a normal
    

private:
    float   m_x;
    float   m_y;
    float   m_z;
    float   m_w;   // must be 0 for a vector


};

// to allow f*vec
vector operator*(float f, const vector& vec);
std::ostream& operator<<(std::ostream&, const vector&);
