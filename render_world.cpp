#include "flat_shader.h"
#include "object.h"
#include "light.h"
#include "ray.h"

#include <limits>

extern bool disable_hierarchy;

Render_World::Render_World()
    :background_shader(0),ambient_intensity(0),enable_shadows(true),
    recursion_depth_limit(3)
{}

Render_World::~Render_World()
{
    delete background_shader;
    for(size_t i=0;i<objects.size();i++) delete objects[i];
    for(size_t i=0;i<lights.size();i++) delete lights[i];
}

// Find and return the Hit structure for the closest intersection.  Be careful
// to ensure that hit.dist>=small_t.
Hit Render_World::Closest_Intersection(const Ray& ray)
{
    double min_t = std::numeric_limits<double>::max();
    Hit closest = {NULL, 0, 0};

    for (unsigned i = 0; i < objects.size(); i++) {
        Hit h = objects[i]->Intersection(ray, -1);

        // If the ray hit an object and it is the closest
        if ((h.object && h.dist >= small_t) && h.dist < min_t) {
            min_t = h.dist;
            closest = h;
        }
    }

    return closest;
}

// set up the initial view ray and call
void Render_World::Render_Pixel(const ivec2& pixel_index)
{
    // Calculate the direction vector from camera to the pixel
    vec3 dir = camera.World_Position(pixel_index) - camera.position;
    // Create the ray from camera with direction vector
    // Ray ctor normalizes the vector for us.
    Ray ray(camera.position, dir);
    vec3 color=Cast_Ray(ray, 1);
    camera.Set_Pixel(pixel_index,Pixel_Color(color));
}

void Render_World::Render()
{
    if(!disable_hierarchy)
        Initialize_Hierarchy();

    for(int j=0;j<camera.number_pixels[1];j++)
        for(int i=0;i<camera.number_pixels[0];i++)
            Render_Pixel(ivec2(i,j));
}

// cast ray and return the color of the closest intersected surface point,
// or the background color if there is no object intersection
vec3 Render_World::Cast_Ray(const Ray& ray,int recursion_depth)
{
    vec3 color;
    Hit closest = Closest_Intersection(ray);
    vec3 inter = ray.Point(closest.dist);
    const Object * o = closest.object;
    
    if (o) {
        color = o->material_shader->Shade_Surface(ray, inter, 
            o->Normal(inter, closest.part), recursion_depth);

    } else {
        // background_shader is a flat shader, so its parameters don't
        // matter
        color = background_shader->Shade_Surface(ray, color, color, 0); 
    }

    return color;
}

void Render_World::Initialize_Hierarchy()
{
    TODO; // Fill in hierarchy.entries; there should be one entry for
    // each part of each object.

    hierarchy.Reorder_Entries();
    hierarchy.Build_Tree();
}
