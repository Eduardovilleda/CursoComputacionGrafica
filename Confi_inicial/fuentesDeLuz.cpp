//Practica 9: Fuentes de Luz
//Entrega: 06/04/2025
//Villeda Tlecuitl José Eduardo
// 422090638

#include <iostream>
#include <cmath>
#include <string>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"
#include "SOIL2/SOIL2.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();

// Window dimensions
const GLuint WIDTH = 1200, HEIGHT = 650;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;

// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active;
float reloj = 0.0f;
bool modoDia = true;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
   glm::vec3(0.5f, -0.2f, 1.25f),    // Luz de la fogata (posición 0)
   glm::vec3(0.0f, 0.0f, 0.0f),      // Luz adicional 1
   glm::vec3(0.0f, 0.0f, 0.0f),      // Luz adicional 2
   glm::vec3(0.0f, 0.0f, 0.0f)       // Luz adicional 3
};

// Cube vertices for light sources
float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

glm::vec3 Light1 = glm::vec3(0);

// Deltatime
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

int main() {
    // Init GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create window
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 9: Fuentes de Luz - Villeda Tlecuitl Jose Eduardo", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

    // Set callbacks
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    if (GLEW_OK != glewInit()) {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    // Configure global OpenGL state
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glEnable(GL_DEPTH_TEST);

    // Setup shaders
    Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
    Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");

    // Load models 
    Model dog((char*)"Models/RedDog.obj");
    Model desert((char*)"Models/Desierto/tripo_convert_ad64926a-302a-4f75-bf9e-152e88c3016e.obj");
    Model camel((char*)"Models/Camello/tripo_convert_f5e35d79-3cde-4afb-b061-c75830ee1e2e.obj");
    Model familia((char*)"Models/Familia/tripo_convert_0654c96e-611d-46c4-bb48-4a28ef5e4b7d.obj");
    Model captus((char*)"Models/Captus/tripo_convert_38ffb5cc-29f8-4cc7-b186-93601266c293.obj");
    Model iguana((char*)"Models/Iguana/tripo_convert_7e139703-e2dc-49d9-b4f1-5093681d1302.obj");
    Model alacran((char*)"Models/Alacran/tripo_convert_aaf33b86-c178-4995-b815-e40f8490d1a6.obj");
    Model fogata((char*)"Models/Fogata/tripo_convert_fce180ce-9a90-4552-986d-6265833fe494.obj");

    // Setup vertex data and buffers
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    // Set texture units
    lightingShader.Use();
    glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);
    glUniform1i(glGetUniformLocation(lightingShader.Program, "material.specular"), 1);

    glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

    // Game loop
    while (!glfwWindowShouldClose(window)) {
        // Calculate delta time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check events
        glfwPollEvents();
        DoMovement();

        // Clear buffers
        if (modoDia) {
            glClearColor(0.1f, 0.2f, 0.3f, 1.0f); // Day sky color
        }
        else {
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  // Night sky color
        }
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use lighting shader
        lightingShader.Use();

        // Set view position
        GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
        glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

        // Directional light (luna para modo noche)
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.05f, 0.05f, 0.1f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.2f, 0.2f, 0.3f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.2f, 0.2f, 0.3f);

        // Configuración especial para la luz de la fogata (índice 0)
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"),
            pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"),
            0.2f, 0.1f, 0.0f);  // Ambiente cálido para la fogata
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"),
            1.0f, 0.5f, 0.0f);  // Luz difusa naranja para la fogata
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"),
            1.0f, 0.7f, 0.3f);  // Especular cálido para la fogata
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.09f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.032f);

        // Otras luces (menos intensas)
        for (GLuint i = 1; i < 4; i++) {
            std::string number = std::to_string(i);
            glUniform3f(glGetUniformLocation(lightingShader.Program, ("pointLights[" + number + "].position").c_str()),
                pointLightPositions[i].x, pointLightPositions[i].y, pointLightPositions[i].z);
            glUniform3f(glGetUniformLocation(lightingShader.Program, ("pointLights[" + number + "].ambient").c_str()),
                0.02f, 0.02f, 0.02f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, ("pointLights[" + number + "].diffuse").c_str()),
                0.3f, 0.3f, 0.3f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, ("pointLights[" + number + "].specular").c_str()),
                0.4f, 0.4f, 0.4f);
            glUniform1f(glGetUniformLocation(lightingShader.Program, ("pointLights[" + number + "].constant").c_str()), 1.0f);
            glUniform1f(glGetUniformLocation(lightingShader.Program, ("pointLights[" + number + "].linear").c_str()), 0.09f);
            glUniform1f(glGetUniformLocation(lightingShader.Program, ("pointLights[" + number + "].quadratic").c_str()), 0.032f);
        }

        // SpotLight (flashlight)
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"),
            camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"),
            camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.5f, 0.5f, 0.5f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.5f, 0.5f, 0.5f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.09f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.032f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(15.0f)));

        // Material properties
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 28.0f);

        // Create camera transformations
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projectionMatrix = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

        // Get the uniform locations
        GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
        GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

        // Pass the matrices to the shader
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

        // Dog
        glm::mat4 modelDog = glm::mat4(1.0f);
        modelDog = glm::translate(modelDog, glm::vec3(0.0f, -0.41f, 2.0f));
        modelDog = glm::scale(modelDog, glm::vec3(0.4f, 0.4f, 0.4f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelDog));
        dog.Draw(lightingShader);

        // Desierto
        glm::mat4 modelDesert = glm::mat4(1.0f);
        modelDesert = glm::translate(modelDesert, glm::vec3(0.0f, -0.1f, 1.0f));
        modelDesert = glm::scale(modelDesert, glm::vec3(4.5f, 4.5f, 4.5f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelDesert));
        desert.Draw(lightingShader);

        // Camello
        glm::mat4 modelCamello = glm::mat4(1.0f);
        modelCamello = glm::translate(modelCamello, glm::vec3(-1.3f, -0.35f, 1.1f));
        modelCamello = glm::scale(modelCamello, glm::vec3(0.5f, 0.5f, 0.5f));
        modelCamello = glm::rotate(modelCamello, 1.5708f, glm::vec3(0.0f, -12.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelCamello));
        camel.Draw(lightingShader);

        // Familia
        glm::mat4 modelFamilia = glm::mat4(1.0f);
        modelFamilia = glm::translate(modelFamilia, glm::vec3(0.0f, -0.34f, 1.8f));
        modelFamilia = glm::scale(modelFamilia, glm::vec3(0.5f, 0.5f, 0.5f));
        modelFamilia = glm::rotate(modelFamilia, 1.5708f, glm::vec3(0.0f, -12.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelFamilia));
        familia.Draw(lightingShader);

        // Captus
        glm::mat4 modelCaptus = glm::mat4(1.0f);
        modelCaptus = glm::translate(modelCaptus, glm::vec3(1.0f, -0.29f, 1.0f));
        modelCaptus = glm::scale(modelCaptus, glm::vec3(0.45f, 0.45f, 0.45f));
        modelCaptus = glm::rotate(modelCaptus, 1.5708f, glm::vec3(0.0f, -12.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelCaptus));
        captus.Draw(lightingShader);

        // Iguana
        glm::mat4 modelIguana = glm::mat4(1.0f);
        modelIguana = glm::translate(modelIguana, glm::vec3(0.4f, -0.5f, 2.25f));
        modelIguana = glm::scale(modelIguana, glm::vec3(0.18f, 0.18f, 0.18f));
        modelIguana = glm::rotate(modelIguana, -1.5708f, glm::vec3(0.0f, -12.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelIguana));
        iguana.Draw(lightingShader);

        // Alacran
        glm::mat4 modelAlacran = glm::mat4(1.0f);
        modelAlacran = glm::translate(modelAlacran, glm::vec3(-0.4f, -0.29f, 2.25f));
        modelAlacran = glm::scale(modelAlacran, glm::vec3(0.08f, 0.08f, 0.08f));
        modelAlacran = glm::rotate(modelAlacran, -1.5708f, glm::vec3(0.0f, -12.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelAlacran));
        alacran.Draw(lightingShader);

        // Fogata
        glm::mat4 modelFogata = glm::mat4(1.0f);
        modelFogata = glm::translate(modelFogata, glm::vec3(0.69f, -0.4f, 2.45f));
        modelFogata = glm::scale(modelFogata, glm::vec3(0.4f, 0.4f, 0.4f));
        modelFogata = glm::rotate(modelFogata, -1.5708f, glm::vec3(0.0f, -12.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelFogata));
        fogata.Draw(lightingShader);

        // Dibujar el cubo de luz de la fogata
        lampShader.Use();
        glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glm::mat4 modelLamp = glm::mat4(1.0f);
        modelLamp = glm::translate(modelLamp, pointLightPositions[0]);  // Usamos la posición de la fogata
        modelLamp = glm::scale(modelLamp, glm::vec3(0.05f, 0.05f, 0.05f));
        glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelLamp));
        glUniform3f(glGetUniformLocation(lampShader.Program, "lightColor"), 1.0f, 0.5f, 0.0f);  // Color naranja para la fogata
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        // Swap buffers
        glfwSwapBuffers(window);
    }

    // Clean up
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement() {
    // Camera controls
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP]) {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN]) {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT]) {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT]) {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }

    // Light position controls
    if (keys[GLFW_KEY_R]) {
        pointLightPositions[0].x += 0.01f;
    }
    if (keys[GLFW_KEY_F]) {
        pointLightPositions[0].x -= 0.01f;
    }
    if (keys[GLFW_KEY_T]) {
        pointLightPositions[0].y += 0.01f;
    }
    if (keys[GLFW_KEY_G]) {
        pointLightPositions[0].y -= 0.01f;
    }
    if (keys[GLFW_KEY_Y]) {
        pointLightPositions[0].z -= 0.1f;
    }
    if (keys[GLFW_KEY_H]) {
        pointLightPositions[0].z += 0.01f;
    }
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS) {
            keys[key] = true;
        }
        else if (action == GLFW_RELEASE) {
            keys[key] = false;
        }
    }

    if (keys[GLFW_KEY_N] && action == GLFW_PRESS) {
        modoDia = !modoDia;
    }
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos) {
    if (firstMouse) {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;

    lastX = xPos;
    lastY = yPos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}
