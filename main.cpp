#include "tgaimage.h"

#include <cmath>

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

    for (float x = ax; x <= bx; ++x) {
        float t = (x - ax) / static_cast<float>(bx - ax);
        int y = std::round(ay + (by - ay) * t);
        if (steep) {
            framebuffer.set(y, x, color);
        } else {
            framebuffer.set(x, y, color);
        }
    }
}

int main(int argc, char** argv) {
    constexpr int width = 64;
    constexpr int height = 64;
    TGAImage framebuffer(width, height, TGAImage::RGB);

    int ax = 7, ay = 3;
    int bx = 12, by = 37;
    int cx = 62, cy = 53;

    drawLine(ax, ay, bx, by, framebuffer, red);
    drawLine(ax, ay, cx, cy, framebuffer, green);
    drawLine(cx, cy, bx, by, framebuffer, blue);

    framebuffer.write_tga_file("framebuffer.tga");
    return 0;
}
