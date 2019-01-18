#include "sphere.h"
#include "ray.h"

#include <cmath>

// Determine if the ray intersects with the sphere
Hit Sphere::Intersection(const Ray& ray, int part) const
{
    double t1;
    double t2;
    Hit ret;
    ret.object = NULL;
    ret.part = -1;

    vec3 u = ray.direction;
    vec3 w = ray.endpoint - center;
    double a = u.magnitude_squared();
    double b = 2 * dot(u, w);
    double c = w.magnitude_squared() - (radius * radius);
    double descrim = (b * b) - (4 * a * c);

    // The ray does not intersect
    if (descrim < 0) {
        return ret;
    }

    // Calculated an intersection
    t1 = (-b + sqrt(descrim)) / (2 * a);
    t2 = (-b - sqrt(descrim)) / (2 * a);
        
    // Determine which is the closer intersection
    // And whether it beats the threshold
    if (t1 < t2 && t1 >= small_t) {
        ret.dist = t1;
        ret.object = this;
    } else if (t2 <= t1 && t2 >= small_t) {
        ret.dist = t2;
        ret.object = this;
    }

    return ret;
}

vec3 Sphere::Normal(const vec3& point, int part) const
{
    vec3 normal;
    normal = point - center; // compute the normal direction
    return normal.normalized();
}

Box Sphere::Bounding_Box(int part) const
{
    Box box;
    TODO; // calculate bounding box
    return box;
}
