#ifndef modelr_H
#define modelr_H

#include <vector>
#include <string>

class modelr {
public:
    struct Vertex {
        float x, y, z;
    };

    modelr(const std::string& filename);

    bool LoadModel() const;

    const std::vector<Vertex>& GetVertices() const;
    const std::vector<std::vector<int>>& GetFaces() const;
private:
    std::string filename;
    mutable std::vector<Vertex> vertices;
    mutable std::vector<std::vector<int>> faces;
};

#endif // modelr_H
