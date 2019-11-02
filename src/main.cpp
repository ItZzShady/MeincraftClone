#include <iostream>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Log.h"
#include "Texture.h"
#include "Camera.h"
#include "ModelLoader.h"

int main() {

    GLFWwindow* window;

    LOG::Print("Failed to initialize glfw", ERROR);

    if (!glfwInit()) {
        LOG::Print("Failed to initialize glfw", ERROR);
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    window = glfwCreateWindow(1024, 768, "Minecraft Clone", nullptr, nullptr);
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
            outColor = texture(tex, Texcoord); 
        }
    )glsl";

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

    glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);

    Camera camera;

    glm::mat4 Transform = glm::mat4(1.0f);
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

    /*GLuint elements[] = {
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
    };*/

    float pixels[] = {
        0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f
    };

    std::vector<glm::vec3> indices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;

    Model model;
    model.load("cube.obj", indices, uvs, normals);

    for (int i = 0; i < indices.size(); i++) {
        //std::cout << "X: " << indices[i].x << " Y: " << indices[i].y << " Z: " << indices[i].z << std::endl;
    }

    for (int i = 0; i < uvs.size(); i++) {
        std::cout << i << ". UV - X: " << uvs[i].x << " Y: " << uvs[i].y << std::endl;
    }

    // now on to buffers
    enum {POSITION, UV, NUM_BUFFERS };

    GLuint vao, vbo[NUM_BUFFERS], ebo;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(NUM_BUFFERS, vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[POSITION]);
    glBufferData(GL_ARRAY_BUFFER, indices.size() * sizeof(glm::vec3), &indices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[UV]);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

    /*glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements.size() * sizeof(int), &elements[0], GL_STATIC_DRAW);*/

    Texture sample("sample.png");

    glEnableVertexAttribArray(POSITION_ATTRIB_LOC);
    glVertexAttribPointer(POSITION_ATTRIB_LOC, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    /*glEnableVertexAttribArray(COLOR_ATTRIB_LOC);
    glVertexAttribPointer(COLOR_ATTRIB_LOC, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 3));*/

    glEnableVertexAttribArray(TEXCOORD_ATTRIB_LOC);
    glVertexAttribPointer(TEXCOORD_ATTRIB_LOC, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    
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

        glm::mat4 MVP = Projection * camera.getView() * Transform;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, glm::value_ptr(MVP));

        glBindVertexArray(vao);
        // 1rst attribute buffer : vertices
		glEnableVertexAttribArray(POSITION);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[POSITION]);
		glVertexAttribPointer(
			0,                  // attribute
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : UVs
		glEnableVertexAttribArray(UV);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[UV]);
		glVertexAttribPointer(
			1,                                // attribute
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

        //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glDrawArrays(GL_TRIANGLES, 0, indices.size());

        glDisableVertexAttribArray(POSITION);
		glDisableVertexAttribArray(UV);

        /* Tausche den front und back Buffer */
        glfwSwapBuffers(window);
        lastTime = currentTime;
    }

    glfwTerminate();
    return 0;
}