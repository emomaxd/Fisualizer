#include "Renderer.hpp"

#include <iostream>



void Renderer::drawFunction(const std::vector<Vertex>& points) {
    if (points.empty()) return;

    glPointSize(3);

    GLuint vbo, vao;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(Vertex), points.data(), GL_STATIC_DRAW);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, color)));

    glDrawArrays(GL_POINTS, 0, points.size());

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}

static void drawPoints(const std::vector<Vertex>& points) {
    if (points.empty()) return;

    glPointSize(5);

    GLuint vbo, vao;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(Vertex), points.data(), GL_STATIC_DRAW);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, color)));

    glDrawArrays(GL_POINTS, 0, points.size());

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}

void Renderer::drawCoordinateSystem(float xStart, float yStart, float zStart, float xEnd, float yEnd, float zEnd) {
    glPointSize(5);
    const float step = 0.01f;

    std::vector<Vertex> points;

    glm::vec4 color(0, 0, 0, 1);
 
    // Draw the X-axis (right)
    for (float x = xStart; x <= xEnd; x += step) {
        points.push_back({glm::vec3(x, yStart, zStart), glm::vec4(color) });
    }

    // Draw the Y-axis (top)
    for (float y = yStart; y <= yEnd; y += step) {
        points.push_back({ glm::vec3(xStart, y, zStart), glm::vec4(color) });
    }

    // Draw the X-axis (left)
    for (float x = xStart; x >= -xEnd; x -= step) {
        points.push_back({ glm::vec3(x, yStart, zStart), glm::vec4(color) });
    }

    // Draw the Y-axis (bottom)
    for (float y = yStart; y >= -yEnd; y -= step) {
        points.push_back({ glm::vec3(xStart, y, zStart), glm::vec4(color) });
    }

    // Batch render the points
    drawPoints(points);

}

void Renderer::draw3DLine(const glm::vec3& startPos, float range, const glm::vec4& lineColor) {
    // Define your vertex data with positions and colors
    glm::vec3 positiveEndpoint = startPos + glm::vec3(range, 0.0f, 0.0f);
    glm::vec3 negativeEndpoint = startPos - glm::vec3(range, 0.0f, 0.0f);

    // Define your vertex data with positions and colors
    Vertex vertices[] = {
        {positiveEndpoint, lineColor},  // Positive endpoint with the given color
        {negativeEndpoint, lineColor}   // Negative endpoint with the given color
    };

    // Create vertex and index buffers
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Bind the VAO
    glBindVertexArray(VAO);

    // Bind and buffer vertex data
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Set the vertex attributes (position and color)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));

    // Render loop:
    // Bind shaders, set uniforms, and draw the line
    // Ensure you have a rendering context and shaders set up before calling this function.
    // You can use this function inside your rendering loop.

    glDrawArrays(GL_LINES, 0, 2);

    // Don't forget to clean up the buffers when you're done.
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Renderer::drawCircle(const glm::vec3& center, float radius, const glm::vec4& circleColor) {
    // Define the number of vertices to approximate the circle
    const int numSegments = 30;
    const float segmentAngle = 2 * glm::pi<float>() / numSegments;

    Vertex vertices[numSegments + 2];

    // Center vertex
    vertices[0].position = center;
    vertices[0].color = circleColor;

    // Calculate the vertices for the circle
    for (int i = 1; i <= numSegments + 1; ++i) {
        float angle = segmentAngle * (i - 1);
        vertices[i].position = center + glm::vec3(radius * glm::cos(angle), radius * glm::sin(angle), 0);
        vertices[i].color = circleColor;
    }

    // Create vertex and index buffers
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Bind the VAO
    glBindVertexArray(VAO);

    // Bind and buffer vertex data
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Set the vertex attributes (position and color)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));

    // Render loop:
    // Bind shaders, set uniforms, and draw the circle
    // Ensure you have a rendering context and shaders set up before calling this function.
    // You can use this function inside your rendering loop.

    glDrawArrays(GL_TRIANGLE_FAN, 0, numSegments + 2);

    // Don't forget to clean up the buffers when you're done.
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}



