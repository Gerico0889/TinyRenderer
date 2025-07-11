#include "matrix.h"
#include "model.h"
#include "tgaimage.h"
#include "vector.h"

#include <algorithm>
#include <cmath>

constexpr TGAColor white = {{255, 255, 255, 255}}; // attention, BGRA order
constexpr TGAColor green = {{0, 255, 0, 255}};
constexpr TGAColor red = {{0, 0, 255, 255}};
constexpr TGAColor blue = {{255, 128, 64, 255}};
constexpr TGAColor yellow = {{0, 200, 255, 255}};

Vec3 normalize_to_viewport(const Vec3& vertex, const int width, const int height) {
    if (vertex.x() < -1 || vertex.x() > 1 || vertex.y() < -1 || vertex.y() > 1 || vertex.z() < -1 || vertex.z() > 1) {
        return vertex;
    }

    float const normalized_x = (vertex.x() + 1) * 0.5 * width;
    float const normalized_y = (vertex.y() + 1) * 0.5 * width;
    float const normalized_z = (vertex.z() + 1) * 0.5 * 255;

    return Vec3(normalized_x, normalized_y, normalized_z);
}

std::pair<Vec3, Vec3> findBoundingBox(const Vec3& vertex1, const Vec3& vertex2, const Vec3& vertex3) {
    auto const min_x = std::min({vertex1.x(), vertex2.x(), vertex3.x()});
    auto const min_y = std::min({vertex1.y(), vertex2.y(), vertex3.y()});
    auto const max_x = std::max({vertex1.x(), vertex2.x(), vertex3.x()});
    auto const max_y = std::max({vertex1.y(), vertex2.y(), vertex3.y()});

    return {Vec3(min_x, min_y, 0), Vec3(max_x, max_y, 0)};
}

double signedTriangleArea(const Vec3& vertex1, const Vec3& vertex2, const Vec3& vertex3) {
    const auto ax = vertex1.x();
    const auto ay = vertex1.y();
    const auto bx = vertex2.x();
    const auto by = vertex2.y();
    const auto cx = vertex3.x();
    const auto cy = vertex3.y();

    return 0.5 * ((by - ay) * (bx + ax) + (cy - by) * (cx + bx) + (ay - cy) * (ax + cx));
}

Vec3 rotate(Vec3 vector) {
    const double angle = M_PI / 6;
    const Matrix<3, 3> y_rotation(std::cos(angle), 0, std::sin(angle),
                                  0, 1, 0,
                                  -std::sin(angle), 0, std::cos(angle));
    return y_rotation * vector;
}

Vec3 project(Vec3 vector, int width, int height) {
    return {
        (vector.x() + 1.) * width / 2,
        (vector.y() + 1.) * height / 2,
        (vector.z() + 1.) * 255. / 2};
};

void drawTriangle(const Vec3& vertex1, const Vec3& vertex2, const Vec3& vertex3, TGAImage& framebuffer, TGAImage& zbuffer, const TGAColor& color) {
    auto const normalized_vertex1 = normalize_to_viewport(vertex1, framebuffer.width(), framebuffer.height());
    auto const normalized_vertex2 = normalize_to_viewport(vertex2, framebuffer.width(), framebuffer.height());
    auto const normalized_vertex3 = normalize_to_viewport(vertex3, framebuffer.width(), framebuffer.height());

    const auto [vertex_min, vertex_max] = findBoundingBox(normalized_vertex1, normalized_vertex2, normalized_vertex3);
    const auto total_area = signedTriangleArea(normalized_vertex1, normalized_vertex2, normalized_vertex3);

    if (total_area < 0) {
        return;
    }

    for (int x = vertex_min.x(); x <= vertex_max.x(); ++x) {
        for (int y = vertex_min.y(); y <= vertex_max.y(); ++y) {
            const auto alpha = signedTriangleArea(Vec3(x, y, 0), normalized_vertex2, normalized_vertex3) / total_area;
            const auto beta = signedTriangleArea(Vec3(x, y, 0), normalized_vertex3, normalized_vertex1) / total_area;
            const auto gamma = signedTriangleArea(Vec3(x, y, 0), normalized_vertex1, normalized_vertex2) / total_area;

            const unsigned char z = static_cast<unsigned char>(normalized_vertex1.z() * alpha + normalized_vertex2.z() * beta + normalized_vertex3.z() * gamma);

            if (alpha < 0 || beta < 0 || gamma < 0) {
                continue;
            }

            if (z < zbuffer.get(x, y)[0]) {
                continue;
            }

            framebuffer.set(x, y, color);
            zbuffer.set(x, y, {{z}});
        }
    }
}

int main(int argc, char** argv) {
    constexpr int width = 2000;
    constexpr int height = 2000;
    TGAImage framebuffer(width, height, TGAImage::RGB);
    TGAImage zbuffer(width, height, TGAImage::GRAYSCALE);

    Model model;
    std::string file_name{"african_head/african_head.obj"};
    model.loadVerticesFromObj(file_name);
    model.loadFacesFromObjs(file_name);

    auto const& vertices = model.getVertices();
    auto const& faces = model.getFaces();
    for (auto const& indices : faces) {
        auto const index1 = std::get<0>(indices);
        auto const index2 = std::get<1>(indices);
        auto const index3 = std::get<2>(indices);

        auto const vertex1 = project(rotate(vertices[index1]), width, height);
        auto const vertex2 = project(rotate(vertices[index2]), width, height);
        auto const vertex3 = project(rotate(vertices[index3]), width, height);

        TGAColor random_color;
        for (int c = 0; c < 3; c++)
            random_color[c] = std::rand() % 255;

        drawTriangle(vertex1, vertex2, vertex3, framebuffer, zbuffer, random_color);
    }

    // drawTriangle(Vec3(170, 40, 255), Vec3(550, 390, 255), Vec3(230, 590, 255), framebuffer, red);

    framebuffer.write_tga_file("framebuffer.tga");
    // zbuffer.write_tga_file("zbuffer.tga");
    return 0;
}
