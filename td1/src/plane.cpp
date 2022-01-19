#include "plane.h"

Plane::Plane()
{
}

Plane::Plane(const PropertyList &propList)
{
    m_position = propList.getPoint("position",Point3f(0,0,0));
    m_normal = propList.getVector("normal",Point3f(0,0,1));
}

Plane::~Plane()
{
}

bool Plane::intersect(const Ray& ray, Hit& hit) const
{
    if ((m_position - ray.origin).dot(m_normal) == 0 || ray.direction.dot(m_normal) == 0)
        return false;

    const float t = (m_position - ray.origin).dot(m_normal) / ray.direction.dot(m_normal);

    if (t <= 0)
        return false;

    hit.setShape(this);
    hit.setT(t);
    hit.setNormal(m_normal);

    return true;
}

REGISTER_CLASS(Plane, "plane")
