#include <iostream>
#include <string>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Log.h"
#include "Texture.h"
#include "Camera.h"
#include "model/Vertex.h"
#include "model/Mesh.h"

int main() {

    GLFWwindow* window;

    if (!glfwInit()) {
        LOG::Print("Failed to initialize glfw", ERROR);
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    window = glfwCreateWindow(1024, 768, "Terrain", nullptr, nullptr);
    if(!window) {
        LOG::Print("Failed to create glfw window", ERROR);
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        LOG::Print((char*) "Failed to initialize glew", ERROR);
        glfwTerminate();
        return -1;
    };

    glEnable(GL_DEPTH_TEST);
    
    unsigned int shaderProgram  = glCreateProgram();
    unsigned int vertexShader   = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    const char* vertexSource = R"glsl( 
        #version 410

        layout(location = 0) in vec3 position;
        layout(location = 1) in vec2 texcoord;

        out vec2 Texcoord;

        uniform mat4 MVP;

        void main() {
            gl_Position = MVP * vec4(position, 1.0); 
            Texcoord = texcoord;
        }
    )glsl";

    const char* fragmentSource = R"glsl( 
        #version 410

        out vec4 outColor;
        in vec2 Texcoord;

        uniform sampler2D tex;

        void main() {
            outColor = vec4(1.0, 1.0, 1.0, 1.0);
        }
    )glsl";
    //texture(tex, Texcoord); 

    enum { POSITION_ATTRIB_LOC, TEXCOORD_ATTRIB_LOC };

    glShaderSource(vertexShader, 1, &vertexSource, nullptr);
    glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);

    glCompileShader(vertexShader);
    glCompileShader(fragmentShader);

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    glBindAttribLocation(shaderProgram, POSITION_ATTRIB_LOC, "position");
    glBindAttribLocation(shaderProgram, TEXCOORD_ATTRIB_LOC, "texcoord");

    glLinkProgram(shaderProgram);

    GLint info;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &info);
    if (info == GL_FALSE) {
        char infolog[1024];
        glGetShaderInfoLog(vertexShader, 1024, NULL, infolog);
        std::cout << "The vertex shader failed to compile with the error: " << infolog << std::endl;
        LOG::Print("Failed to initialize glfw", infolog, ERROR);
    }

    info = 0;
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &info);
    if (info == GL_FALSE) {
        char infolog[1024];
        glGetShaderInfoLog(fragmentShader, 1024, NULL, infolog);
        std::cout << "The fragment shader failed to compile with the error: " << infolog << std::endl;
    }

    glValidateProgram(shaderProgram);
    glUseProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    GLuint MatrixID = glGetUniformLocation(shaderProgram, "MVP");

    glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 1000.0f);

    Camera camera;

    glm::mat4 Transform = glm::mat4(1.0f);
    //Model = glm::rotate(Model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    Mesh terrain;
    std::vector<Vertex> vertices;
    std::vector<int> indices; 

    for (int x = 0; x < 16; x++) {
        for (int z = 0; z < 16; z++) {
            int offset = (x * 16) + z;
            vertices.push_back(Vertex(x * 1, 0, z * 1));
        }
    }

    for (int x = 0; x < 16 - 1; x++) {
        for (int z = 0; z < 16 - 1; z++) {
            int a = (x       * (16)) + z;
            int b = ((x + 1) * (16)) + z;
            int c = ((x + 1) * (16)) + (z + 1);
            int d = (x       * (16)) + (z + 1);

            indices.push_back(c);
            indices.push_back(b);
            indices.push_back(a);

            indices.push_back(a);
            indices.push_back(d);
            indices.push_back(c);
        }
    }

    terrain.setVertices(vertices);
    terrain.setIndices(indices);

    Texture sample("sample.png");
    
    glClearColor(0.133, 0.133, 0.133, 1.0);

    while(!glfwWindowShouldClose(window)) {
        static double lastTime = glfwGetTime();
        double currentTime = glfwGetTime();
        GLfloat deltaTime = float(currentTime - lastTime);
     
        /* Poll for and process events */
        glfwPollEvents();

        /* Hier Rendern */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
        glUseProgram(shaderProgram);

        camera.update(window, deltaTime);

        glm::mat4 MVP = Projection * camera.getView() * Transform;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, glm::value_ptr(MVP));

        terrain.draw();

        /* Tausche den front und back Buffer */
        glfwSwapBuffers(window);
        lastTime = currentTime;
    }

    glfwTerminate();
    return 0;
}