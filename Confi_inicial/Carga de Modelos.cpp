//Villeda Tlecuitl Jose Eduardo
// Practica 6 Carga de modelos
//Fecha 15/03/25
// 319281998


// Std. Includes
#include <string>

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
const GLuint WIDTH = 800, HEIGHT = 600;
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

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

int main() {
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 6, Carga de modelos - Jose Villeda", nullptr, nullptr);

    if (nullptr == window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();

        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);

    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

    // Set the required callback functions
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);

    // GLFW Options
    //glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if (GLEW_OK != glewInit()) {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    // Define the viewport dimensions
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // OpenGL options
    glEnable(GL_DEPTH_TEST);

    // Setup and compile our shaders
    Shader shader("Shader/modelLoading.vs", "Shader/modelLoading.frag");

    // Load models
    Model dog((char*)"Models/RedDog.obj");

    Model desert((char*)"Models/Desierto/tripo_convert_ad64926a-302a-4f75-bf9e-152e88c3016e.obj");

    Model camel((char*)"Models/Camello/tripo_convert_f5e35d79-3cde-4afb-b061-c75830ee1e2e.obj");

    Model familia((char*)"Models/Familia/tripo_convert_0654c96e-611d-46c4-bb48-4a28ef5e4b7d.obj");

    Model captus((char*)"Models/Captus/tripo_convert_38ffb5cc-29f8-4cc7-b186-93601266c293.obj");

    Model iguana((char*)"Models/Iguana/tripo_convert_7e139703-e2dc-49d9-b4f1-5093681d1302.obj");
    
    Model alacran((char*)"Models/Alacran/tripo_convert_aaf33b86-c178-4995-b815-e40f8490d1a6.obj");
    glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

    // Game loop
    while (!glfwWindowShouldClose(window)) {
        // Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check and call events
        glfwPollEvents();
        DoMovement();

        // Clear the colorbuffer
        glClearColor(0.53f, 0.81f, 0.92f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.Use();

        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        // Draw the loaded model
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

        // Alacran
        glm::mat4 modelAlacran = glm::mat4(1.0f);
        modelAlacran = glm::translate(modelAlacran, glm::vec3(-0.4f, -0.29f, 2.25f));
        modelAlacran = glm::scale(modelAlacran, glm::vec3(0.08f, 0.08f, 0.08f));
        modelAlacran = glm::rotate(modelAlacran, -1.5708f, glm::vec3(0.0f, -12.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelAlacran));
        alacran.Draw(shader);

        

        // Swap the buffers
        glfwSwapBuffers(window);
    }

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
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
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
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos) {
    if (firstMouse) {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

    lastX = xPos;
    lastY = yPos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}

