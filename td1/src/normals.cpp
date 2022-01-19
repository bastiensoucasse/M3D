#include "integrator.h"
#include "scene.h"

class NormalsIntegrator : public Integrator {
public:
    NormalsIntegrator(const PropertyList& props) { }

    Color3f Li(const Scene* scene, const Ray& ray) const {
        Hit hit;

        scene->intersect(ray, hit);

        if (!hit.foundIntersection())
            return scene->backgroundColor();

        return Color3f(abs(hit.normal().x()), abs(hit.normal().y()), abs(hit.normal().z()));
    }

    std::string toString() const {
        return "NormalsIntegrator[]";
    }
};

REGISTER_CLASS(NormalsIntegrator, "normals")
