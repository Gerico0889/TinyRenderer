#include "gl.h"

#include "matrix.h"

#include <algorithm>

void perspective(const double focal) {
    Perspective = {{{1, 0, 0, 0},
                    {0, 1, 0, 0},
                    {0, 0, 1, 0},
                    {0, 0, -1 / focal, 1}}};
}

void viewport(const int x, const int y, const int w, const int h) {
    Viewport = {{{w / 2., 0, 0, x + w / 2.},
                 {0, h / 2., 0, y + h / 2.},
                 {0, 0, 1, 0},
                 {0, 0, 0, 1}}};
}

void lookAt(const Vec3& eye, const Vec3& center, const Vec3& up) {
    Vec3 n = normalized(eye - center);
    Vec3 l = normalized(cross(up, n));
    Vec3 m = normalized(cross(n, l));
    Modelview = Matrix<4, 4>{{{l.x, l.y, l.z, 0}, {m.x, m.y, m.z, 0}, {n.x, n.y, n.z, 0}, {0, 0, 0, 1}}}
                * Matrix<4, 4>{{{1, 0, 0, -center.x}, {0, 1, 0, -center.y}, {0, 0, 1, -center.z}, {0, 0, 0, 1}}};
}

void rasterize(const Vec4 clip[3], std::vector<double>& zbuffer, TGAImage& framebuffer, const TGAColor color) {
    Vec4 ndc[3] = {clip[0] / clip[0].w, clip[1] / clip[1].w, clip[2] / clip[2].w};
    Vec2 screen[3] = {
        (Viewport * ndc[0]).xy(),
        (Viewport * ndc[1]).xy(),
        (Viewport * ndc[2]).xy(),
    };

    Matrix<3, 3> ABC{{{screen[0].x, screen[0].y, 1.0},
                      {screen[1].x, screen[1].y, 1.0},
                      {screen[2].x, screen[2].y, 1.0}}};

    // Backface culling
    if (ABC.det() < 1) {
        return;
    }

    auto [bbminx, bbmaxx] = std::minmax({screen[0].x, screen[1].x, screen[2].x});
    auto [bbminy, bbmaxy] = std::minmax({screen[0].y, screen[1].y, screen[2].y});
    for (auto x = std::max<std::size_t>(bbminx, 0); x <= std::min<std::size_t>(bbmaxx, framebuffer.width() - 1); ++x) {
        for (auto y = std::max<std::size_t>(bbminy, 0); y <= std::min<std::size_t>(bbmaxy, framebuffer.height() - 1); ++y) {
            Vec3 bc = ABC.invertTranspose() * Vec3{static_cast<double>(x), static_cast<double>(y), 1.};

            if (bc.x < 0 || bc.y < 0 || bc.z < 0)
                continue;

            double z = bc * Vec3{ndc[0].z, ndc[1].z, ndc[2].z};
            if (z <= zbuffer[x + y * framebuffer.width()])
                continue;

            zbuffer[x + y * framebuffer.width()] = z;
            framebuffer.set(x, y, color);
        }
    }
}
