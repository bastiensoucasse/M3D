#include <Eigen/Geometry>
#include <filesystem/resolver.h>

#include "plane.h"
#include "scene.h"
#include "sphere.h"

Scene::Scene(const PropertyList &props)
{
    m_backgroundColor = props.getColor("background", Color3f(.6f));
}

void Scene::clear()
{
    m_shapeList.clear();
    m_lightList.clear();
    if (m_camera)
        delete m_camera;
    m_camera = nullptr;
    if (m_integrator)
        delete m_integrator;
    m_integrator = nullptr;
}

void Scene::intersect(const Ray &ray, Hit &hit) const
{
    Hit shapeHit;

    for (const Shape *shape : m_shapeList)
        if (shape->intersect(ray, shapeHit) && shapeHit.t() < hit.t())
            hit = shapeHit;
}

void Scene::addChild(Object *obj)
{
    switch (obj->getClassType())
    {
    case EShape:
    {
        Shape *shape = static_cast<Shape *>(obj);
        m_shapeList.push_back(shape);
    }
    break;

    case ELight:
    {
        Light *light = static_cast<Light *>(obj);
        m_lightList.push_back(light);
    }
    break;

    case ECamera:
        if (m_camera)
            throw RTException("There can only be one camera per scene!");
        m_camera = static_cast<Camera *>(obj);
        break;

    case EIntegrator:
        if (m_integrator)
            throw RTException("There can only be one integrator per scene!");
        m_integrator = static_cast<Integrator *>(obj);
        break;

    default:
        throw RTException("Scene::addChild(<%s>) is not supported!", classTypeName(obj->getClassType()));
    }
}

std::string Scene::toString() const
{
    std::string shapes;
    for (size_t i = 0; i < m_shapeList.size(); ++i)
    {
        shapes += std::string("  ") + indent(m_shapeList[i]->toString(), 2);
        if (i + 1 < m_shapeList.size())
            shapes += ",";
        shapes += "\n";
    }

    std::string lights;
    for (size_t i = 0; i < m_lightList.size(); ++i)
    {
        lights += std::string("  ") + indent(m_lightList[i]->toString(), 2);
        if (i + 1 < m_lightList.size())
            lights += ",";
        lights += "\n";
    }

    return tfm::format(
        "Scene[\n"
        "  background = %s,\n"
        "  integrator = %s,\n"
        "  camera = %s,\n"
        "  shapes = {\n"
        "  %s  }\n"
        "  lights = {\n"
        "  %s  }\n"
        "]",
        m_backgroundColor.toString(),
        indent(m_integrator->toString()),
        indent(m_camera->toString()),
        indent(shapes, 2),
        indent(lights, 2));
}

REGISTER_CLASS(Scene, "scene");
