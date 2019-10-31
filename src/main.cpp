#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

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
    
    unsigned int shaderProgram  = glCreateProgram();
    unsigned int vertexShader   = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    const char* vertexSource = R"glsl( 
        #version 410

        in vec2 position;
        in vec4 color;
        in vec2 texcoord;

        out vec4 color0;
        out vec2 Texcoord;

        void main() {
            gl_Position = vec4(position, 0.0, 1.0); 
            color0 = color;
            Texcoord = texcoord;
        }
    )glsl";

    const char* fragmentSource = R"glsl( 
        #version 410

        out vec4 outColor;
        in vec4 color0;
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

    const float vertices[] = {
    //  Position      Color                   TexCoords
        -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, // Top-left
         0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, // Top-right
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, // Bottom-right
        -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f  // Bottom-left
    };

    GLuint elements[] = {
        0, 1, 2,
        2, 3, 0
    };

    float pixels[] = {
        0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f
    };

    // now on to buffers
    enum {POSITION, COLOR, NUM_BUFFERS };

    GLuint vao, vbo[NUM_BUFFERS], ebo, texture;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(NUM_BUFFERS, vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[POSITION]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* image = stbi_load("sample.png", &width, &height, &channels, STBI_rgb);
    std::cout <<
    " width: " << width <<
    " height: " << height << std::endl;
    if (image == 0) {
        std::cout << "Cannot load image." << std::endl;
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_FLOAT, pixels);
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    stbi_image_free(image);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glEnableVertexAttribArray(POSITION_ATTRIB_LOC);
    glVertexAttribPointer(POSITION_ATTRIB_LOC, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0);

    glEnableVertexAttribArray(COLOR_ATTRIB_LOC);
    glVertexAttribPointer(COLOR_ATTRIB_LOC, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 2));

    glEnableVertexAttribArray(TEXCOORD_ATTRIB_LOC);
    glVertexAttribPointer(TEXCOORD_ATTRIB_LOC, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 6));
    
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