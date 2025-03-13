#pragma once

#include "vertex.h"

#include <string>
#include <vector>

class Model {
public:
    Model() = default;

    void loadFromObj(const std::string& file_name);
    std::vector<Vertex> getVertices();
    std::vector<int> getFaces();

private:
    std::vector<Vertex> _vertices;
    std::vector<int> _faces;
};
