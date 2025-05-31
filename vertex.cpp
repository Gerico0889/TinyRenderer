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

bool Vertex::operator==(const Vertex& rhs) const {
    if (this->_x == rhs.getX() && this->_y == rhs.getY() && this->_z == rhs.getZ()) {
        return true;
    }

    return false;
}

Vertex Vertex::normalize_to_viewport(const Vertex& vertex, const int width, const int height) {
    if (vertex.getX() < -1 || vertex.getX() > 1 || vertex.getY() < -1 || vertex.getY() > 1) {
        return vertex;
    }

    float const normalized_x = (vertex.getX() + 1) * 0.5 * width;
    float const normalized_y = (vertex.getY() + 1) * 0.5 * width;
    float const normalized_z = (vertex.getZ() + 1) * 0.5 * 255;

    return Vertex(normalized_x, normalized_y, normalized_z);
}
