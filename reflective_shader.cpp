#include "reflective_shader.h"
#include "ray.h"
#include "render_world.h"

vec3 Reflective_Shader::
Shade_Surface(const Ray& ray,const vec3& intersection_point,
    const vec3& normal,int recursion_depth) const {
    vec3 color = shader->Shade_Surface(ray, intersection_point,
        normal, recursion_depth);

    if (recursion_depth >= world.recursion_depth_limit) {
        return (1 - reflectivity) * color;
    }

    vec3 v = ray.direction;
    Ray r(intersection_point, v - 2 * dot(v, normal) * normal);

    return (1 - reflectivity) * color 
           + reflectivity * world.Cast_Ray(r, ++recursion_depth);
}
