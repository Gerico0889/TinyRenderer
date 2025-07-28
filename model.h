#pragma once

#include "vector.h"

#include <string>
#include <tuple>
#include <vector>

class Model {
public:
    Model() = default;

    void loadVerticesFromObj(const std::string& file_name);
    void loadFacesFromObjs(const std::string& file_name);
    void loadNormalsFromObj(const std::string& file_name);
    std::vector<Vec3> getVertices();
    std::vector<Vec3> getNormals();
    std::vector<std::tuple<int, int, int>> getVertexFaces();
    std::vector<std::tuple<int, int, int>> getNormalFaces();

private:
    int parseFaceIndex(const std::string& face);

    std::vector<Vec3> _vertices;
    std::vector<Vec3> _normals;
    std::vector<std::tuple<int, int, int>> _vertexFaces;
    std::vector<std::tuple<int, int, int>> _normalFaces;
};
