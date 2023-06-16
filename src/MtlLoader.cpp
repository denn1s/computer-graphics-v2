#include "MtlLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>

bool loadMTL(const std::string& filename, std::unordered_map<std::string, glm::vec3>& materials) {
    std::ifstream file(filename);

    if (!file) {
        std::cerr << "Unable to open file " << filename << std::endl;
        return false;
    }

    std::string currentMaterial;
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string firstWord;
        ss >> firstWord;

        if (firstWord == "newmtl") {
            ss >> currentMaterial;
        }
        else if (firstWord == "Kd") {
            glm::vec3 diffuseColor;
            ss >> diffuseColor.r >> diffuseColor.g >> diffuseColor.b;
            materials[currentMaterial] = diffuseColor;
        }
    }

    return true;
}
