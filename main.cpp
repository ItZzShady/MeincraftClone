#include <stdio.h>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

int main() {

    GLFWwindow* window;

    if (!glfwInit()) {
        printf("Failed to initialize glfw.");
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    /* For OSX */
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    window = glfwCreateWindow(640, 480, "Minecraft Clone", nullptr, nullptr);
    if(!window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
   if (glewInit() != GLEW_OK) {

   };

    float vertices[] = {
         0.0f,  0.5f, // Vertex 1 (X, Y) 
         0.5f, -0.5f, // Vertex 2 (X, Y)
        -0.5f, -0.5f  // Vertex 3 (X, Y)
    };
    
    unsigned int shaderProgram = glCreateProgram();
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    const char* vertexSource = R"glsl( 
        #version 410

        in vec2 position;
        in vec4 color;
        out vec4 color0;

        void main() {
            gl_Position = vec4(position, 0.0, 1.0); 
            color0 = color;
        }
    )glsl";

    const char* fragmentSource = R"glsl( 
        #version 410

        out vec4 fragmentColor;
        in vec4 color0;

        void main() {
            fragmentColor = color0; 
        }
    )glsl";

    enum {POSITION_ATTRIB_LOC, COLOR_ATTRIB_LOC };

    glShaderSource(vertexShader, 1, &vertexSource, nullptr);
    glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);

    glCompileShader(vertexShader);
    glCompileShader(fragmentShader);

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    glBindAttribLocation(shaderProgram, POSITION_ATTRIB_LOC, "position");
    glBindAttribLocation(shaderProgram, COLOR_ATTRIB_LOC, "color");

    glLinkProgram(shaderProgram);
    glValidateProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // now on to buffers
    enum {POSITION, COLOR, NUM_BUFFERS };

    unsigned int vao, vbo[NUM_BUFFERS];

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(NUM_BUFFERS, vbo);

    const float positions[] = {
        -0.5, -0.5,
        0.0, 0.5,
        0.5, -0.5
    };

    const float colors[] = {
        1, 0, 0, 1,
        0, 1, 0, 1,
        0, 0, 1, 1
    };

    glBindBuffer(GL_ARRAY_BUFFER, vbo[POSITION]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
    glEnableVertexAttribArray(POSITION_ATTRIB_LOC);
    glVertexAttribPointer(POSITION_ATTRIB_LOC, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[COLOR]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
    glEnableVertexAttribArray(COLOR_ATTRIB_LOC);
    glVertexAttribPointer(COLOR_ATTRIB_LOC, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0);
    

    glClearColor(0.1, 0.1, 0.1, 1.0);
    /* Loop unitl the user closes the window */
    while(!glfwWindowShouldClose(window)) {

        /* Poll for and process events */
        glfwPollEvents();

        /* Hier Rendern */
        glClear(GL_COLOR_BUFFER_BIT);
        
        glUseProgram(shaderProgram);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Tausche den front und back Buffer */
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}