#pragma once

const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;

    out vec3 Normal;
    out vec3 FragPos;
    out vec3 LightPos;

    uniform mat4 projection;
    uniform mat4 view;
    uniform mat4 model;

    uniform vec3 lightPosition; // Position of the light source in world coordinates

    void main()
    {
        gl_Position = projection * view * model * vec4(aPos, 1.0);
        FragPos = vec3(model * vec4(aPos, 1.0));
        Normal = normalize(vec3(model * vec4(aPos, 0.0))); // Use the vertex position as normal
        LightPos = lightPosition;
    }
)";

const char* fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;

    in vec3 Normal;
    in vec3 FragPos;
    in vec3 LightPos;

    void main()
    {
        // Ambient light
        float ambientStrength = 0.1;
        vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);  // white
        vec3 ambient = ambientStrength * lightColor;

        // Diffuse light
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(LightPos - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lightColor;

        // Color selection based on "height" (y-coordinate)
        vec3 objectColorLand = vec3(0.13f, 0.55f, 0.13f);  // forest green
        vec3 objectColorSea = vec3(0.0f, 0.0f, 1.0f);  // blue
        vec3 objectColor = FragPos.y > 0.0 ? objectColorLand : objectColorSea;
        
        vec3 result = (ambient + diffuse) * objectColor;
        FragColor = vec4(result, 1.0);
    }

)";