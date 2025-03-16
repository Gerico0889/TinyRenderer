#include "model.h"
#include "tgaimage.h"

#include <cmath>
#include <iostream>

constexpr TGAColor white = {255, 255, 255, 255}; // attention, BGRA order
constexpr TGAColor green = {0, 255, 0, 255};
constexpr TGAColor red = {0, 0, 255, 255};
constexpr TGAColor blue = {255, 128, 64, 255};
constexpr TGAColor yellow = {0, 200, 255, 255};

void drawLine(int ax, int ay, int bx, int by, TGAImage& framebuffer, const TGAColor& color) {
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

Vertex normalize_vertex(const Vertex& vertex, const int width, const int height) {
    float const normalized_x = (vertex.getX() + 1) * 0.5 * width;
    float const normalized_y = (vertex.getY() + 1) * 0.5 * width;

    return Vertex(normalized_x, normalized_y, vertex.getZ());
}

void drawLine(const Vertex& vertex1, const Vertex& vertex2, TGAImage& framebuffer, const TGAColor& color) {
    auto const normalized_vertex1 = normalize_vertex(vertex1, framebuffer.width(), framebuffer.height());
    auto const normalized_vertex2 = normalize_vertex(vertex2, framebuffer.width(), framebuffer.height());

    for (float t = 0; t <= 1; t += 0.01) {
        int x = normalized_vertex1.getX() + t * (normalized_vertex2.getX() - normalized_vertex1.getX());
        int y = normalized_vertex1.getY() + t * (normalized_vertex2.getY() - normalized_vertex1.getY());

        framebuffer.set(x, y, color);
    }
}

int main(int argc, char** argv) {
    constexpr int width = 2000;
    constexpr int height = 2000;
    TGAImage framebuffer(width, height, TGAImage::RGB);

    Model diablo3;
    diablo3.loadVerticesFromObj("diablo3_pose/diablo3_pose.obj");
    diablo3.loadFacesFromObjs("diablo3_pose/diablo3_pose.obj");

    auto const& vertices = diablo3.getVertices();
    auto const& faces = diablo3.getFaces();
    for (auto const& indices : faces) {
        auto const index1 = std::get<0>(indices);
        auto const index2 = std::get<1>(indices);
        auto const index3 = std::get<2>(indices);

        drawLine(vertices[index1], vertices[index2], framebuffer, red);
        drawLine(vertices[index2], vertices[index3], framebuffer, red);
        drawLine(vertices[index3], vertices[index1], framebuffer, red);
    }

    framebuffer.write_tga_file("framebuffer.tga");
    return 0;
}
