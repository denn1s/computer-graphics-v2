#include <iostream>
#include <fstream>
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
        int pixelIndex = (static_cast<int>(y) * renderer->width + static_cast<int>(x)) * 3;
        renderer->framebuffer[pixelIndex] = color.blue;
        renderer->framebuffer[pixelIndex + 1] = color.green;
        renderer->framebuffer[pixelIndex + 2] = color.red;

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

// Function to render the scene using the renderer
void render(Renderer* renderer)
{
    const Color whiteColor(255, 255, 255);
    const Color redColor(255, 0, 0);
    const Color blueColor(0, 0, 255);

    clear(renderer, whiteColor);

    glm::ivec2 center(renderer->width / 2, renderer->height / 2);
    point(renderer, center, blueColor);

    glm::ivec2 start(100, 100);
    glm::ivec2 end(250, 150);
    line(renderer, start, end, redColor);
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
