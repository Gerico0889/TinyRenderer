#include "model.h"

Model::loadFromObj(const std::string& file_name) {
}

std::vector<float> Model::getVertices() {
    return _vertices;
}

std::vector<int> Model::getFaces() {
    return _faces;
}
