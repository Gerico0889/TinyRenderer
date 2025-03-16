#pragma once

class Vertex {
public:
    Vertex() = default;
    explicit Vertex(const float x, const float y, const float z);
    Vertex(const Vertex&) = default;
    Vertex(Vertex&&) = default;
    Vertex& operator=(const Vertex&) = default;
    Vertex& operator=(Vertex&&) = default;
    ~Vertex() = default;

    bool operator==(const Vertex& rhs) const;

    float getX() const;
    float getY() const;
    float getZ() const;

    void setX(const float x);
    void setY(const float y);
    void setZ(const float z);

    static Vertex normalize_to_viewport(const Vertex& vertex, const int width, const int height);

private:
    float _x;
    float _y;
    float _z;
};
