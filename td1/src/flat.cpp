#include "integrator.h"
#include "scene.h"

class FlatIntegrator : public Integrator {
public:
    FlatIntegrator(const PropertyList& props) { }

    Color3f Li(const Scene* scene, const Ray& ray) const {
        Hit hit;

        scene->intersect(ray, hit);

        if (!hit.foundIntersection())
            return scene->backgroundColor();

        return hit.shape()->material()->ambientColor();
    }

    std::string toString() const {
        return "FlatIntegrator[]";
    }
};

REGISTER_CLASS(FlatIntegrator, "flat")
