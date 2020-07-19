#include "pch.h"

#include "light.h"

// implements phong shading
color light::lighting(material m, point pt, vector eyev, vector normalv, bool inShadow)
{
    color ambient;
    color diffuse;
    color specular;

    color effColor = m.c() * i();                // calculate effective color

    vector lightv = p() - pt;                    // calculate direction to light source
    lightv.normalize();

    ambient = effColor * m.a();                    // calculate ambient contribution

    float f1 = lightv.dot(normalv);
    if (f1 < 0)
    {
        diffuse = color(0.0f, 0.0f, 0.0f);         // no contribution to diffuse
        specular = color(0.0f, 0.0f, 0.0f);        // no contribution to specular
    }
    else
    {
        diffuse = effColor * m.d()*f1;             // calculate diffuse contribution

        vector v2 = -lightv.reflect(normalv);
        float f2 = v2.dot(eyev);                   // cosine between reflected light & eye vector

        if (f2 <= 0)
            specular = color(0.0f, 0.0f, 0.0f);    // no contribution to specular
        else
        {
            float factor = pow(f2, m.sh());
            specular = i() * m.sp() * factor;    // contribution to specular
        }
    }

    if (inShadow)
        return ambient;
    else
        return (ambient + diffuse + specular);
}


std::ostream& operator<<(std::ostream& os, const light& l)
{
    os << "light, intensity: " << const_cast<light&>(l).i();
    os << " location: " << const_cast<light&>(l).p() << std::endl;
    return os;
}