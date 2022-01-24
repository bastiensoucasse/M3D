#include "integrator.h"
#include "scene.h"

class DirectIntegrator : public Integrator
{
public:
    DirectIntegrator(const PropertyList &props)
    {
    }

    Color3f Li(const Scene *scene, const Ray &ray) const
    {
        Hit hit;
        scene->intersect(ray, hit);
        if (!hit.foundIntersection())
            return ray.recursionLevel == 0 ? scene->backgroundColor() : Color3f(0.f);

        Color3f color = Color3f(0.f);

        const Point3f intersection_point = ray.at(hit.t());

        for (const Light *light : scene->lightList())
        {
            float ldist;
            const Vector3f ldir = light->direction(intersection_point, (&ldist));
            const Ray lray(intersection_point + hit.normal() * 10e-4, ldir.normalized());

            Hit lhit;
            scene->intersect(lray, lhit);
            if (lhit.foundIntersection() && (ldist == std::numeric_limits<float>::max() || ldist > lhit.t()))
                continue;

            const Color3f rho = hit.shape()->material()->brdf(-ray.direction, ldir, hit.normal());
            color += rho * std::max(ldir.dot(hit.normal()), 0.f) * light->intensity(intersection_point);
        }

        return color;
    }

    std::string toString() const
    {
        return "DirectIntegrator[]";
    }
};

REGISTER_CLASS(DirectIntegrator, "direct");
