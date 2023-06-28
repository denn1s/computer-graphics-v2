# Framebuffer and SDL Example

This repository contains a simple example of using a framebuffer and the Simple DirectMedia Layer (SDL) library to draw points on a screen.

## Framebuffer

A framebuffer is a memory buffer containing a complete frame of pixel data, which can be sent to a display device to be rendered on the screen. It stores the color information for each pixel on the screen, usually in a two-dimensional array. Framebuffers are commonly used in computer graphics to draw shapes, apply shading, perform transformations, and manipulate images before displaying them on the screen.

In our example, we use a `Framebuffer` class that encapsulates a two-dimensional array of `Color` objects. This framebuffer is used to store color information for each pixel on the screen, and it is updated whenever we draw points or other shapes.

## SDL (Simple DirectMedia Layer)

SDL is a cross-platform development library that provides low-level access to audio, keyboard, mouse, joystick, and graphics hardware via OpenGL and Direct3D. It is widely used in game development, multimedia applications, and emulators.

In our example, we use SDL to create a window, initialize a renderer, and handle events such as window closing. We also use SDL to render the contents of our framebuffer to the screen.

## Using SDL to Draw on the Screen

In the example, we create a simple application that uses a framebuffer to draw points on the screen. We initialize SDL, create a window and renderer, and update the framebuffer with pixel colors. We then create a texture from the framebuffer data and render it to the screen using the SDL renderer.

In an alternative version of the example, we use SDL's built-in `SDL_RenderDrawPoint` function to draw points directly on the renderer without using a framebuffer. However, this approach may result in slower performance when drawing a large number of points.

## Video Memory and Framebuffer

Video memory, also known as VRAM (Video Random Access Memory), is a special type of memory used by graphics processing units (GPUs) to store image data and other information needed for rendering graphics. Video memory is separate from the main system memory (RAM) and is typically faster and optimized for graphics operations.

The framebuffer is stored in video memory, as it needs to be quickly accessible by the GPU for rendering. When a frame is ready to be displayed, the GPU reads the contents of the framebuffer and sends the pixel data to the display device, which then renders the image on the screen. The process of reading from the framebuffer and displaying the image is called a "buffer swap" or "frame swap".

In our example, the framebuffer is stored in system memory and is later copied to video memory when creating an SDL texture. This allows us to modify the framebuffer contents directly in system memory before updating the screen. However, modern graphics APIs such as OpenGL and Direct3D often store framebuffers directly in video memory for better performance.

## Excercises

Here are some simple exercises to familiarize with the framebuffer.

1. **Draw a line**: Create a function that takes two points (x1, y1) and (x2, y2) as input and draws a straight line between them on the framebuffer using a specified color. Students can learn about line drawing algorithms like Bresenham's or DDA.

2. **Draw a rectangle**: Create a function that takes the top-left corner coordinates (x, y), width, height, and a color as input, and draws a filled or outlined rectangle on the framebuffer.

3. **Draw a circle**: Create a function that takes the center coordinates (x, y), radius, and a color as input, and draws a filled or outlined circle on the framebuffer. Students can learn about the Midpoint Circle Algorithm.

4. **Flood fill**: Implement a flood fill algorithm that takes a starting point (x, y) and a target color as input, and fills an enclosed area with the target color. Students can explore recursive and iterative implementations (e.g. using a queue).

5. **Simple shapes**: Create a function that takes a set of vertices, and draws a polygon (e.g., triangle, hexagon) on the framebuffer by connecting the vertices with lines.

6. **Pattern generation**: Create functions that generate simple patterns on the framebuffer, such as checkerboards, stripes, or radial gradients.

7. **Image loading**: Load an image file (e.g., PNG or BMP) and display it on the framebuffer. Students can learn about file formats, file I/O, and image decoding.

8. **Basic animation**: Create a simple animation by updating the framebuffer in a loop, such as a bouncing ball, a moving line, or a rotating shape.

9. **User interaction**: Add keyboard or mouse input to control elements on the framebuffer, such as moving a shape, changing colors, or toggling the visibility of elements.
