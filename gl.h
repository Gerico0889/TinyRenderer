#include "matrix.h"
#include "tgaimage.h"
#include "vector.h"

inline Matrix<4, 4> Viewport;
inline Matrix<4, 4> Modelview;
inline Matrix<4, 4> Perspective;

void perspective(const double focal);
void viewport(const int x, const int y, const int w, const int h);
void lookAt(const Vec3& eye, const Vec3& center, const Vec3& up);
void rasterize(const Vec4 clip[3], std::vector<double>& zbuffer, TGAImage& framebuffer, const TGAColor color);
