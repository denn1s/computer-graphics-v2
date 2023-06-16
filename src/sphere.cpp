#include "sphere.h"

Sphere::Sphere(glm::vec3 center, float radius) 
    : center(center), radius(radius) {}

bool Sphere::rayIntersect(const glm::vec3& rayOrigin, const glm::vec3& rayDirection) const {
    // oc is the vector from the ray's origin to the sphere's center
    glm::vec3 oc = rayOrigin - center;

    // The coefficients a, b, and c for the quadratic equation
    // These are derived from the equation for a sphere and the parametric equation for a line (ray)
    // a corresponds to the direction of the ray
    float a = glm::dot(rayDirection, rayDirection);

    // b is two times the dot product of the direction of the ray and the vector from the ray's origin to the sphere's center
    float b = 2.0f * glm::dot(oc, rayDirection);

    // c is the dot product of oc with itself minus the radius of the sphere squared
    float c = glm::dot(oc, oc) - radius * radius;

    // The discriminant determines how many solutions there are to the quadratic equation
    // If the discriminant is less than 0, there are no real solutions, and the ray does not intersect the sphere
    // If the discriminant is 0, there is one solution, and the ray intersects the sphere once (i.e., it is tangent to the sphere's surface)
    // If the discriminant is greater than 0, there are two solutions, and the ray intersects the sphere twice (i.e., it enters and then exits the sphere)
    float discriminant = b * b - 4 * a * c;

    // return true if the discriminant is greater than 0, indicating the ray intersects the sphere
    return discriminant > 0;
}
