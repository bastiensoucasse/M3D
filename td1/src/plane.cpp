#include "plane.h"

Plane::Plane() {}

Plane::Plane(const PropertyList &propList)
{
    m_position = propList.getPoint("position", Point3f(0.f));
    m_normal = propList.getVector("normal", Point3f(0.f, 0.f, 1.f));
}

Plane::~Plane() {}

bool Plane::intersect(const Ray &ray, Hit &hit) const
{
    const float distance = m_position.dot(m_normal);
    const float top = distance - ray.origin.dot(m_normal);
    const float bottom = ray.direction.dot(m_normal);

    if (top == 0) // infinity
        return false;

    if (bottom == 0) // undefined
        return false;

    const float t = top / bottom;

    if (t <= 0)
        return false;

    hit.setShape(this);
    hit.setT(t);
    hit.setNormal(m_normal);

    return true;
}

REGISTER_CLASS(Plane, "plane");
