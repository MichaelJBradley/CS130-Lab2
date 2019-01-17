#include "plane.h"
#include "ray.h"
#include <cfloat>
#include <limits>

// Intersect with the half space defined by the plane.  The plane's normal
// points outside.  If the ray starts on the "inside" side of the plane, be sure
// to record a hit with t=0 as the first entry in hits.
Hit Plane::Intersection(const Ray& ray, int part) const
{
    double t;
    Hit ret;
    ret.object = NULL;
    ret.part = -1;

    vec3 w = x1 - ray.endpoint;
    double denom = dot(ray.direction, normal);

    // No intersection if the ray is perpendicular to the normal
    if (!denom) {
        return ret;
    } 

    t = dot(w, normal) / denom;

    // No intersection if t is below threshold
    if (t < small_t) {
        return ret;
    }

    // If we made it this far then we found an intersection
    ret.object = this;
    ret.dist = t;

    return ret;
}

vec3 Plane::Normal(const vec3& point, int part) const
{
    return normal;
}

// There is not a good answer for the bounding box of an infinite object.
// The safe thing to do is to return a box that contains everything.
Box Plane::Bounding_Box(int part) const
{
    Box b;
    b.hi.fill(std::numeric_limits<double>::max());
    b.lo=-b.hi;
    return b;
}
