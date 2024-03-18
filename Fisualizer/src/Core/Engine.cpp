#include "Engine.hpp"

#include <iostream>
#include <thread>
#include <cmath>

#include "Parser.hpp"

#include <gtc/matrix_transform.hpp>
#include <ext.hpp>


Engine::Engine() {
    this->window = new Window();
}

Engine::~Engine(){

    std::cout << "ENGINE DESTRUCTED\n";

    delete window;

}
bool canProcessFunction = true;

glm::vec3 startingTextPosition(0, 0, 0);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_ENTER) {
            canProcessFunction = true;
        }
        

    }
    ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
    
}

static void scrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
    if (yOffset > 0.0f && globalPosition.z > 5.0f) {
        globalPosition.z += 1.2f * -yOffset;
    }
    if (yOffset < 0.0f && globalPosition.z < 98.0f) {
        globalPosition.z += 1.2f * -yOffset;
    }
    ImGui_ImplGlfw_ScrollCallback(window, xOffset, yOffset);
}

bool isMousePressed = false;

static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
        if (action == GLFW_PRESS) {
            
            isMousePressed = true;
        }
        else if (action == GLFW_RELEASE) {
            
            isMousePressed = false;
        }
    }
    ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);

}

static void cursorPositionCallback(GLFWwindow* window, double xPos, double yPos) {
    static double lastX = xPos;
    double xOffset = xPos - lastX;

    static double lastY = yPos;
    double yOffset = yPos - lastY;

    // camera z -> 50
    // slide speed -> 0.02f
    const float slideSpeed =  globalPosition.z / 1000 ;

    if (isMousePressed) {
        if (xOffset != 0.0f) {
            globalPosition.x += slideSpeed * -xOffset;
            globalCameraTarget.x += slideSpeed * -xOffset;
        }
        if (yOffset != 0.0f) {
            globalPosition.y += slideSpeed * yOffset;
            globalCameraTarget.y += slideSpeed * yOffset;
        }
    }
    lastX = xPos;
    lastY = yPos;
    ImGui_ImplGlfw_CursorPosCallback(window, xPos, yPos);
}

inline bool isCloseToZero(float value, float tolerance = 1e-6) { return std::abs(value) < tolerance; }


std::vector<std::pair<std::string, glm::vec3>> texts;

static void findExtremePoints(const std::string& function, std::vector<Vertex>& destination, float minX, float maxX) {

    destination.clear();

    const float step = 0.0001f;

    const float h = 0.0001f;

    auto fn = Parser::Parse(function);


    glm::vec4 extremePointColor(0, 1, 0, 1);

    for (float x = minX; x < maxX; x += step) {
        
        float y = fn(x);
        float derivative = (fn(x + h) - fn(x)) / h;

        if (isCloseToZero(derivative, 1e-6)) {
            //std::cout << "Extreme point found on X = " << x << std::endl;

            bool updated = false;

            // Iterate through the coordinates vector
            for (auto& coord : destination) {
                if (coord.position.x == x ) {
                    // Update the existing element in coordinates
                    coord.position.y = y;
                    coord.color = glm::vec4(0, 1, 0, 1);
                    
                    
                    updated = true;
                    break;
                }
            }

            if (!updated) {
                // If the element wasn't found, add a new one
                //std::cout << "new EXTREME POINT pushed to the vector\n";
                destination.push_back({ glm::vec3(x, y, 0), extremePointColor });
            }
            texts.push_back({ std::to_string(y), { 0, y, 0 } });
        }
    }
}

static void findRoots(const std::string& function, std::vector<Vertex>& destination, float minX, float maxX) {
    
    destination.clear();

    const float step = 0.0001f;

    auto fn = Parser::Parse(function);


    glm::vec4 rootColor(0, 0, 1, 1);

    for (float x = minX; x < maxX; x += step) {
        float y = fn(x);
        //x = std::round(x * 100.0f) / 100.0f;
        
        //y = std::round(x * 10.0f) / 10.0f;
        if ( x == 0.0f || isCloseToZero(y, 1e-2)) {
            //std::cout << "Found a root on X = " << x << std::endl;
            bool updated = false;

            // Iterate through the coordinates vector
            for (auto& coord : destination) {
                if (coord.position.x == x) {
                    // Update the existing element in coordinates
                    coord.position.y = y;
                    coord.color = rootColor;
                    
                    
                    updated = true;
                    break;
                }
            }

            if (!updated) {
                // If the element wasn't found, add a new one
                //std::cout << "new ROOT pushed to the vector\n";
                destination.push_back({ glm::vec3(x, y, 0), rootColor });
            }

            // if x is equal 0 render the y value on the screen
            if(x == 0)
                texts.push_back({ std::to_string(y), { 0, y, 0 } });

        }
    }
    
    
}

static void processFunction(const std::string& function, std::vector<Vertex>& coordinates, float minX, float maxX) {

    constexpr float step = 0.0001f;

    coordinates.clear();

    auto expression = Parser::Parse(function);


    for (float x = minX; x <= maxX; x += step) {
        float y = expression(x);
        coordinates.push_back({ glm::vec3(x, y, 0), glm::vec4(1, 1, 1, 0.8f) });
    }

}


static void drawCircleOnRoots(const std::vector<Vertex>& coordinates, float radius, const glm::vec4& color) {
    for (auto& e : coordinates)
        Renderer::drawCircle(e.position, radius, color);
}
static void drawLineOnExtremes(const std::vector<Vertex>& coordinates, float range, const glm::vec4& color) {
    for (auto& e : coordinates)
        Renderer::draw3DLine(e.position, range, color);
}


static void turnTextPositionsIntoString(std::vector<std::pair<std::string, glm::vec3>>& destination, float range) {
    texts.clear();
    float curr = -range;
    float gap = globalPosition.z / 25.0f;
    float currPos = -range;
    int precision = 2;

    while (curr <= range) {

        std::string text = "";
        if (48 + curr < 48)
            precision = 3;


        else
            precision = 2;

        // x positions pushed
        destination.push_back({ std::to_string(curr).substr(0, precision + 1), {curr, 0, 0} });


        curr += gap;
        currPos += gap;
    }
}

static void drawText(TextRenderer& tr, float scale) {
    tr.RenderTextBatch(texts, scale, {1, 1, 1});
}

static void drawCoordinateTexts(TextRenderer& textRenderer, float range, float textSize) {
    texts.clear();
    float curr = -range;
    float gap = globalPosition.z / 30.0f;
    float pos = -range;
    int precision = 2;
     
    while (curr <= range) {
        std::string text = "";
        if (48 + curr < 48)
            precision = 3;


        else
            precision = 2;

        text += std::to_string(curr).substr(0, precision + 1);

        textRenderer.RenderText(text, startingTextPosition.x + pos, startingTextPosition.y, textSize, glm::vec3(1.0, 1.0f, 1.0f));

        curr += gap;
        pos += gap;
    }
    
}

void Engine::start(){
    
    auto GLFWwindow = window->getWindow();
    

 
    glfwSetKeyCallback(GLFWwindow, key_callback);
    glfwSetScrollCallback(GLFWwindow, scrollCallback);
    glfwSetMouseButtonCallback(GLFWwindow, mouseButtonCallback);
    glfwSetCursorPosCallback(GLFWwindow, cursorPositionCallback);



    Shader shader("src/Renderer/Shader/BasicShader.glsl");
    shader.Bind();

    shader.AddUniform("color");
    shader.AddUniform("Matrix");

    Shader textShader("src/Renderer/Shader/text.shader");

    TextRenderer textRenderer("Assets/rainyhearts.ttf", textShader);
    
    
    // Model Matrix initialization
    glm::mat4 rotationMatrix(1);
    glm::mat4 scaleMatrix(1);
    glm::mat4 translationMatrix(1);
    glm::mat4 modelMatrix;

    glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), WIDTH / HEIGHT, 1.0f, 100.0f);

    // Camera setup
    /// In the "RenderingDefinitons.hpp" file

    glm::mat4 viewMatrix;

    glm::mat4 combinedMatrix(1);


    static float range = 5.0f;

    std::string function = "x^3 - 4x";

    std::vector<Vertex> coordinates;
    std::vector<Vertex> rootCoordinates;
    std::vector<Vertex> extremePointCoordinates;



    std::string previousFunction = "";
    float previousRange = -1;

    ImGuiIO& io = ImGui::GetIO();
    

    while (!glfwWindowShouldClose(GLFWwindow))
    {
        float textSize = 2 * globalPosition.z / 5000;

        // if user presses "ENTER"
        if ( canProcessFunction ) {
            processFunction(function, coordinates, -range, range);
            findRoots(function, rootCoordinates, -range, range);
            findExtremePoints(function, extremePointCoordinates, -range, range);
            //turnTextPositionsIntoString(texts, range);
            canProcessFunction = false;
            previousFunction = function;
            previousRange = range;
        }

        previousRange = range;


        window->ImGuiNewFrame();


        window->ClearScreen();



        shader.Bind();
        cameraPosition = globalPosition;
        cameraTarget   = globalCameraTarget;
        
        viewMatrix     = glm::lookAt(cameraPosition, cameraTarget, cameraUp);
        
        modelMatrix    = translationMatrix * rotationMatrix * scaleMatrix;

        combinedMatrix = projectionMatrix * viewMatrix * modelMatrix;
        
        shader.setMat4   ("Matrix", &combinedMatrix);


        Renderer::drawCoordinateSystem(0, 0, 0, 100, 100, 0);
        
        
        Renderer::drawFunction(coordinates);

        drawCircleOnRoots(rootCoordinates, 2*globalPosition.z / 500, {0.6f, 0.5f, 0.5f, 1.0f});
        drawLineOnExtremes(extremePointCoordinates, 0.5f, { 0.4f, 0.2f, 0.2f, 0.5f });

        // IMGUI

            ImGui::Begin("Control Panel");       
            
            if (ImGui::SliderFloat("Render Range", &range, 0.0f, 10.0f));
            

            char inputTextBuffer[256] = "";

            strcpy_s(inputTextBuffer, function.c_str());
            
            

            if (ImGui::InputText("Enter Function", inputTextBuffer, sizeof(inputTextBuffer))) {
                // This block will execute when the user enters text and presses Enter.
                // inputTextBuffer contains the text entered by the user.
                function = inputTextBuffer;
                if (function[function.size() - 1] == '*')
                    canProcessFunction = true;
            }


            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            
            ImGui::End();
            
        // IMGUI
        // cameraZ : 50 size : 0.02f
        
    
        drawCoordinateTexts(textRenderer ,range, textSize);
        //drawText(textRenderer, textSize);

        window->ImGuiRender();

        window->SwapBuffers();
    } 

    window->Cleanup();

}