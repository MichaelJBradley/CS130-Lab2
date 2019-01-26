#include "light.h"
#include "phong_shader.h"
#include "ray.h"
#include "render_world.h"
#include "object.h"

vec3 Phong_Shader::
Shade_Surface(const Ray& ray,const vec3& intersection_point,
    const vec3& normal,int recursion_depth) const
{
    vec3 color; 
    vec3 intense_amb; 
    vec3 intense_dif;
    vec3 intense_spec;
    TODO; //determine the color

    // Use the world's ambient color and intensity to determine ambient
    // light only once
    intense_amb = color_ambient * ambient_color[c] * ambient_intensity;
    
    for (unsigned i = 0; i < lights.size(); i++) { // iterate through lights
        for (unsigned c = 0; c < color.size(); c++) { // iterate through rgb
        }
    }

    /* DEBUG MAKE SURE YOU CHANGE THIS BACK WHEN YOU'RE ALL DONE
    color = intense_amb + intense_dif + intense_spec;
    return color;
    /**/
    return intense_amb;
    /**/
}
