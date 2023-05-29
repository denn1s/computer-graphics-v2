#include <iostream>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <SDL2/SDL_opengl.h>
#include "shaders.h"

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

    // Initialize GLEW. GLEW manages function pointers for OpenGL so we want to initialize it before
    // calling any OpenGL functions. We are using the experimental flag to get access to more features.
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW: " << glewGetErrorString(err) << std::endl;
        return false;
    }

    return true;
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

// This function sets up the vertex data
bool vertexSetup(GLuint &vertexArrayID, GLuint &vertexBuffer, const std::vector<glm::vec3>& vertices) {
    // Create a vertex array object (VAO)
    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);
    // A VAO stores all the links between the attributes and the VBOs with the raw vertex data

    // Generate 1 buffer, put the resulting identifier in vertexbuffer
    glGenBuffers(1, &vertexBuffer);
    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    // Give our vertices to OpenGL
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

    return true;
}

// This function sets up the vertex data and creates a VAO and VBO
GLuint vertexSetup(const std::vector<glm::vec3>& vertices) {
    // Generate the Vertex Array Object (VAO) and the Vertex Buffer Object (VBO)
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    // Bind the Vertex Array Object
    glBindVertexArray(VAO);

    // Bind the Vertex Buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    // Send the vertex data to the Vertex Buffer
    glBufferData(GL_ARRAY_BUFFER, // target: Specifies the target buffer object. The symbolic constant must be GL_ARRAY_BUFFER.
             vertices.size() * sizeof(glm::vec3), // size: Specifies the size in bytes of the buffer object's new data store.
             &vertices[0], // data: Specifies a pointer to the data that will be copied into the data store for initialization, or NULL if no data is to be copied.
             GL_STATIC_DRAW); // usage: Specifies the expected usage pattern of the data store.

    // Set the vertex attributes (only position here)
    glVertexAttribPointer(0, // index: Specifies the index of the generic vertex attribute to be modified.
                        3, // size: Specifies the number of components per generic vertex attribute. Must be 1, 2, 3, 4.
                        GL_FLOAT, // type: Specifies the data type of each component in the array.
                        GL_FALSE, // normalized: Specifies whether fixed-point data values should be normalized (GL_TRUE) or converted directly as fixed-point values (GL_FALSE) when they are accessed.
                        3 * sizeof(float), // stride: Specifies the byte offset between consecutive generic vertex attributes. If stride is 0, the generic vertex attributes are understood to be tightly packed in the array.
                        (void*)0); // pointer: Specifies an offset of the first component of the first generic vertex attribute in the array. It's cast to void* here because the API requires this type.
    glEnableVertexAttribArray(0);
    
    // Unbind the VAO (optional)
    glBindVertexArray(0);

    // Unbind the VBO and delete it
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &VBO);

    // Return the VAO
    return VAO;
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

    // Setup vertex data
    std::vector<glm::vec3> vertices = {
        glm::vec3(-0.5f, -0.5f, 0.0f),
        glm::vec3(0.5f, -0.5f, 0.0f),
        glm::vec3(0.0f, 0.5f, 0.0f)
    };
    GLuint VAO = vertexSetup(vertices);

    // Setup time related variables
    Uint32 currentTime = SDL_GetTicks();
    Uint32 lastTime = 0;
    float deltaTime = 0.0f;

    float rotateAngle = 0.0f;

    // Main loop
    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            // Add other event handling here if needed...
        }
        // Calculate delta time
        lastTime = currentTime;
        currentTime = SDL_GetTicks();
        deltaTime = (currentTime - lastTime) / 1000.0f;

        // Clear the screen to black
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        rotateAngle += glm::radians(120.0f) * deltaTime;  // 120 deg per second
        
        glm::mat4 rotateMat = glm::rotate(glm::mat4(1.0f), rotateAngle, glm::vec3(0.0f, 1.0f, 0.0f));
        std::cout << rotateAngle << std::endl;
        // Pass the rotation matrix to the vertex shader
        GLuint transformLoc = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(rotateMat));

        // Draw your object
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());
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