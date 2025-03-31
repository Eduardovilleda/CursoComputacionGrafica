////Practica 8 Materiales e Iluminaciones        
//  Villeda Tlecuitl José Eduardo
// 30/03/25                             
// 319281998


// Std. Includes
#include <string>
#include <iostream>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include "SOIL2/SOIL2.h"
#include "stb_image.h"

// Properties
const GLuint WIDTH = 1200, HEIGHT = 650;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

// Light attributes
glm::vec3 lightPos(0.9f, 0.5f, 0.0f);       // Posición del sol
glm::vec3 secondLightPos(0.5f, 0.5f, 0.0f); // Posición de la luna

float reloj = 0.0f; // Reloj para el ciclo día/noche
float movelightPos = 0.0f;
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
float rot = 0.0f;
bool activanim = false;
bool modoDia = true; // Comienza en modo día

int main() {
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 8: Materiales e Iluminacion - Villeda Tlecuitl Jose", nullptr, nullptr);

    if (nullptr == window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

    // Set callback functions
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    if (GLEW_OK != glewInit()) {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    // Define viewport
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glEnable(GL_DEPTH_TEST);

    // Setup shaders
    Shader shader("Shader/modelLoading.vs", "Shader/modelLoading.frag");
    Shader lampshader("Shader/lamp.vs", "Shader/lamp.frag");
    Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");

    // Load models
    Model dog((char*)"Models/RedDog.obj");

    Model desert((char*)"Models/Desierto/tripo_convert_ad64926a-302a-4f75-bf9e-152e88c3016e.obj");

    Model camel((char*)"Models/Camello/tripo_convert_f5e35d79-3cde-4afb-b061-c75830ee1e2e.obj");

    Model familia((char*)"Models/Familia/tripo_convert_0654c96e-611d-46c4-bb48-4a28ef5e4b7d.obj");

    Model captus((char*)"Models/Captus/tripo_convert_38ffb5cc-29f8-4cc7-b186-93601266c293.obj");

    Model iguana((char*)"Models/Iguana/tripo_convert_7e139703-e2dc-49d9-b4f1-5093681d1302.obj");

    Model Alacran((char*)"Models/Alacran/tripo_convert_aaf33b86-c178-4995-b815-e40f8490d1a6.obj");

    Model luna((char*)"Models/Una/tripo_convert_2ed32cdb-5128-4834-9316-d7e660e6a7fe.obj");

    Model moon((char*)"Models/Luna/Moon.obj");

    Model sun((char*)"Models/Sol/Sun.obj");

    glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

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

    // First, set the container's VAO (and VBO)
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

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
        //glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Set lighting shader
        lightingShader.Use();
        GLint lightPosLoc = glGetUniformLocation(lightingShader.Program, "light.position");
        GLint lightPosLoc2 = glGetUniformLocation(lightingShader.Program, "light2.position");
        GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");

        // Movimiento del sol
        lightPos.x = cos(reloj) * 5.0f; // Movimiento en el eje X
        lightPos.y = sin(reloj) * 5.0f; // Movimiento en el eje Y

        // Movimiento de la luna (opuesto al sol)
        secondLightPos.x = -cos(reloj) * 5.0f; // Movimiento en el eje X
        secondLightPos.y = -sin(reloj) * 5.0f; // Movimiento en el eje Y

        // Set light positions and view pos
        glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
        glUniform3f(lightPosLoc2, secondLightPos.x, secondLightPos.y, secondLightPos.z);
        glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

        // Dia 
        if (modoDia) {
            // Sun properties (warm light)
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.ambient"), 0.4f, 0.4f, 0.3f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.diffuse"), 1.0f, 0.8f, 0.6f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.specular"), 0.8f, 0.6f, 0.4f);
        }
        else {
            // Turn off sun
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.ambient"), 0.0f, 0.0f, 0.0f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.diffuse"), 0.0f, 0.0f, 0.0f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.specular"), 0.0f, 0.0f, 0.0f);
        }

        // Noche
        if (!modoDia) {
            // Moon properties (cool light)
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.ambient"), 0.1f, 0.1f, 0.2f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.diffuse"), 0.6f, 0.7f, 0.9f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.specular"), 0.5f, 0.6f, 0.7f);
        }
        else {
            // Turn off moon
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.ambient"), 0.0f, 0.0f, 0.0f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.diffuse"), 0.0f, 0.0f, 0.0f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.specular"), 0.0f, 0.0f, 0.0f);
        }

        // Set projection and view matrices
        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        // Set material properties
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.ambient"), 0.2f, 0.2f, 0.2f); // Gris oscuro
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0.8f, 0.75f, 0.7f); // Ligero tono cálido
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.4f, 0.4f, 0.4f); // Reflejos moderados
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 100.0f); // Brillantez moderada

        

        // Alacran
        glm::mat4 model(1);
        model = glm::translate(model, glm::vec3(-0.4f, -0.5f, 2.25f));
        model = glm::scale(model, glm::vec3(0.08f, 0.08f, 0.08f));
        model = glm::rotate(model, -1.5708f, glm::vec3(0.0f, -12.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        Alacran.Draw(shader);

        // Dog
        glm::mat4 modelDog = glm::mat4(1.0f);
        modelDog = glm::translate(modelDog, glm::vec3(0.0f, -0.41f, 2.0f));
        modelDog = glm::scale(modelDog, glm::vec3(0.4f, 0.4f, 0.4f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelDog));
        dog.Draw(shader);

        // Desierto
        glm::mat4 modelDesert = glm::mat4(1.0f);
        modelDesert = glm::translate(modelDesert, glm::vec3(0.0f, -0.1f, 1.0f));
        modelDesert = glm::scale(modelDesert, glm::vec3(4.5f, 4.5f, 4.5f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelDesert));
        desert.Draw(shader);

        // Camello
        glm::mat4 modelCamello = glm::mat4(1.0f);
        modelCamello = glm::translate(modelCamello, glm::vec3(-1.3f, -0.35f, 1.1f));
        modelCamello = glm::scale(modelCamello, glm::vec3(0.5f, 0.5f, 0.5f));
        modelCamello = glm::rotate(modelCamello, 1.5708f, glm::vec3(0.0f, -12.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelCamello));
        camel.Draw(shader);

        // Familia
        glm::mat4 modelFamilia = glm::mat4(1.0f);
        modelFamilia = glm::translate(modelFamilia, glm::vec3(0.0f, -0.34f, 1.8f));
        modelFamilia = glm::scale(modelFamilia, glm::vec3(0.5f, 0.5f, 0.5f));
        modelFamilia = glm::rotate(modelFamilia, 1.5708f, glm::vec3(0.0f, -12.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelFamilia));
        familia.Draw(shader);

        // Captus
        glm::mat4 modelCaptus = glm::mat4(1.0f);
        modelCaptus = glm::translate(modelCaptus, glm::vec3(1.0f, -0.29f, 1.0f));
        modelCaptus = glm::scale(modelCaptus, glm::vec3(0.45f, 0.45f, 0.45f));
        modelCaptus = glm::rotate(modelCaptus, 1.5708f, glm::vec3(0.0f, -12.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelCaptus));
        captus.Draw(shader);

        // Iguana
        glm::mat4 modelIguana = glm::mat4(1.0f);
        modelIguana = glm::translate(modelIguana, glm::vec3(0.4f, -0.5f, 2.25f));
        modelIguana = glm::scale(modelIguana, glm::vec3(0.18f, 0.18f, 0.18f));
        modelIguana = glm::rotate(modelIguana, -1.5708f, glm::vec3(0.0f, -12.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelIguana));
        iguana.Draw(shader);

        // Render sun
        glm::mat4 modelSun = glm::mat4(1.0f);
        modelSun = glm::translate(modelSun, lightPos);
        modelSun = glm::scale(modelSun, glm::vec3(0.7f));
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelSun));
        sun.Draw(lampshader);

        // Render moon
        glm::mat4 modelMoon = glm::mat4(1.0f);
        modelMoon = glm::translate(modelMoon, secondLightPos);
        modelMoon = glm::scale(modelMoon, glm::vec3(1.0f));
        modelMoon = glm::scale(modelMoon, glm::vec3(0.4));
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelMoon));
        moon.Draw(lampshader);

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

    if (activanim) {
        if (rot > -90.0f)
            rot -= 0.1f;
    }
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action) {
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

    // Controls for sun position
    if (keys[GLFW_KEY_R]) {
        reloj += 0.01f;  //Incrementa
    }
    if (keys[GLFW_KEY_F]) {
        reloj -= 0.01f;  // Decrementa
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


