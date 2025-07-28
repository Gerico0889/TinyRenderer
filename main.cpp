#include "gl.h"
#include "model.h"
#include "tgaimage.h"

int main(int argc, char** argv) {
    constexpr int width = 800;
    constexpr int height = 800;
    const Vec3 eye{-1, 0, 2};   // Camera position
    const Vec3 center{0, 0, 0}; // Camera direction
    const Vec3 up{0, 1, 0};     // Camera up vector

    TGAImage framebuffer(width, height, TGAImage::RGB);
    std::vector<double> zbuffer(width * height, -std::numeric_limits<double>::max());

    Model model;
    std::string file_name{"african_head/african_head.obj"};
    model.loadVerticesFromObj(file_name);
    model.loadFacesFromObjs(file_name);
    model.loadNormalsFromObj(file_name);

    lookAt(eye, center, up);
    perspective(norm(eye - center));
    viewport(width / 16, height / 16, width * 7 / 8, height * 7 / 8); // build the Viewport matrix

    auto const& vertices = model.getVertices();
    auto const& faces = model.getVertexFaces();
    auto const& normals = model.getNormals();

    for (const auto& face : faces) {
        Vec4 clip[3];
        Vec3 v0 = vertices[std::get<0>(face)];
        Vec3 v1 = vertices[std::get<1>(face)];
        Vec3 v2 = vertices[std::get<2>(face)];

        auto composed_matrix = Perspective * Modelview;

        clip[0] = composed_matrix * Vec4{v0.x, v0.y, v0.z, 1.};
        clip[1] = composed_matrix * Vec4{v1.x, v1.y, v1.z, 1.};
        clip[2] = composed_matrix * Vec4{v2.x, v2.y, v2.z, 1.};

        TGAColor rnd;
        for (int c = 0; c < 3; c++) {
            rnd[c] = std::rand() % 255;
        }
        rasterize(clip, zbuffer, framebuffer, rnd);
    }

    framebuffer.write_tga_file("framebuffer.tga");
    return 0;
}
