#include "light.h"
#include "phong_shader.h"
#include "ray.h"
#include "render_world.h"
#include "object.h"

#include <cmath>

vec3 Phong_Shader::
Shade_Surface(const Ray& ray,const vec3& intersection_point,
    const vec3& normal,int recursion_depth) const
{
    vec3 color; 
    vec3 intense_amb; 
    vec3 intense_dif;
    vec3 intense_spec;

    // Use the world's ambient color and intensity to determine ambient
    // light only once
    intense_amb = color_ambient 
                * world.ambient_color
                * world.ambient_intensity;
    
    // iterate through world lights
    for (unsigned i = 0; i < world.lights.size(); i++) { 
        vec3 l = world.lights[i]->position - intersection_point;

        // check whether object is in shadow
        Ray to_light(intersection_point, l);
        Hit hit = world.Closest_Intersection(to_light);
        vec3 obj_to_hit = to_light.Point(hit.dist) - intersection_point;
        if (!world.enable_shadows || (!hit.object || 
            obj_to_hit.magnitude() > l.magnitude())) {

        vec3 r = (-l + 2 * dot(l, normal) * normal).normalized();

        intense_dif += color_diffuse 
                    * world.lights[i]->Emitted_Light(l)
                    * std::max(dot(normal, l.normalized()), 0.0);

        intense_spec += color_specular
                     * world.lights[i]->Emitted_Light(l)
                     * std::pow(std::max(dot(r, -(ray.direction)), 0.0),
                                specular_power);
        }
        
    }


    return intense_amb + intense_dif + intense_spec;
}
