#include <cstring>
#include <fstream>
#include <sstream>
#include <vector>
#include <glm/glm.hpp>

bool loadOBJ(
    const char * path, 
    std::vector<glm::vec3> & out_vertices, 
    std::vector<glm::vec3> & out_normals,
    std::vector<glm::ivec3> & out_faces)
{
    std::ifstream file(path, std::ios::in);
    if(!file)
    {
        printf("Impossible to open the file !\n");
        return false;
    }

    while(!file.eof())
    {
        char lineHeader[128];
        // read the first word of the line
        file >> lineHeader;
        if (strcmp(lineHeader, "v") == 0){
            glm::vec3 vertex;
            file >> vertex.x >> vertex.y >> vertex.z;
            out_vertices.push_back(vertex);
        }
        else if(strcmp(lineHeader, "vn") == 0){
            glm::vec3 normal;
            file >> normal.x >> normal.y >> normal.z;
            out_normals.push_back(normal);
        }
        else if(strcmp(lineHeader, "f") == 0){
            std::string vertex1, vertex2, vertex3;
            glm::ivec3 face;
            file >> vertex1 >> vertex2 >> vertex3;
            face.x = stoi(vertex1.substr(0, vertex1.find("/")));
            face.y = stoi(vertex2.substr(0, vertex2.find("/")));
            face.z = stoi(vertex3.substr(0, vertex3.find("/")));
            // In OBJ, vertex indices start from 1, we need them to start from 0
            face.x--; face.y--; face.z--;
            out_faces.push_back(face);
        }
    }
    return true;
}
