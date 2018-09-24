// Draw Triangles in a window using shaders, Vertex Buffer(VBO) and Atrribute (VAO) Objects
// Use: Shaders, Vertex Buffer(VBO), Atrribute (VAO) Objects

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
int DrawTwoTrianglesSingleBufferSameColor();
int DrawTwoTrianglesMultipleBuffersDifferentColors();
int DrawTwoTrianglesSingleBufferDifferentColor();

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// settings
const GLuint SCR_WIDTH = 800;
const GLuint SCR_HEIGHT = 600;

// SHADERS
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "out vec4 VertexColor;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "   VertexColor = vec4(1.0f, 0.0f, 1.0f, 1.0);\n"
    "}\0";
const char *fragmentShaderSource1 = "#version 330 core\n"
    "in vec4 VertexColor;\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = VertexColor;\n"
//    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";
const char *fragmentShaderSource2 = "#version 330 core\n"
    "out vec4 FragColor;\n"
    " uniform vec4 ourColor;\n"
    "void main()\n"
    "{\n"
      "   FragColor = ourColor;\n"
//     "   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
    "}\n\0";


const char *vertexShaderDifferentColorSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n"
    "out vec4 VertexColor;\n"
    "out vec4 VertexPos;\n"
    " uniform vec4 offset;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(-aPos, 1.0) + offset;\n" // upside down + offset
    "   VertexPos = gl_Position;\n"
//    "   gl_Position = vec4(aPos, 1.0);\n"
    "   VertexColor = vec4(aColor, 1.0);\n"
//    "   VertexColor = vec4(aColor, 1.0);\n"
    "}\0";
const char *fragmentShaderDifferentColorSource = "#version 330 core\n"
    "in vec4 VertexColor;\n"
    "in vec4 VertexPos;\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = VertexPos;\n"
//    "   FragColor = VertexColor;\n"
    "}\n\0";


//  ----------------------------------------MAIN METHOD--------------------------------------------

int main()
{
//    return DrawTwoTrianglesSingleBufferSameColor();
    return DrawTwoTrianglesMultipleBuffersDifferentColors();
//    return DrawTwoTrianglesSingleBufferDifferentColor();
}

//  -----------------------------------------------------------------------------------------------

int DrawTwoTrianglesSingleBufferSameColor()
{
    // Drawing triangles of the same color with 1 VAO and VBO

    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);
    // check for shader compile errors
    GLint success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource1, nullptr);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link vertexShader and fragmentShader into shaderProgram
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // set up vertex data
    const GLfloat vertices[] = {
        // 1st triangle
        -0.5f,  0.5f, 0.0f,  // top
        -0.9f, -0.5f, 0.0f, // left
        -0.1f, -0.5f, 0.0f, // right
        // 2nd triangle
         0.5f,  0.5f, 0.0f,  // top
         0.1f, -0.5f, 0.0f, // left
         0.9f, -0.5f, 0.0f, // right
    };

    GLuint VAO;
    glGenVertexArrays(1, &VAO); // generate a VAO

    GLuint VBO;
    glGenBuffers(1, &VBO); // generate a VBO

    glBindVertexArray(VAO); // 1. bind the VAO
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // 2.1 bind the VBO with information about its type
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // 2.2 set VBO data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)nullptr); // 3.1 put VBO into VAO i.e. into one of the attribute lists (attribute list # 0) of the VAO
    glEnableVertexAttribArray(0); // 3.2 enable the vertex attribute array to which VBO is registered i.e. 0
    glBindBuffer(GL_ARRAY_BUFFER, 0); // 4. unbind VBO after registering it to VAO via glVertexAttribPointer
    glBindVertexArray(0); // 5. unbind VAO

    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);

        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw triangle
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6); // Drawing two triangles side by side
        glBindVertexArray(0);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources
    glfwTerminate();
    return 0;
}




int DrawTwoTrianglesMultipleBuffersDifferentColors()
{
    // Drawing triangles of different colors with multiple VAOs and VBOs

    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);
    // check for shader compile errors
    GLint success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // fragment shader 1
    GLuint fragmentShader1 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader1, 1, &fragmentShaderSource1, nullptr);
    glCompileShader(fragmentShader1);
    // check for shader compile errors
    glGetShaderiv(fragmentShader1, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader1, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // link vertexShader and fragmentShader1 into shaderProgram1
    GLuint shaderProgram1 = glCreateProgram();
    glAttachShader(shaderProgram1, vertexShader);
    glAttachShader(shaderProgram1, fragmentShader1);
    glLinkProgram(shaderProgram1);
    // check for linking errors
    glGetProgramiv(shaderProgram1, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram1, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // fragment shader 2
    GLuint fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader2, 1, &fragmentShaderSource2, nullptr);
    glCompileShader(fragmentShader2);
    // check for shader compile errors
    glGetShaderiv(fragmentShader2, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader2, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // link vertexShader and fragmentShader2 into shaderProgram2
    GLuint shaderProgram2 = glCreateProgram();
    glAttachShader(shaderProgram2, vertexShader);
    glAttachShader(shaderProgram2, fragmentShader2);
    glLinkProgram(shaderProgram2);
    // check for linking errors
    glGetProgramiv(shaderProgram2, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram2, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader1);
    glDeleteShader(fragmentShader2);

    // set up vertex data
    const GLfloat firstTriangle[] = {
        -0.5f, 0.5f, 0.0f,  // top
        -0.9f, -0.5f, 0.0f,  // left
        -0.1f, -0.5f, 0.0f,  // right
    };
    const GLfloat secondTriangle[] = {
        0.5f, 0.5f, 0.0f,   // top
        0.1f, -0.5f, 0.0f,  // left
        0.9f, -0.5f, 0.0f,  // right
    };

    GLuint VAOs[2];
    glGenVertexArrays(2, VAOs); // generate multiple VAOs at the same time

    GLuint VBOs[2];
    glGenBuffers(2, VBOs); // generate multiple VBOs at the same time


    // first triangle setup
    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)nullptr);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // second triangle setup
    glBindVertexArray(VAOs[1]);	// note that we bind to a different VAO now
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);	// and a different VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle), secondTriangle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)nullptr);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);

        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw first triangle using the data from the first VAO
        glUseProgram(shaderProgram1);
        glBindVertexArray(VAOs[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

         // draw second triangle using the data from the first VAO
        glUseProgram(shaderProgram2);
        // set uniform color for the second fragment shader linked inside shaderProgram2
        const GLfloat timeValue = static_cast<float>(glfwGetTime()); // update the uniform color
        const GLfloat greenValue = std::sin(timeValue) / 2.0f + 0.5f;
        const GLint vertexColorLocation = glGetUniformLocation(shaderProgram2, "ourColor");
        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
        glBindVertexArray(VAOs[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(2, VAOs);
    glDeleteBuffers(2, VBOs);

    // glfw: terminate, clearing all previously allocated GLFW resources
    glfwTerminate();
    return 0;
}


int DrawTwoTrianglesSingleBufferDifferentColor()
{
    // Drawing triangles of the same color with 1 VAO and VBO

    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // vertex shader
    GLuint vertexShaderDifferentColor = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShaderDifferentColor, 1, &vertexShaderDifferentColorSource, nullptr);
    glCompileShader(vertexShaderDifferentColor);
    // check for shader compile errors
    GLint success;
    char infoLog[512];
    glGetShaderiv(vertexShaderDifferentColor, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShaderDifferentColor, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    GLuint fragmentShaderDifferentColor = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderDifferentColor, 1, &fragmentShaderDifferentColorSource, nullptr);
    glCompileShader(fragmentShaderDifferentColor);
    // check for shader compile errors
    glGetShaderiv(fragmentShaderDifferentColor, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShaderDifferentColor, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // link vertexShader and fragmentShader into shaderProgram
    GLuint shaderProgramDifferentColor = glCreateProgram();
    glAttachShader(shaderProgramDifferentColor, vertexShaderDifferentColor);
    glAttachShader(shaderProgramDifferentColor, fragmentShaderDifferentColor);
    glLinkProgram(shaderProgramDifferentColor);
    // check for linking errors
    glGetProgramiv(shaderProgramDifferentColor, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgramDifferentColor, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShaderDifferentColor);
    glDeleteShader(fragmentShaderDifferentColor);

    // set up vertex data
    const GLfloat vertices[] = {
        // 1st triangle positions and colors
        -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f,  // top
        -0.9f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // left
        -0.1f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // right
        // 2nd triangle positions and colors
         0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // top
         0.1f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // left
         0.9f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  // right
    };

    GLuint VAO;
    glGenVertexArrays(1, &VAO); // generate a VAO

    GLuint VBO;
    glGenBuffers(1, &VBO); // generate a VBO

    glBindVertexArray(VAO); // 1. bind the VAO
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // 2.1 bind the VBO with information about its type
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // 2.2 set VBO data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)nullptr); // 3.1 put VBO POSITION data into VAO i.e. into one of the attribute lists (attribute list # 0) of the VAO
    glEnableVertexAttribArray(0); // 3.2 enable the vertex attribute array to which VBO POSITION data is registered i.e. 0
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float))); // 3.3 put VBO COLOR data into VAO i.e. into one of the attribute lists (attribute list # 1) of the VAO
    glEnableVertexAttribArray(1); // 3.4 enable the vertex attribute array to which VBO COLOR data is registered i.e. 1
    glBindBuffer(GL_ARRAY_BUFFER, 0); // 4. unbind VBO after registering it to VAO via glVertexAttribPointer
    glBindVertexArray(0); // 5. unbind VAO

    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);

        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw triangle
        glUseProgram(shaderProgramDifferentColor);
        // set uniform offset for the fragment shader linked inside shaderProgramDifferentColor
        const GLfloat  x_offset = 0.15f;
        const GLint offsetLocation = glGetUniformLocation(shaderProgramDifferentColor, "offset");
        glUniform4f(offsetLocation,x_offset, 0.0f, 0.0f, 0.0f);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6); // Drawing two triangles side by side
        glBindVertexArray(0);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources
    glfwTerminate();
    return 0;
}
