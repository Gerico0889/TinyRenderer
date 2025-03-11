#include "tgaimage.h"

#include <cmath>

constexpr TGAColor white = {255, 255, 255, 255}; // attention, BGRA order
constexpr TGAColor green = {0, 255, 0, 255};
constexpr TGAColor red = {0, 0, 255, 255};
constexpr TGAColor blue = {255, 128, 64, 255};
constexpr TGAColor yellow = {0, 200, 255, 255};

void drawLine(const int ax, const int ay, const int bx, const int by, TGAImage& framebuffer, const TGAColor& color) {
    for (float t = 0; t <= 1; t += .02) {
        float xt = std::round(ax + t * (bx - ax));
        float yt = std::round(ay + t * (by - ay));
        framebuffer.set(xt, yt, color);
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

    framebuffer.set(ax, ay, white);
    framebuffer.set(bx, by, white);
    framebuffer.set(cx, cy, white);

    framebuffer.write_tga_file("framebuffer.tga");
    return 0;
}
