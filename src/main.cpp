#include <stdio.h>
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
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    window = glfwCreateWindow(1024, 768, "Minecraft Clone", nullptr, nullptr);
    if(!window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        glfwTerminate();
        return -1;
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
    glUseProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // now on to buffers
    enum {POSITION, COLOR, NUM_BUFFERS };

    unsigned int vao, vbo[NUM_BUFFERS];

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(NUM_BUFFERS, vbo);

    const float vertices[] = {
        -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // Top-left
         0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // Top-right
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // Bottom-right
        -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, // Bottom-left
    };

    glBindBuffer(GL_ARRAY_BUFFER, vbo[POSITION]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLuint ebo;
    glGenBuffers(1, &ebo);

    GLuint elements[] = {
        0, 1, 2,
        2, 3, 0
    };

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

    glEnableVertexAttribArray(POSITION_ATTRIB_LOC);
    glVertexAttribPointer(POSITION_ATTRIB_LOC, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)0);

    glEnableVertexAttribArray(COLOR_ATTRIB_LOC);
    glVertexAttribPointer(COLOR_ATTRIB_LOC, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(sizeof(GLfloat) * 2));
    
    glClearColor(0.1, 0.1, 0.1, 1.0);
    /* Loop unitl the user closes the window */
    while(!glfwWindowShouldClose(window)) {

        /* Poll for and process events */
        glfwPollEvents();

        /* Hier Rendern */
        glClear(GL_COLOR_BUFFER_BIT);
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        /* Tausche den front und back Buffer */
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}