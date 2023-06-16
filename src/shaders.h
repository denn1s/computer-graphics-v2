#pragma once

const char* vertexShaderSource = R"(
    #version 330 core

    layout (location = 0) in vec3 in_position;
    layout (location = 1) in vec3 in_normal;
    layout (location = 2) in vec3 in_color;

    out vec3 Normal;
    out vec3 FragPos;
    out vec3 Color;

    uniform mat4 projection;
    uniform mat4 view;
    uniform mat4 model;

    void main()
    {
        gl_Position = projection * view * model * vec4(in_position, 1.0);
        FragPos = vec3(model * vec4(in_position, 1.0));
        Normal = mat3(transpose(inverse(model))) * in_normal;
        Color = in_color;
    }
)";

const char* fragmentShaderSource = R"(
    #version 330 core

    in vec3 Normal;
    in vec3 FragPos;
    in vec3 Color;

    out vec4 FragColor;

    uniform vec3 lightPosition;

    void main()
    {
        // Ambient lighting
        float ambientStrength = 0.1f;
        vec3 ambient = ambientStrength * vec3(1.0f, 1.0f, 1.0f);

        // Diffuse lighting
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(lightPosition - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * vec3(1.0f, 1.0f, 1.0f);

        vec3 result = (ambient + diffuse) * Color;
        FragColor = vec4(result, 1.0f);
    }
)";
