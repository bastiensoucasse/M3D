#include "sphere.h"
#include <iostream>

Sphere::Sphere(float radius)
    : m_radius(radius)
{
}

Sphere::Sphere(const PropertyList &propList)
{
    m_radius = propList.getFloat("radius",1.f);
    m_center = propList.getPoint("center",Point3f(0,0,0));
}

Sphere::~Sphere()
{
}

bool Sphere::intersect(const Ray& ray, Hit& hit) const
{
    const float a = ray.direction.dot(ray.direction);
    const float b = 2. * ray.direction.dot(ray.origin - m_center);
    const float c = (ray.origin - m_center).squaredNorm() - pow(m_radius, 2);

    const float delta = pow(b, 2) - 4 * a * c;

    if (delta < 0)
        return false;

    float t;

    if (delta == 0)
        t = -b / (2 * a);

    if (delta > 0)
    {
        const float t1 = (-b - sqrt(delta)) / (2 * a);
        const float t2 = (-b + sqrt(delta)) / (2 * a);

        if (t2 <= 0)
            t = t1;
        else if (t1 <= 0)
            t = t2;
        else
            t = std::min(t1, t2);
    }

    if (t <= 0)
        return false;

    const Point3f intersection = ray.origin + ray.direction * t;
    const Normal3f normal = (intersection - m_center).normalized();

    hit.setShape(this);
    hit.setT(t);
    hit.setNormal(normal);

    return true;
}

REGISTER_CLASS(Sphere, "sphere")
