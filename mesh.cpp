#include "mesh.h"
#include <fstream>
#include <string>
#include <limits>

#include "plane.h"
#include "globals.h"

#include <iostream>

// Consider a triangle to intersect a ray if the ray intersects the plane of the
// triangle with barycentric weights in [-weight_tolerance, 1+weight_tolerance]
static const double weight_tolerance = 1e-4;

// Read in a mesh from an obj file.  Populates the bounding box and registers
// one part per triangle (by setting number_parts).
void Mesh::Read_Obj(const char* file)
{
    std::ifstream fin(file);
    if(!fin)
    {
        exit(EXIT_FAILURE);
    }
    std::string line;
    ivec3 e;
    vec3 v;
    box.Make_Empty();
    while(fin)
    {
        getline(fin,line);

        if(sscanf(line.c_str(), "v %lg %lg %lg", &v[0], &v[1], &v[2]) == 3)
        {
            vertices.push_back(v);
            box.Include_Point(v);
        }

        if(sscanf(line.c_str(), "f %d %d %d", &e[0], &e[1], &e[2]) == 3)
        {
            for(int i=0;i<3;i++) e[i]--;
            triangles.push_back(e);
        }
    }
    number_parts=triangles.size();
}

// Check for an intersection against the ray.  See the base class for details.
Hit Mesh::Intersection(const Ray& ray, int part) const
{
    Hit ret;
    ret.object = NULL;
    ret.dist = 0;
    ret.part = -1;

    if (part >= 0) {
        if (Intersect_Triangle(ray, part, ret.dist)) {
            ret.object = this;
            ret.part = part;
        }
        
    } else {
        // naive solution
        ret.dist = std::numeric_limits<double>::max();

        for (unsigned i = 0; i < triangles.size(); i++) {
            double temp_dist;
            if (Intersect_Triangle(ray, i, temp_dist)) {
                if (temp_dist < ret.dist) {
                    ret.object = this;
                    ret.dist = temp_dist;
                    ret.part = i;
                }
            }
        }        
    }

    return ret;
}

// Compute the normal direction for the triangle with index part.
vec3 Mesh::Normal(const vec3& point, int part) const
{
    assert(part>=0);
    // Get the specified triangle represented by 3 vertices (a, b, c)
    ivec3 tri = triangles[part];
    
    // The normal = u x v, where:
    // u is the vector between vertices b (index 1) and a (index 0)
    vec3 u = vertices[tri[B]] - vertices[tri[A]];
    // v is the vector between vertices c (index 2) and a (index 0)
    vec3 v = vertices[tri[C]] - vertices[tri[A]];

    //Don't forget to normalize the result!
    return cross(u, v).normalized();
}

// This is a helper routine whose purpose is to simplify the implementation
// of the Intersection routine.  It should test for an intersection between
// the ray and the triangle with index tri.  If an intersection exists,
// record the distance and return true.  Otherwise, return false.
// This intersection should be computed by determining the intersection of
// the ray and the plane of the triangle.  From this, determine (1) where
// along the ray the intersection point occurs (dist) and (2) the barycentric
// coordinates within the triangle where the intersection occurs.  The
// triangle intersects the ray if dist>small_t and the barycentric weights are
// larger than -weight_tolerance.  The use of small_t avoid the self-shadowing
// bug, and the use of weight_tolerance prevents rays from passing in between
// two triangles.
bool Mesh::Intersect_Triangle(const Ray& ray, int tri, double& dist) const
{
    // Triangle vertices
    vec3 tri_a = vertices[triangles[tri][A]];
    vec3 tri_b = vertices[triangles[tri][B]];
    vec3 tri_c = vertices[triangles[tri][C]];
    vec3 intersect;
    
    Hit hit = Plane(tri_a, Normal(tri_a, tri)).Intersection(ray, tri);

    if (!hit.object) {
        return false;
    }
    
    intersect = ray.Point(dist);

    // Temporary vectors to condense math later
    vec3 u = ray.direction;
    vec3 v = tri_b - tri_a;
    vec3 w = tri_c - tri_a;
    vec3 y = intersect - tri_a;
    
    // Denominator is the same for each weight calc
    double denom = dot(cross(u, v), w);
    
    if (!denom) {
        return false;
    }

    // calc barycentric weights
    double gamma = dot(cross(u, v), y) / denom;
    double beta = dot(cross(w, u), y) / denom;
    double alpha = 1 - (gamma + beta);

    // compare against weight_tol to prevent ray going between triangles
    if (gamma > -weight_tol && beta > -weight_tol && alpha > -weight_tol) {
        dist = hit.dist;
        return true;
    }
    
    return false;
}

// Compute the bounding box.  Return the bounding box of only the triangle whose
// index is part.
Box Mesh::Bounding_Box(int part) const
{
    Box b;
    TODO;
    return b;
}
