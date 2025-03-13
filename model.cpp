#include "model.h"

#include <fstream>
#include <iostream>

void Model::loadFromObj(const std::string& file_name) {
    std::ifstream model_file{"obj/" + file_name};

    if (!model_file) {
        std::cerr << "Could not find file!\n";
        return;
    }

    float x, y, z;
    std::string string{};
    while (model_file >> string >> x >> y >> z) {
        if (string == "v") {
            _vertices.emplace_back(x, y, z);
        } else {
            return;
        }
    }
}

std::vector<Vertex> Model::getVertices() {
    return _vertices;
}

std::vector<int> Model::getFaces() {
    return _faces;
}
