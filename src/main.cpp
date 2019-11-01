#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Texture.h"
#include "Camera.h"

int main() {

    GLFWwindow* window;

    if (!glfwInit()) {
        printf("Failed to initialize glfw.");
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    window = glfwCreateWindow(1024, 768, "Minecraft Clone", nullptr, nullptr);
    if(!window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        glfwTerminate();
        return -1;
    };

    glEnable(GL_DEPTH_TEST);
    
    unsigned int shaderProgram  = glCreateProgram();
    unsigned int vertexShader   = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    const char* vertexSource = R"glsl( 
        #version 410

        in vec3 position;
        in vec3 color;
        in vec2 texcoord;

        out vec3 color0;
        out vec2 Texcoord;

        uniform mat4 MVP;

        void main() {
            gl_Position = MVP * vec4(position, 1.0); 
            color0 = color;
            Texcoord = texcoord;
        }
    )glsl";

    const char* fragmentSource = R"glsl( 
        #version 410

        out vec4 outColor;
        in vec3 color0;
        in vec2 Texcoord;

        uniform sampler2D tex;

        void main() {
            outColor = texture(tex, Texcoord); 
        }
    )glsl";

    enum { POSITION_ATTRIB_LOC, COLOR_ATTRIB_LOC, TEXCOORD_ATTRIB_LOC };

    glShaderSource(vertexShader, 1, &vertexSource, nullptr);
    glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);

    glCompileShader(vertexShader);
    glCompileShader(fragmentShader);

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    glBindAttribLocation(shaderProgram, POSITION_ATTRIB_LOC, "position");
    glBindAttribLocation(shaderProgram, COLOR_ATTRIB_LOC, "color");
    glBindAttribLocation(shaderProgram, TEXCOORD_ATTRIB_LOC, "texcoord");

    glLinkProgram(shaderProgram);

    GLint info;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &info);
    if (info == GL_FALSE) {
        char infolog[1024];
        glGetShaderInfoLog(vertexShader, 1024, NULL, infolog);
        std::cout << "The vertex shader failed to compile with the error: " << infolog << std::endl;
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

    glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);

    Camera camera;

    glm::mat4 Model = glm::mat4(1.0f);
    //Model = glm::rotate(Model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));


    GLfloat vertices[] = {
        -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

         0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
         0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
         0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
         0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
         0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f
    };

    GLuint elements[] = {
        0, 1, 2,
        2, 3, 0,
        4, 5, 6,
        6, 7, 4,
        8, 9, 10,
        10, 11, 8,
        12, 13, 14,
        14, 15, 12,
        16, 17, 18,
        18, 19, 16,
        20, 21, 22,
        22, 23, 20,
    };

    float pixels[] = {
        0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f
    };

    // now on to buffers
    enum {POSITION, COLOR, NUM_BUFFERS };

    GLuint vao, vbo[NUM_BUFFERS], ebo;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(NUM_BUFFERS, vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[POSITION]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    /*glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);*/

    Texture sample("sample.png");

    glEnableVertexAttribArray(POSITION_ATTRIB_LOC);
    glVertexAttribPointer(POSITION_ATTRIB_LOC, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0);

    glEnableVertexAttribArray(COLOR_ATTRIB_LOC);
    glVertexAttribPointer(COLOR_ATTRIB_LOC, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 3));

    glEnableVertexAttribArray(TEXCOORD_ATTRIB_LOC);
    glVertexAttribPointer(TEXCOORD_ATTRIB_LOC, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 6));
    
    glClearColor(0.133, 0.133, 0.133, 1.0);
    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

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

        glm::mat4 MVP = Projection * camera.getView() * Model;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, glm::value_ptr(MVP));

        //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        /* Tausche den front und back Buffer */
        glfwSwapBuffers(window);
        lastTime = currentTime;
    }

    glfwTerminate();
    return 0;
}