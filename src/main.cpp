#include <map>
#include <iostream>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <array>
#include <unordered_map>

#include "glad.h"
#include "shaders.h"
#include "ObjLoader.h"
#include "MtlLoader.h"

bool init(SDL_Window*& window, SDL_GLContext& context) {

    // Initialize SDL's video subsystem.
    // SDL_Init will return -1 if it fails to initialize.
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return false;
    }

    // Create an application window with the following settings:
    window = SDL_CreateWindow(
        "OpenGL",               // window title
        SDL_WINDOWPOS_CENTERED, // initial x position
        SDL_WINDOWPOS_CENTERED, // initial y position
        800,                    // width, in pixels
        600,                    // height, in pixels
        SDL_WINDOW_OPENGL       // flags - set to use OpenGL
    );

    // Check that the window was successfully created
    if (window == NULL) {
        // In the case that the window could not be made...
        std::cerr << "Could not create window: " << SDL_GetError() << std::endl;
        return false;
    }

    // We want to request that SDL creates a OpenGL context for our window.
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3); // Request OpenGL 3.3
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3); // Request OpenGL 3.3

    // Create an OpenGL context associated with the window.
    context = SDL_GL_CreateContext(window);

    // glad
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        // Failed to initialize GLAD
        std::cout << "Failed to load glad" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    return true;
}

GLuint vertexSetup(
    const std::vector<glm::vec3>& vertices,
    const std::vector<glm::vec3>& normals,
    const std::vector<Face>& faces,
    const std::unordered_map<std::string, glm::vec3>& materials,
    const std::vector<std::string>& materialsPerFace
) {
    // Step 1: Unpack the OBJ data into a format OpenGL understands.
    std::vector<float> unpackedData;
    
    for (std::size_t index = 0; Face face : faces) {
        for (auto vertexIndex: face.vertexIndices) {
            // Add vertex positions
            unpackedData.push_back(vertices[vertexIndex[0]].x);
            unpackedData.push_back(vertices[vertexIndex[0]].y);
            unpackedData.push_back(vertices[vertexIndex[0]].z);

            // Add normals
            unpackedData.push_back(normals[vertexIndex[2]].x);
            unpackedData.push_back(normals[vertexIndex[2]].y);
            unpackedData.push_back(normals[vertexIndex[2]].z);

            // Add diffuse light to array
            glm::vec3 material = materials.at(materialsPerFace[index]);

            unpackedData.push_back(material.x);
            unpackedData.push_back(material.y);
            unpackedData.push_back(material.z);
        }
        ++index;
    }

    // Step 2: Create and bind a Vertex Array Object (VAO).
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Step 3: Create a Vertex Buffer Object (VBO) and copy the vertex data to it.
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, unpackedData.size() * sizeof(float), unpackedData.data(), GL_STATIC_DRAW);

    // Step 4: Set up vertex attribute pointers.
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // Color attribute
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Unbind VAO (NOT the EBO)
    glBindVertexArray(0);

    // Delete the VBO as we've copied the data to the GPU
    glDeleteBuffers(1, &VBO);

    return VAO;
}


GLuint setupShaders(const char* vertexSource, const char* fragmentSource) {
    // Create and compile the vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);

    // Check the vertex shader
    GLint success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Create and compile the fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

    // Check the fragment shader
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Link shaders
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check the shader program
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // Delete shaders as they're linked into our program now and no longer necessery
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

int main() {

    // The window we'll be rendering to
    SDL_Window* window = nullptr;

    // OpenGL context
    SDL_GLContext context;

    // Initialize SDL and OpenGL
    if (!init(window, context)) {
        std::cerr << "Failed to initialize!" << std::endl;
        return EXIT_FAILURE;
    }

    // Setup shaders
    GLuint shaderProgram = setupShaders(vertexShaderSource, fragmentShaderSource);
    if (shaderProgram == 0) {
        std::cerr << "Failed to set up the shader program." << std::endl;
        return EXIT_FAILURE;
    }

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<Face> faces;
    std::vector<std::string> materialsPerFace;
    std::unordered_map<std::string, glm::vec3> materials;
        
    loadOBJ("./models/cube.obj", vertices, normals, faces, materialsPerFace);
    loadMTL("./models/cube.mtl", materials);

    GLuint VAO = vertexSetup(
        vertices,
        normals,
        faces,
        materials,
        materialsPerFace
    );
        
    // Setup time related variables
    Uint32 currentTime = SDL_GetTicks();
    Uint32 lastTime = 0;
    float deltaTime = 0.0f;

    float rotateAngle = 0.0f;

    glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);

    // Main loop
    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            if (event.type == SDL_KEYDOWN) {
                float cameraSpeed = 0.5f; // adjust accordingly
                switch (event.key.keysym.sym) {
                    case SDLK_w:
                        cameraPos += cameraSpeed * cameraFront;
                        break;
                    case SDLK_s:
                        cameraPos -= cameraSpeed * cameraFront;
                        break;
                    case SDLK_a:
                        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
                        break;
                    case SDLK_d:
                        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
                        break;
                }
            }
        }

        // Calculate delta time
        lastTime = currentTime;
        currentTime = SDL_GetTicks();
        deltaTime = (currentTime - lastTime) / 1000.0f;

        // Clear the screen to black
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = glm::lookAt(
            cameraPos, // Camera is at (4,3,3), in World Space
            cameraPos + cameraFront, // and looks at the origin
            cameraUp  // Head is up (set to 0,-1,0 to look upside-down)
        );


        glm::mat4 projection = glm::perspective(
            glm::radians(45.0f), // The field of view
            800.0f / 600.0f, // Aspect Ratio
            0.1f, // Near clipping plane
            100.0f // Far clipping plane
        );

        rotateAngle += glm::radians(120.0f) * deltaTime;  // 120 deg per second
        glm::mat4 rotateMat = glm::rotate(glm::mat4(1.0f), rotateAngle, glm::vec3(0.0f, 1.0f, 0.0f));
        // glm::mat4 rotateMat = glm::mat4(1.0f);

        GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        GLuint projLoc = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(rotateMat));

        GLuint lightPositionLoc = glGetUniformLocation(shaderProgram, "lightPosition");
        glm::vec3 lightPosition = glm::vec3(2.0f, 2.0f, 2.0f); // Position of the light source
        glUniform3fv(lightPositionLoc, 1, glm::value_ptr(lightPosition));

        // Draw your object
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        // glDrawElements(GL_TRIANGLES, faces.size() * 3, GL_UNSIGNED_INT, 0);
        glDrawArrays(GL_TRIANGLES, 0, faces.size() * 3);

        glBindVertexArray(0);

        // Swap buffers
        SDL_GL_SwapWindow(window);
    }

    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(shaderProgram);
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}