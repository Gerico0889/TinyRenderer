#pragma once

#include <string>
#include <vector>

class Model {
public:
    Model() = default;

    void loadFromObj(const std::string& file_name);
    std::vector<float> getVertices();
    std::vector<int> getFaces();

private:
    std::vector<float> _vertices;
    std::vector<int> _faces;
};
