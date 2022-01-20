#include "integrator.h"
#include "scene.h"

class DirectIntegrator : public Integrator {
public:
    DirectIntegrator(const PropertyList& props) { }

    Color3f Li(const Scene* scene, const Ray& ray) const {
        Hit hit;

        scene->intersect(ray, hit);

        if (!hit.foundIntersection())
            return scene->backgroundColor();

        Color3f color = Color3f(0.f);

        const Point3f intersection_point = ray.origin + ray.direction * hit.t();

        for (const Light* light : scene->lightList()) {
            const Ray lightRay(intersection_point + hit.normal() * 10e-4, light->direction(intersection_point).normalized());
            Hit lightHit;

            scene->intersect(lightRay, lightHit);
            if (lightHit.foundIntersection() && lightHit.shape())
                continue;

            const Color3f rho = hit.shape()->material()->brdf(-ray.direction, light->direction(intersection_point), hit.normal());
            color += rho * std::max(light->direction(intersection_point).dot(hit.normal()), 0.f) * light->intensity(intersection_point);
        }

        return color;
    }

    std::string toString() const
    {
        return "DirectIntegrator[]";
    }
};

REGISTER_CLASS(DirectIntegrator, "direct")
