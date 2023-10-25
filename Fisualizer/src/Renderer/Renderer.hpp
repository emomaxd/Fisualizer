#pragma once

#include "../../Dependencies/GLEW/include/GL/glew.h"
#include "../../Dependencies/GLFW/include/GLFW/glfw3.h"

#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "Shader.hpp"
#include "IndexBuffer.hpp"

#include "TextRenderer.hpp"


#include <glm.hpp>

#include <iostream>


struct Vertex {
    glm::vec3 position;
    glm::vec4 color;
};


class Renderer{
public:

    Renderer() = delete;
  

    
    static void drawCoordinateSystem(float xStart, float yStart, float zStart, float xEnd, float yEnd, float zEnd);

    static void drawFunction(const std::vector<Vertex>& points);

    static void draw3DLine(const glm::vec3& startPos, float range, const glm::vec4& color);

    static void drawCircle(const glm::vec3& center, float radius, const glm::vec4& circleColor);


    
   
};