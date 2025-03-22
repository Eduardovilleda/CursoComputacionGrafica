//Villeda Tlecuitl Jose Eduardo
// Practica 7: Texturizado
//Fecha 16/03/25
// 319281998



#include <iostream>
#include <cmath>

// GLEW - Biblioteca para gestionar extensiones de OpenGL
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Biblioteca para la carga de imágenes
#include "stb_image.h"

// GLM - Biblioteca matemática para gráficos en 3D
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Inclusión de clases personalizadas
#include "Shader.h"
#include "Camera.h"

// Prototipos de funciones
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();

// Dimensiones de la ventana
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Dimensiones de la ventana
Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;

// Posición de la luz en la escena
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

// Variables para el control del tiempo entre cuadros
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

// Función principal
int main() {
    // Init GLFW
    glfwInit();
    // Configuración de la versión de OpenGL y perfil
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Creación de la ventana de OpenGL
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 7: Texturizado - Villeda Tlecuitl Jose Eduardo ", nullptr, nullptr);

    if (nullptr == window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);

    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

    // Configuración de funciones de callback para entrada de usuario
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);

    // GLFW Options
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Inicialización de GLEW
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

    // Creación del shader
    Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");

    // Definición de los vértices y atributos de un objeto 3D
    GLfloat vertices[] = {
        // Posiciones          // Colores (RGB)     // Coordenadas de textura
        // Cara Frontal
        -0.5f, -0.5f, 0.0f,    1.0f, 1.0f, 1.0f,    0.0f, 0.25f,    
        0.5f, -0.5f, 0.0f,     1.0f, 1.0f, 1.0f,    0.25f, 0.25f,   
        0.5f,  0.5f, 0.0f,     1.0f, 1.0f, 1.0f,    0.25f, 0.5f,    
        -0.5f,  0.5f, 0.0f,    1.0f, 1.0f, 1.0f,    0.0f, 0.5f,     

        // Cara derecha
        0.5f, -0.5f, 1.0f,     1.0f, 1.0f, 1.0f,    0.5f, 0.25f,    
        0.5f, 0.5f, 1.0f,      1.0f, 1.0f, 1.0f,    0.5f, 0.5f,     

        // Cara 3 izquierda
        -0.5f, -0.5f, 1.0f,    1.0f, 1.0f, 1.0f,    0.75f, 0.25f,   
        -0.5f, 0.5f, 1.0f,     1.0f, 1.0f, 1.0f,    0.75f, 0.5f,    

        // Cara 4 Trasera
        -0.5f, -0.5f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 0.25f,    
        -0.5f, 0.5f, 0.0f,     1.0f, 1.0f, 1.0f,    1.0f, 0.5f,     

        // Cara Superior
        -0.5f, 0.5f, -0.0f,    1.0f, 1.0f, 1.0f,    0.75f, 0.75f,   
        0.5f, 0.5f, -0.0f,     1.0f, 1.0f, 1.0f,    0.5f, 0.75f,    

        // Cara Inferior
        0.5f, -0.5f, 0.0f,     1.0f, 1.0f, 1.0f,    0.5f, 0.0f,    
        -0.5f, -0.5f, 0.0f,    1.0f, 1.0f, 1.0f,    0.75f, 0.0f    
    };

    GLuint indices[] = {
        // Cara Frontal
        0, 1, 3,
        1, 2, 3,

        // Cara Derecha
        1, 4, 2,
        4, 5, 2,

        // Cara Izquierda
        4, 6, 5,
        6, 7, 5,

        // Cara Trasera
        6, 8, 7,
        8, 9, 7,

        // Cara Superior
        7, 10, 5,
        10, 11, 5,

        // Cara Inferior
        4, 12, 6,
        12, 13, 6
    };


    // Configuración de los buffers de OpenGL
    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Color del atributo
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // Coordenada de textura del atributo
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);

    // Carga del modelo
    GLuint texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    int textureWidth, textureHeight, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* image;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);

    // Mapeo
    image = stbi_load("images/dadodi.png", &textureWidth, &textureHeight, &nrChannels, 0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    if (image)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(image);

    // Bucle principal del juego
    while (!glfwWindowShouldClose(window)) {
        // Calculate deltatime of current frame
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Verifica si algún evento ha sido activado (teclado, mouse, etc.) y llama a las funciones de respuesta correspondientes
        glfwPollEvents();
        DoMovement();

        // Limpia el buffer de color y profundidad
        glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        lampShader.Use();
        // Crea las transformaciones de la cámara
        glm::mat4 view;
        view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);
        glm::mat4 model(1);

        // Obtiene las ubicaciones de las matrices en el shader del objeto iluminado
        GLint modelLoc = glGetUniformLocation(lampShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(lampShader.Program, "view");
        GLint projLoc = glGetUniformLocation(lampShader.Program, "projection");

        // Activa y vincula la textura difusa
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);

        // Establece las matrices en el shader
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        // Dibuja el objeto iluminado utilizando los atributos de los vértices
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Intercambia los buffers de pantalla
        glfwSwapBuffers(window);
    }

    // Elimina los buffers y arrays de vértices
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();

    return 0;
}

// Mueve o altera la posición de la cámara basado en la entrada del usuario
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

// Se llama cada vez que se presiona o suelta una tecla en GLFW
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