# README.md

## Table of Contents

1. [Introduction to Lines](#introduction-to-lines)
2. [Bresenham's Line Algorithm](#bresenhams-line-algorithm)
3. [DDA Line Algorithm](#dda-line-algorithm)
4. [Triangles in Computer Graphics](#triangles-in-computer-graphics)
5. [Triangulation in Computer Graphics](#triangulation-in-computer-graphics)

## Introduction to Lines

A line is a straight one-dimensional figure that extends infinitely in both directions. In geometry, a line can be represented by its equation, which is given by:

y = mx + b

Where `m` is the slope of the line, `b` is the y-intercept, and `x` and `y` are the coordinates of any point on the line.

## Bresenham's Line Algorithm

Bresenham's Line Algorithm is an efficient method for drawing lines on a raster grid, such as a computer screen. The algorithm uses integer arithmetic to calculate the points that should be drawn to create a straight line between two given points. This approach minimizes rounding errors and is faster than algorithms that use floating-point arithmetic.

## DDA Line Algorithm

The Digital Differential Analyzer (DDA) Line Algorithm is another method for drawing lines on a raster grid. The algorithm calculates the slope of the line and uses it to incrementally determine the points that should be drawn. Unlike Bresenham's algorithm, the DDA algorithm uses floating-point arithmetic, which can be slower and less precise.

## Triangles in Computer Graphics

Triangles are a fundamental building block in computer graphics because they are the simplest convex polygon that can be used to approximate more complex shapes. Triangles are preferred over other shapes because they can be easily rasterized and transformed using linear algebra operations. Additionally, any polygon can be decomposed into a set of triangles, making them a versatile and efficient representation for complex geometry.

## Triangulation in Computer Graphics

Triangulation is the process of breaking down a complex shape or surface into a set of triangles. This process is essential in computer graphics because it allows for efficient rendering and manipulation of complex models. Triangulation is used in various applications, such as 3D modeling, terrain rendering, and physics simulations. By decomposing a shape into triangles, computer graphics algorithms can take advantage of the simplicity and efficiency of working with triangles to perform complex operations on more intricate shapes.

## Excersises 

Square and "fake" cube: Students can start by drawing a square and a "fake" cube using lines and triangles. They can experiment with different sizes and positions for these shapes.

Grid of lines: Have the students draw a grid made up of horizontal and vertical lines. They can vary the number of rows and columns and the spacing between them.

Regular polygons: Have the students draw regular polygons like an equilateral triangle, pentagon, hexagon, etc. They can calculate the vertices of these polygons and use the line or triangle functions to draw them.

Pattern of triangles: Have the students create a pattern of triangles by drawing multiple triangles in a row, column, or a grid. They can alter the size, orientation, and spacing of the triangles to create interesting patterns.

Star shapes: Have the students draw star shapes using lines or triangles. They can vary the number of points and the length of the lines to create different star patterns.

Spirograph-like patterns: Have the students draw spirograph-like patterns using lines or triangles. They can experiment with the parameters of the spirograph equations to create intricate designs.

Fractal patterns: Have the students draw simple fractal patterns such as the Sierpinski triangle or the Koch snowflake using lines or triangles. These patterns can be generated iteratively by applying simple geometric transformations.

Animating shapes: Have the students animate the shapes they've drawn by changing their position, size, or orientation over time. They can use a timer or a loop to update the shapes and redraw them at each frame.

Interacting with shapes: Have the students write a program that allows them to interact with the shapes they've drawn. They can use mouse clicks or keyboard input to select, move, resize, or change the color of the shapes.

Creating a simple drawing tool: Have the students create a simple drawing tool that allows them to draw lines, triangles, squares, and other shapes on a canvas. They can use mouse input to draw the shapes and keyboard input to switch between different drawing modes or colors.