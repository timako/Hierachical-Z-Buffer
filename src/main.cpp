#include <iostream>
#include "model.h"
#include "shader.h"
#include "camera.h"
#include "light.h"
#include "renderer.h"

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << "Usage: project <path_to_obj_file> <output_image.bmp>" << std::endl;
        return 1;
    }

    std::string objFile = argv[1];
    std::string outputImage = argv[2];
    Model model;

    if (!model.loadFromOBJ(objFile)) {
        std::cerr << "Failed to load OBJ file." << std::endl;
        return 1;
    }
    model.normalizeToUnitCube();
    // Define light
    Light light(Vec3f(1.0f, 1.0f, 1.0f), Vec3f(1.0f, 1.0f, 1.0f));

    // Define shader with material properties
    Shader shader(light,
                  Vec3f(0.1f, 0.1f, 0.1f), // Ambient
                  Vec3f(0.5f, 0.5f, 0.5f), // Diffuse
                  Vec3f(1.0f, 1.0f, 1.0f), // Specular
                  32.0f);                  // Shininess

    // Define camera
    Camera camera(
        Vec3f(1.5f, 1.5f, 1.5f), // Position
        model.center, // Target
        Vec3f(0.0f, 1.0f, 0.0f), // Up vector
        60.0f,                    // FOV
        1.3333f,                  // Aspect ratio (4:3)
        0.1f,                     // Near plane
        100.0f                    // Far plane
    );

    // Define renderer with desired image size
    int width = 2400;
    int height = 1800;
    Renderer renderer(width, height, shader, camera);

    // Render the model
    renderer.render(model);

    // Save the framebuffer to an image
    renderer.framebuffer.saveToBMP(outputImage);

    std::cout << "Rendering complete. Image saved to " << outputImage << std::endl;

    return 0;
}
