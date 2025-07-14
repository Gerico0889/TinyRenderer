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

Matrix<4, 4> Viewport, Modelview, Perspective;

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

void perspective(const double focal) {
    Perspective = Matrix<4, 4>(1, 0, 0, 0,
                               0, 1, 0, 0,
                               0, 0, 1, 0,
                               0, 0, -1 / focal, 1);
}

Vec3 project(Vec3 vector, int width, int height) {
    return {
        (vector.x() + 1.) * width / 2,
        (vector.y() + 1.) * height / 2,
        (vector.z() + 1.) * 255. / 2};
};

void viewport(const Vec3& vertex, const int width, const int height) {
    Viewport = Matrix<4, 4>(width / 2, 0, 0, vertex.x() + width / 2.,
                            0, height / 2, 0, vertex.y() + height / 2.,
                            0, 0, 1, 0,
                            0, 0, 0, 1);
}

void lookAt(const Vec3 eye, const Vec3 center, const Vec3 up, const int width, const int height) {
    Vec3 n = normalize_to_viewport(eye - center, width, height);
    Vec3 l = normalize_to_viewport(up.cross(n), width, height);
    Vec3 m = normalize_to_viewport(n.cross(l), width, height);
    Matrix<4, 4> m1(l.x(), l.y(), l.z(), 0,
                    m.x(), m.y(), m.z(), 0,
                    n.x(), n.y(), n.z(), 0,
                    0, 0, 0, 1);
    Matrix<4, 4> m2(1, 0, 0, -center.x(),
                    0, 1, 0, -center.y(),
                    0, 0, 1, -center.z(),
                    0, 0, 0, 1);
    Modelview = m1 * m2;
}

void rasterize(const Vec4 clip[3], std::vector<double>& zbuffer, TGAImage& framebuffer, const TGAColor color) {
    Vec4 ndc[3]{clip[0] / clip[0].w(), clip[1] / clip[1].w(), clip[2] / clip[2].w()};
    Vec2 screen[3] = {(Viewport * ndc[0]).xy(), (Viewport * ndc[1]).xy(), (Viewport * ndc[2]).xy()};

    Matrix<3, 3> ABC(screen[0].x(), screen[0].y(), 1.,
                     screen[1].x(), screen[1].y(), 1.,
                     screen[2].x(), screen[2].y(), 1.);

    auto [bbminx, bbmaxx] = std::minmax({screen[0].x(), screen[1].x(), screen[2].x()});
    auto [bbminy, bbmaxy] = std::minmax({screen[0].y(), screen[1].y(), screen[2].y()});
    for (auto x = std::max<std::size_t>(bbminx, 0); x <= std::min<std::size_t>(bbmaxx, framebuffer.width() - 1); ++x) {
        for (auto y = std::max<std::size_t>(bbminy, 0); y <= std::min<std::size_t>(bbmaxy, framebuffer.height() - 1); ++y) {
            Vec3 bc = ABC.invert_transpose() * Vec3(static_cast<double>(x), static_cast<double>(y), 1.);

            if (bc.x() < 0 || bc.y() < 0 || bc.z() < 0)
                continue;

            double z = bc * Vec3(ndc[0].z(), ndc[1].z(), ndc[2].z());
            if (z <= zbuffer[x + y * framebuffer.width()])
                continue;

            zbuffer[x + y * framebuffer.width()] = z;
            framebuffer.set(x, y, color);
        }
    }
}

int main(int argc, char** argv) {
    constexpr int width = 2000;
    constexpr int height = 2000;
    const Vec3 eye(-1, 0, 2);   // Camera position
    const Vec3 center(0, 0, 0); // Camera direction
    const Vec3 up(0, 1, 0);     // Camera up vector

    TGAImage framebuffer(width, height, TGAImage::RGB);
    std::vector<double> zbuffer(width * height, -std::numeric_limits<double>::max());

    Model model;
    std::string file_name{"african_head/african_head.obj"};
    model.loadVerticesFromObj(file_name);
    model.loadFacesFromObjs(file_name);

    auto const& vertices = model.getVertices();
    auto const& faces = model.getFaces();
    Vec4 clip[3];

    for (const auto& face : faces) {
        Vec3 v0 = vertices[std::get<0>(face)];
        Vec3 v1 = vertices[std::get<1>(face)];
        Vec3 v2 = vertices[std::get<2>(face)];
        clip[0] = Perspective * Modelview * Vec4{v0.x(), v0.y(), v0.z(), 1.};
        clip[1] = Perspective * Modelview * Vec4{v1.x(), v1.y(), v1.z(), 1.};
        clip[2] = Perspective * Modelview * Vec4{v2.x(), v2.y(), v2.z(), 1.};

        TGAColor rnd;
        for (int c = 0; c < 3; c++)
            rnd[c] = std::rand() % 255;
        rasterize(clip, zbuffer, framebuffer, rnd);
    }

    // drawTriangle(Vec3(170, 40, 255), Vec3(550, 390, 255), Vec3(230, 590, 255), framebuffer, red);

    framebuffer.write_tga_file("framebuffer.tga");
    return 0;
}
