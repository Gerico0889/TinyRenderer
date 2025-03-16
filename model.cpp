#include "model.h"

#include <fstream>
#include <iostream>

void Model::loadVerticesFromObj(const std::string& file_name) {
    std::ifstream model_file{"obj/" + file_name};

    if (!model_file) {
        std::cerr << "Could not find file!\n";
        return;
    }

    float x, y, z;
    char first_char;
    while (model_file >> first_char >> x >> y >> z) {
        if (first_char == 'v') {
            _vertices.emplace_back(x, y, z);
        } else {
            return;
        }
    }
}

void Model::loadFacesFromObjs(const std::string& file_name) {
    std::ifstream model_file{"obj/" + file_name};

    if (!model_file) {
        std::cerr << "Could not find file!\n";
        return;
    }

    std::string line{};
    while (std::getline(model_file, line)) {
        if (line == "s 1") {
            break;
        }
    }

    char first_char;
    std::string face1, face2, face3;
    while (model_file >> first_char >> face1 >> face2 >> face3) {
        if (first_char == 'f') {
            _faces.emplace_back(parseFaceIndex(face1), parseFaceIndex(face2), parseFaceIndex(face3));
        }
    }
}

int Model::parseFaceIndex(const std::string& face) {
    return std::stoi(face.substr(0, face.find('/'))) - 1;
}

std::vector<Vertex> Model::getVertices() {
    return _vertices;
}

std::vector<std::tuple<int, int, int>> Model::getFaces() {
    return _faces;
}
