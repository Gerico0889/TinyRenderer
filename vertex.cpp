#include "vertex.h"

Vertex::Vertex(const float x, const float y, const float z)
    : _x(x), _y(y), _z(z) {
}

float Vertex::getX() const {
    return _x;
}

float Vertex::getY() const {
    return _y;
}

float Vertex::getZ() const {
    return _z;
}

void Vertex::setX(const float x) {
    _x = x;
}

void Vertex::setY(const float y) {
    _y = y;
}

void Vertex::setZ(const float z) {
    _z = z;
}
