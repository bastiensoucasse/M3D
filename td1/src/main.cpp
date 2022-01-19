#include <filesystem/resolver.h>

#include "viewer.h"

void render(Scene* scene, ImageBlock* result, std::string outputName, bool* done) {
    if (!scene)
        return;

    clock_t t = clock();

    const Camera* camera = scene->camera();
    const Integrator* integrator = scene->integrator();
    integrator->preprocess(scene);

    float tanfovy2 = tan(camera->fovY() * .5f);
    Vector3f camX = camera->right() * tanfovy2 * camera->nearDist() * float(camera->vpWidth()) / float(camera->vpHeight());
    Vector3f camY = -camera->up() * tanfovy2 * camera->nearDist();
    Vector3f camF = camera->direction() * camera->nearDist();

    const unsigned int width = camera->vpWidth();
    const unsigned int height = camera->vpHeight();

    const Point3f origin = camera->position();

    for (float y = 0.f; y < height; y++)
        for (float x = 0.f; x < width; x++) {
            const Vector3f direction = camF + 2.f * (x / width - .5f) * camX + 2.f * (y / height - .5f) * camY;
            const Ray ray(origin, direction.normalized());
            const Color3f color = integrator->Li(scene, ray);
            result->put(Vector2f(x, y), color);
        }

    t = clock() - t;
    std::cout << "Raytracing time: " << float(t) / CLOCKS_PER_SEC << "s" << std::endl;

    *done = true;
}

int main(const int argc, const char** argv) {
    getFileResolver()->prepend(DATA_DIR);

    try {
        nanogui::init();
        Viewer* screen = new Viewer();

        if (argc == 2) {
            filesystem::path path(argv[1]);

            if (path.extension() == "scn")
                screen->loadScene(argv[1]);
            else if (path.extension() == "exr")
                screen->loadImage(argv[1]);
        }

        nanogui::mainloop();

        delete screen;
        nanogui::shutdown();
    } catch (const std::exception& e) {
        cerr << "Fatal error: " << e.what() << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
