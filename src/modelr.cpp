#include "modelr.h"
#include <iostream>
#include <fstream>
#include <sstream>

modelr::modelr(const std::string& filename) : filename(filename) {}

bool modelr::LoadModel() const{
    std::ifstream objFile(filename);
    if (!objFile.is_open()) {
        std::cerr << "Failed to open obj file: " << filename << std::endl;
        return false;
    }

    std::string line;

    while (std::getline(objFile, line)) {
        if (line.substr(0, 2) == "v ") {
            Vertex vertex;
            sscanf(line.c_str(), "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
            vertices.push_back(vertex);
        } else if (line.substr(0, 2) == "f ") {
            std::istringstream iss(line.substr(2));
            std::vector<int> faceIndices;
            int vertexIndex, textureIndex, normalIndex;

            while (iss >> vertexIndex) {
                if (iss.peek() == '/') {
                    iss.ignore();
                    if (iss.peek() != '/') iss >> textureIndex;
                    if (iss.peek() == '/') {
                        iss.ignore();
                        iss >> normalIndex;
                    }
                }

                faceIndices.push_back(vertexIndex);
            }
            faces.push_back(faceIndices);
        }
    }

    objFile.close();
    return true;
}

const std::vector<modelr::Vertex>& modelr::GetVertices() const {
    return vertices;
}

const std::vector<std::vector<int>>& modelr::GetFaces() const {
    return faces;
}

