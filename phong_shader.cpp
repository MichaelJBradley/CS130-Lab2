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
    intense_amb = (color_ambient * world.ambient_color)
                    * world.ambient_intensity;
    
    // iterate through world lights
    for (unsigned i = 0; i < world.lights.size(); i++) { 
        vec3 light_ray = world.lights[i]->position - intersection_point;
        // iterate through rgb
        for (unsigned c = 0; c < 3; c++) {
            vec3 r = -light_ray + 2 * dot(light_ray, normal) * normal; 
            intense_dif = color_diffuse 
                        * world.lights[i]->Emitted_Light(light_ray)
                        * std::max(dot(normal, light_ray), 0.0);

            intense_spec = color_specular
                         * world.lights[i]->Emitted_Light(light_ray)
                         * std::max(dot(normal, -(ray.direction)), 0.0);
        }
    }

    /* DEBUG MAKE SURE YOU CHANGE THIS BACK WHEN YOU'RE ALL DONE
    color = intense_amb + intense_dif + intense_spec;
    return color;
    /**/
    return intense_spec;
    /**/
}
