#pragma once

#include "vertex.h"

#include <string>
#include <tuple>
#include <vector>

class Model {
public:
    Model() = default;

    void loadVerticesFromObj(const std::string& file_name);
    void loadFacesFromObjs(const std::string& file_name);
    std::vector<Vertex> getVertices();
    std::vector<std::tuple<int, int, int>> getFaces();

private:
    std::vector<Vertex> _vertices;
    std::vector<std::tuple<int, int, int>> _faces;
};
