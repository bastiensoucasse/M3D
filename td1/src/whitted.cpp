#include "integrator.h"
#include "scene.h"

class WhittedIntegrator : public Integrator
{
private:
    const int m_maxRecursion;

public:
    WhittedIntegrator(const PropertyList &props) : m_maxRecursion{props.getInteger("maxRecursion", 0)}
    {
    }

    Color3f Li(const Scene *scene, const Ray &ray) const
    {
        Hit hit;

        scene->intersect(ray, hit);

        if (!hit.foundIntersection())
            return scene->backgroundColor();

        Color3f color = Color3f(0.f);

        const Point3f intersection_point = ray.at(hit.t());

        for (const Light *light : scene->lightList())
        {
            const Ray lightRay(intersection_point + hit.normal() * 10e-4, light->direction(intersection_point).normalized());
            Hit lightHit;

            scene->intersect(lightRay, lightHit);
            if (lightHit.foundIntersection() && lightHit.shape())
                continue;

            const Color3f rho = hit.shape()->material()->brdf(-ray.direction, light->direction(intersection_point), hit.normal());
            color += rho * std::max(light->direction(intersection_point).dot(hit.normal()), 0.f) * light->intensity(intersection_point);
        }

        if (ray.recursionLevel < m_maxRecursion)
        {
            const Vector3f r = 2 * hit.normal().dot(-ray.direction) * hit.normal() + ray.direction;
            Ray newRay(intersection_point + hit.normal() * 10e-4, r.normalized());
            newRay.recursionLevel = ray.recursionLevel + 1;
            color += Li(scene, newRay) * std::max(r.dot(hit.normal()), 0.f) * hit.shape()->material()->reflectivity();
        }

        return color;
    }

    std::string toString() const
    {
        return "WhittedIntegrator[]";
    }
};

REGISTER_CLASS(WhittedIntegrator, "whitted");
