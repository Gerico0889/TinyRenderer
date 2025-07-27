#include "model.h"

#include <fstream>
#include <iostream>
#include <sstream>

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

    {
        std::string line{};
        while (std::getline(model_file, line)) {
            if (line == "s 1") {
                break;
            }
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

void Model::loadNormalsFromObj(const std::string& file_name) {
    std::ifstream model_file{"obj/" + file_name};

    if (!model_file) {
        std::cerr << "Could not find file!\n";
        return;
    }

    // With this we're going through the whole file
    // even when we're done with parsing normals.
    // Do I care at the moment? No
    std::string line{};
    while (std::getline(model_file, line)) {
        if (line.substr(0, 2) == "vn") {
            std::istringstream iss(line.substr(3));
            float x, y, z;
            if (iss >> x >> y >> z) {
                _normals.emplace_back(x, y, z);
            }
        }
    }
}

int Model::parseFaceIndex(const std::string& face) {
    return std::stoi(face.substr(0, face.find('/'))) - 1;
}

std::vector<Vec3> Model::getVertices() {
    return _vertices;
}

std::vector<Vec3> Model::getNormals() {
    return _normals;
}

std::vector<std::tuple<int, int, int>> Model::getFaces() {
    return _faces;
}
