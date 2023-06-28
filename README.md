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