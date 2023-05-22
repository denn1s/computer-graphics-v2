#include <iostream>
#include <fstream>

// Color struct to represent RGB values
struct Color
{
    unsigned char red;
    unsigned char green;
    unsigned char blue;

    Color(unsigned char r, unsigned char g, unsigned char b)
        : red(r), green(g), blue(b)
    {
    }
};

// Function to write BMP image file
void writeBMP(const std::string& filename, int width, int height, unsigned char* framebuffer)
{
    std::ofstream file(filename, std::ios::binary);

    // BMP file header
    unsigned char fileHeader[] = {
        'B', 'M',  // Signature
        0, 0, 0, 0, // File size (will be filled later)
        0, 0,      // Reserved
        0, 0,      // Reserved
        54, 0, 0, 0 // Pixel data offset
    };

    // BMP info header
    unsigned char infoHeader[] = {
        40, 0, 0, 0,  // Info header size
        0, 0, 0, 0,   // Image width (will be filled later)
        0, 0, 0, 0,   // Image height (will be filled later)
        1, 0,         // Number of color planes
        24, 0,        // Bits per pixel (3 bytes)
        0, 0, 0, 0,   // Compression method
        0, 0, 0, 0,   // Image size (will be filled later)
        0, 0, 0, 0,   // Horizontal resolution (pixels per meter, not used)
        0, 0, 0, 0,   // Vertical resolution (pixels per meter, not used)
        0, 0, 0, 0,   // Number of colors in the palette (not used)
        0, 0, 0, 0    // Number of important colors (not used)
    };

    // Calculate some values
    int imageSize = width * height * 3;  // 3 bytes per pixel (BGR)
    int fileSize = imageSize + sizeof(fileHeader) + sizeof(infoHeader);

    // Fill in the file header
    *(int*)&fileHeader[2] = fileSize;          // File size
    *(int*)&fileHeader[10] = sizeof(fileHeader) + sizeof(infoHeader);  // Pixel data offset

    // Fill in the info header
    *(int*)&infoHeader[4] = width;             // Image width
    *(int*)&infoHeader[8] = height;            // Image height
    *(int*)&infoHeader[20] = imageSize;        // Image size

    // Write the headers to the file
    file.write(reinterpret_cast<char*>(fileHeader), sizeof(fileHeader));
    file.write(reinterpret_cast<char*>(infoHeader), sizeof(infoHeader));

    // Write the pixel data
    file.write(reinterpret_cast<char*>(framebuffer), imageSize);

    // Close the file
    file.close();
}

void clear(unsigned char* framebuffer, int width, int height, const Color& color)
{
    for (int i = 0; i < width * height * 3; i += 3)
    {
        framebuffer[i] = color.blue;
        framebuffer[i + 1] = color.green;
        framebuffer[i + 2] = color.red;
    }
}

void point(unsigned char* framebuffer, int width, int height, int x, int y, const Color& color)
{
    int pixelIndex = (y * width + x) * 3;
    framebuffer[pixelIndex] = color.blue;
    framebuffer[pixelIndex + 1] = color.green;
    framebuffer[pixelIndex + 2] = color.red;
}

void drawGundam(unsigned char* framebuffer, int width, int height)
{
    // Draw the head
    for (int y = 0; y < 100; ++y)
    {
        for (int x = 0; x < 100; ++x)
        {
            point(framebuffer, width, height, x + 100, height - (y + 100) - 1, Color(0, 0, 0));
        }
    }

    // Draw the antennas
    for (int y = 0; y < 50; ++y)
    {
        for (int x = 0; x < 10; ++x)
        {
            point(framebuffer, width, height, x + 140, height - (y + 80) - 1, Color(0, 0, 0));
            point(framebuffer, width, height, x + 160, height - (y + 80) - 1, Color(0, 0, 0));
        }
    }

    // Draw the body
    for (int y = 0; y < 200; ++y)
    {
        for (int x = 0; x < 50; ++x)
        {
            point(framebuffer, width, height, x + 125, height - (y + 200) - 1, Color(255, 0, 0));
        }
    }

    // Draw the legs
    for (int y = 0; y < 150; ++y)
    {
        for (int x = 0; x < 50; ++x)
        {
            point(framebuffer, width, height, x + 125, height - (y + 400) - 1, Color(255, 0, 0));
            point(framebuffer, width, height, x + 225, height - (y + 400) - 1, Color(255, 0, 0));
        }
    }

    // Draw the beam saber
    for (int y = 0; y < 300; ++y)
    {
        for (int x = 0; x < 10; ++x)
        {
            point(framebuffer, width, height, x + 155, height - (y + 100) - 1, Color(0, 255, 255));
        }
    }

    // Draw the shield
    for (int y = 0; y < 150; ++y)
    {
        for (int x = 0; x < 150; ++x)
        {
            point(framebuffer, width, height, x + 50, height - (y + 200) - 1, Color(0, 0, 255));
        }
    }

    // Draw the funnels
    for (int y = 0; y < 50; ++y)
    {
        for (int x = 0; x < 10; ++x)
        {
            point(framebuffer, width, height, x + 50, height - (y + 400) - 1, Color(255, 255, 0));
            point(framebuffer, width, height, x + 75, height - (y + 400) - 1, Color(255, 255, 0));
            point(framebuffer, width, height, x + 100, height - (y + 400) - 1, Color(255, 255, 0));
            point(framebuffer, width, height, x + 125, height - (y + 400) - 1, Color(255, 255, 0));
            point(framebuffer, width, height, x + 150, height - (y + 400) - 1, Color(255, 255, 0));
        }
    }
}


int main()
{
    const int width = 1900;
    const int height = 1080;

    // Create an array to hold the pixel data (framebuffer)
    unsigned char* framebuffer = new unsigned char[width * height * 3];

    // Clear the framebuffer to white (255, 255, 255)
    clear(framebuffer, width, height, Color(255, 255, 255));

    // Calculate the center coordinates
    int centerX = width / 2;
    int centerY = height / 2;

    // Set the center pixel to red (0, 0, 255)
    // point(framebuffer, width, height, centerX, centerY, Color(255, 0, 0));
    drawGundam(framebuffer, width, height);
    
    // Write the image to a BMP file
    writeBMP("image.bmp", width, height, framebuffer);

    // Clean up the memory
    delete[] framebuffer;

    std::cout << "Image generated successfully!" << std::endl;

    return 0;
}
