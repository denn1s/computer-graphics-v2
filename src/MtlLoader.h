#include <glm/glm.hpp>
#include <string>
#include <unordered_map>

bool loadMTL(const std::string& filename, std::unordered_map<std::string, glm::vec3>& materials);
