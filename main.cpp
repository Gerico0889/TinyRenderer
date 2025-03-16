#include "model.h"
#include "tgaimage.h"

#include <algorithm>
#include <cmath>

constexpr TGAColor white = {255, 255, 255, 255}; // attention, BGRA order
constexpr TGAColor green = {0, 255, 0, 255};
constexpr TGAColor red = {0, 0, 255, 255};
constexpr TGAColor blue = {255, 128, 64, 255};
constexpr TGAColor yellow = {0, 200, 255, 255};

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
    std::vector<Vertex> vertex_vec{vertex1, vertex2, vertex3};
    std::sort(std::begin(vertex_vec), std::end(vertex_vec), [](auto const& a, auto const& b) {
        return a.getY() < b.getY();
    });

    const int total_height = vertex_vec[2].getY() - vertex_vec[0].getY();

    const auto ax = vertex_vec[0].getX();
    const auto ay = vertex_vec[0].getY();
    const auto bx = vertex_vec[1].getX();
    const auto by = vertex_vec[1].getY();
    const auto cx = vertex_vec[2].getX();
    const auto cy = vertex_vec[2].getY();

    if (ay != by) {
        const int segment_height = by - ay;
        for (int y = ay; y <= by; ++y) {
            const int x1 = ax + ((cx - ax) * (y - ay)) / total_height;
            const int x2 = ax + ((bx - ax) * (y - ay)) / segment_height;
            for (int x = std::min(x1, x2); x < std::max(x1, x2); ++x) {
                framebuffer.set(x, y, color);
            }
        }
    }

    if (cy != by) {
        const int segment_height = cy - by;
        for (int y = by; y <= cy; ++y) {
            const int x1 = ax + ((cx - ax) * (y - ay)) / total_height;
            const int x2 = bx + ((cx - bx) * (y - by)) / segment_height;
            for (int x = std::min(x1, x2); x < std::max(x1, x2); ++x) {
                framebuffer.set(x, y, color);
            }
        }
    }
}

int main(int argc, char** argv) {
    constexpr int width = 128;
    constexpr int height = 128;
    TGAImage framebuffer(width, height, TGAImage::RGB);

    // Model diablo3;
    // diablo3.loadVerticesFromObj("diablo3_pose/diablo3_pose.obj");
    // diablo3.loadFacesFromObjs("diablo3_pose/diablo3_pose.obj");
    //
    // auto const& vertices = diablo3.getVertices();
    // auto const& faces = diablo3.getFaces();
    // for (auto const& indices : faces) {
    //     auto const index1 = std::get<0>(indices);
    //     auto const index2 = std::get<1>(indices);
    //     auto const index3 = std::get<2>(indices);
    //
    //     drawLine(vertices[index1], vertices[index2], framebuffer, red);
    //     drawLine(vertices[index2], vertices[index3], framebuffer, red);
    //     drawLine(vertices[index3], vertices[index1], framebuffer, red);
    // }

    drawTriangle(Vertex(7, 45, 0), Vertex(35, 100, 0), Vertex(45, 60, 0), framebuffer, red);
    drawTriangle(Vertex(120, 35, 0), Vertex(90, 5, 0), Vertex(45, 110, 0), framebuffer, white);
    drawTriangle(Vertex(115, 83, 0), Vertex(80, 90, 0), Vertex(85, 120, 0), framebuffer, green);

    framebuffer.write_tga_file("framebuffer.tga");
    return 0;
}
