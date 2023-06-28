#include "print.h"

int main() {
    glm::vec3 exampleVec3(1.0f, 2.0f, 3.0f);
    glm::mat4 exampleMat4(1.0f);  // Identity matrix

    print("Hello, World!", exampleVec3, exampleMat4);

    return 0;
}