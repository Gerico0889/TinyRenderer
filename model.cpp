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
    int face1_n, face2_n, face3_n;
    while (model_file >> first_char >> face1 >> face2 >> face3) {
        if (first_char == 'f') {
            std::string result{};
            for (auto character : face1) {
                if (character == '/') {
                    face1_n = std::stoi(result);
                    result = {};
                    break;
                }

                result += character;
            }

            for (auto character : face2) {
                if (character == '/') {
                    face2_n = std::stoi(result);
                    result = {};
                    break;
                }

                result += character;
            }

            for (auto character : face3) {
                if (character == '/') {
                    face3_n = std::stoi(result);
                    result = {};
                    break;
                }

                result += character;
            }

            // The indices in the file are 1-based indexing,
            // we need to decrement by 1 to match the vertices
            _faces.emplace_back(--face1_n, --face2_n, --face3_n);
        }
    }
}

std::vector<Vertex> Model::getVertices() {
    return _vertices;
}

std::vector<std::tuple<int, int, int>> Model::getFaces() {
    return _faces;
}
