#include <iostream>
#include <fstream>
#include <algorithm>
#include <glm/glm.hpp>

// Struct representing RGB color values
struct Color
{
    unsigned char red;
    unsigned char blue;
    unsigned char green;

    Color(unsigned char r, unsigned char b, unsigned char g)
        : red(r), blue(b), green(g)
    {
    }
};

// Class representing a renderer with width, height, and framebuffer
class Renderer
{
public:
    int width;
    int height;
    unsigned char* framebuffer;

    Renderer(int w, int h)
        : width(w), height(h)
    {
        framebuffer = new unsigned char[width * height * 3];
    }

    ~Renderer()
    {
        delete[] framebuffer;
    }
};

// Function to clear the renderer's framebuffer with a specified color
void clear(Renderer* renderer, const Color& color)
{
    for (int i = 0; i < renderer->width * renderer->height * 3; i += 3)
    {
        renderer->framebuffer[i] = color.blue;
        renderer->framebuffer[i + 1] = color.green;
        renderer->framebuffer[i + 2] = color.red;
    }
}

// Function to draw a point on the renderer's framebuffer at a specified position with a specified color
void point(Renderer* renderer, const glm::ivec2& point, const Color& color)
{
    int pixelIndex = (point.y * renderer->width + point.x) * 3;
    renderer->framebuffer[pixelIndex] = color.blue;
    renderer->framebuffer[pixelIndex + 1] = color.green;
    renderer->framebuffer[pixelIndex + 2] = color.red;
}

// Function to draw a line on the renderer's framebuffer from a start position to an end position with a specified color
void line(Renderer* renderer, const glm::ivec2& start, const glm::ivec2& end, const Color& color)
{
    int dx = end.x - start.x;
    int dy = end.y - start.y;

    int steps = std::max(abs(dx), abs(dy));

    float xIncrement = static_cast<float>(dx) / steps;
    float yIncrement = static_cast<float>(dy) / steps;

    float x = static_cast<float>(start.x);
    float y = static_cast<float>(start.y);

    for (int i = 0; i <= steps; ++i)
    {
        if (y >= 0 && x >= 0 && y < renderer->height && y < renderer->width) {
            int pixelIndex = (static_cast<int>(y) * renderer->width + static_cast<int>(x)) * 3;    
            renderer->framebuffer[pixelIndex] = color.blue;
            renderer->framebuffer[pixelIndex + 1] = color.green;
            renderer->framebuffer[pixelIndex + 2] = color.red;
        }

        x += xIncrement;
        y += yIncrement;
    }
}

// Function to write the renderer's framebuffer to a BMP file
void writeBMP(Renderer* renderer, const std::string& filename)
{
    std::ofstream file(filename, std::ios::binary);

    unsigned char fileHeader[14] = {
        'B', 'M',
        0, 0, 0, 0,
        0, 0, 0, 0,
        54, 0, 0, 0
    };

    unsigned char infoHeader[40] = {
        40, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        1, 0,
        24, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0
    };

    int imageSize = renderer->width * renderer->height * 3;
    int fileSize = imageSize + sizeof(fileHeader) + sizeof(infoHeader);

    *(int*)&fileHeader[2] = fileSize;
    *(int*)&fileHeader[10] = sizeof(fileHeader) + sizeof(infoHeader);
    *(int*)&infoHeader[4] = renderer->width;
    *(int*)&infoHeader[8] = renderer->height;
    *(int*)&infoHeader[20] = imageSize;

    file.write(reinterpret_cast<char*>(fileHeader), sizeof(fileHeader));
    file.write(reinterpret_cast<char*>(infoHeader), sizeof(infoHeader));
    file.write(reinterpret_cast<char*>(renderer->framebuffer), imageSize);

    file.close();
}

void render(Renderer* renderer)
{
    const Color whiteColor(255, 255, 255);
    const Color redColor(255, 0, 0);

    clear(renderer, whiteColor);

    int squareSize = 100;  // Size of the square

    glm::ivec2 center(renderer->width / 2, renderer->height / 2);  // Center of the screen

    // Calculate the coordinates of the four corners of the square
    glm::ivec2 topLeft = center - glm::ivec2(squareSize / 2, squareSize / 2);
    glm::ivec2 topRight = center + glm::ivec2(squareSize / 2, -squareSize / 2);
    glm::ivec2 bottomLeft = center + glm::ivec2(-squareSize / 2, squareSize / 2);
    glm::ivec2 bottomRight = center + glm::ivec2(squareSize / 2, squareSize / 2);

    glm::mat3 translateToCenter = glm::mat3(
        1, 0, -renderer->width / 2,
        0, 1, -renderer->height / 2,
        0, 0, 1
    );

    glm::mat3 scale = glm::mat3(
        2, 0, 0,
        0, 2, 0,
        0, 0, 1
    );

    glm::mat3 rotate = glm::mat3(
        cos(glm::radians(45.0f)), -sin(glm::radians(45.0f)), 0.0f,
        sin(glm::radians(45.0f)), cos(glm::radians(45.0f)), 0.0f,
        0.0f, 0.0f, 1.0f
    );

    glm::mat3 translateBack = glm::mat3(
        1, 0, renderer->width / 2,
        0, 1, renderer->height / 2,
        0, 0, 1
    );

    glm::mat3 transformMatrix = translateToCenter * rotate * scale * translateBack;

    glm::ivec3 topLeftAugmented(topLeft.x, topLeft.y, 1);
    glm::ivec3 topRightAugmented(topRight.x, topRight.y, 1);
    glm::ivec3 bottomLeftAugmented(bottomLeft.x, bottomLeft.y, 1);
    glm::ivec3 bottomRightAugmented(bottomRight.x, bottomRight.y, 1);


    glm::ivec3 transformedTopLeft = topLeftAugmented * transformMatrix;
    glm::ivec3 transformedTopRight = topRightAugmented * transformMatrix;
    glm::ivec3 transformedBottomLeft = bottomLeftAugmented * transformMatrix;
    glm::ivec3 transformedBottomRight = bottomRightAugmented * transformMatrix;

    topLeft = glm::ivec2(transformedTopLeft.x / transformedTopLeft.z, transformedTopLeft.y / transformedTopLeft.z);
    topRight = glm::ivec2(transformedTopRight.x / transformedTopRight.z, transformedTopRight.y / transformedTopRight.z);
    bottomLeft = glm::ivec2(transformedBottomLeft.x / transformedBottomLeft.z, transformedBottomLeft.y / transformedBottomLeft.z);
    bottomRight = glm::ivec2(transformedBottomRight.x / transformedBottomRight.z, transformedBottomRight.y / transformedBottomRight.z);

    line(renderer, topLeft, topRight, redColor);
    line(renderer, topRight, bottomRight, redColor);
    line(renderer, bottomRight, bottomLeft, redColor);
    line(renderer, bottomLeft, topLeft, redColor);
}


int main()
{
    const int width = 800;
    const int height = 600;

    Renderer renderer(width, height);

    render(&renderer);

    writeBMP(&renderer, "image.bmp");

    std::cout << "Image generated successfully!" << std::endl;

    return 0;
}
