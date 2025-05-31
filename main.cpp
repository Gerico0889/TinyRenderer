#include "model.h"
#include "tgaimage.h"

#include <algorithm>
#include <cmath>
#include <iostream>

constexpr TGAColor white = {255, 255, 255, 255}; // attention, BGRA order
constexpr TGAColor green = {0, 255, 0, 255};
constexpr TGAColor red = {0, 0, 255, 255};
constexpr TGAColor blue = {255, 128, 64, 255};
constexpr TGAColor yellow = {0, 200, 255, 255};

std::pair<Vertex, Vertex> findBoundingBox(const Vertex& vertex1, const Vertex& vertex2, const Vertex& vertex3) {
    auto const min_x = std::min({vertex1.getX(), vertex2.getX(), vertex3.getX()});
    auto const min_y = std::min({vertex1.getY(), vertex2.getY(), vertex3.getY()});
    auto const max_x = std::max({vertex1.getX(), vertex2.getX(), vertex3.getX()});
    auto const max_y = std::max({vertex1.getY(), vertex2.getY(), vertex3.getY()});

    return {Vertex(min_x, min_y, 0), Vertex(max_x, max_y, 0)};
}

double signedTriangleArea(const Vertex& vertex1, const Vertex& vertex2, const Vertex& vertex3) {
    const auto ax = vertex1.getX();
    const auto ay = vertex1.getY();
    const auto bx = vertex2.getX();
    const auto by = vertex2.getY();
    const auto cx = vertex3.getX();
    const auto cy = vertex3.getY();

    return 0.5 * ((by - ay) * (bx + ax) + (cy - by) * (cx + bx) + (ay - cy) * (ax + cx));
}

void drawLine(const Vertex& vertex1, const Vertex& vertex2, TGAImage& framebuffer, const TGAColor& color) {
    auto const normalized_vertex1 = Vertex::normalize_to_viewport(vertex1, framebuffer.width(), framebuffer.height());
    auto const normalized_vertex2 = Vertex::normalize_to_viewport(vertex2, framebuffer.width(), framebuffer.height());

    auto ax = normalized_vertex1.getX();
    auto ay = normalized_vertex1.getY();
    auto bx = normalized_vertex2.getX();
    auto by = normalized_vertex2.getY();

    bool steep = std::abs(ay - by) > std::abs(ax - bx);
    if (steep) {
        std::swap(ax, ay);
        std::swap(bx, by);
    }

    if (ax > bx) {
        std::swap(ax, bx);
        std::swap(ay, by);
    }

    for (int x = ax; x <= bx; ++x) {
        float t = (x - ax) / static_cast<float>(bx - ax);
        int y = std::round(ay + (by - ay) * t);
        if (steep) {
            framebuffer.set(y, x, color);
        } else {
            framebuffer.set(x, y, color);
        }
    }
}

void drawTriangle(const Vertex& vertex1, const Vertex& vertex2, const Vertex& vertex3, TGAImage& framebuffer, const TGAColor& color) {
    auto const normalized_vertex1 = Vertex::normalize_to_viewport(vertex1, framebuffer.width(), framebuffer.height());
    auto const normalized_vertex2 = Vertex::normalize_to_viewport(vertex2, framebuffer.width(), framebuffer.height());
    auto const normalized_vertex3 = Vertex::normalize_to_viewport(vertex3, framebuffer.width(), framebuffer.height());

    const auto [vertex_min, vertex_max] = findBoundingBox(normalized_vertex1, normalized_vertex2, normalized_vertex3);
    const auto total_area = signedTriangleArea(normalized_vertex1, normalized_vertex2, normalized_vertex3);

    if (total_area < 0) {
        return;
    }

    for (int x = vertex_min.getX(); x <= vertex_max.getX(); ++x) {
        for (int y = vertex_min.getY(); y <= vertex_max.getY(); ++y) {
            const auto alpha = signedTriangleArea(Vertex(x, y, 0), normalized_vertex2, normalized_vertex3) / total_area;
            const auto beta = signedTriangleArea(Vertex(x, y, 0), normalized_vertex3, normalized_vertex1) / total_area;
            const auto gamma = signedTriangleArea(Vertex(x, y, 0), normalized_vertex1, normalized_vertex2) / total_area;

            if (alpha < 0 || beta < 0 || gamma < 0) {
                continue;
            }

            const auto z_1 = static_cast<unsigned char>(normalized_vertex1.getZ() * alpha);
            const auto z_2 = static_cast<unsigned char>(normalized_vertex2.getZ() * beta);
            const auto z_3 = static_cast<unsigned char>(normalized_vertex3.getZ() * gamma);

            framebuffer.set(x, y, {z_1, z_2, z_3, 255});
        }
    }
}

int main(int argc, char** argv) {
    constexpr int width = 1000;
    constexpr int height = 1000;
    TGAImage framebuffer(width, height, TGAImage::RGB);

    // Model model;
    // std::string file_name{"african_head/african_head.obj"};
    // model.loadVerticesFromObj(file_name);
    // model.loadFacesFromObjs(file_name);
    //
    // auto const& vertices = model.getVertices();
    // auto const& faces = model.getFaces();
    // for (auto const& indices : faces) {
    //     auto const index1 = std::get<0>(indices);
    //     auto const index2 = std::get<1>(indices);
    //     auto const index3 = std::get<2>(indices);
    //
    //     TGAColor random_color;
    //     for (int c = 0; c < 3; c++)
    //         random_color[c] = std::rand() % 255;
    //
    //     drawTriangle(vertices[index1], vertices[index2], vertices[index3], framebuffer, random_color);
    // }

    drawTriangle(Vertex(170, 40, 255), Vertex(550, 390, 255), Vertex(230, 590, 255), framebuffer, red);

    framebuffer.write_tga_file("framebuffer.tga");
    return 0;
}
