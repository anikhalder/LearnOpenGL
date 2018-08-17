// NOT YET IMPLEMENTED
// Draw Rectangle with texture
// Use: Shader class and files, VBO, VAO, EBO and textures

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <shader.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
int DrawRectangleTexture();

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

//  ----------------------------------------MAIN METHOD--------------------------------------------

int main()
{
    return DrawRectangleTexture();
}

//  -----------------------------------------------------------------------------------------------


int DrawRectangleTexture()
{
    // Drawing a rectangle with VAO, VBO, EBO along with texture

    // set relative path of project from the location of the executable file
    std::string relPathExePro("../../LearnOpenGL/hello_texture/");

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

    // build and compile the shader program
    Shader ShalderProgram((relPathExePro+"hello_texture.vert").c_str(), (relPathExePro+"hello_texture.frag").c_str());

    // set up vertex data
    const GLfloat vertices[] = {
        -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f,   0.0f, 1.0f, // top left
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
         0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,   1.0f, 1.0f  // top right
    };
    const GLuint indices[] = {  // note that we start from 0!
        0, 1, 3,  // first Triangle
        3, 1, 2   // second Triangle
    };

    GLuint VAO;
    glGenVertexArrays(1, &VAO); // generate a VAO

    GLuint VBO;
    glGenBuffers(1, &VBO); // generate a VBO

    glBindVertexArray(VAO); // 1. bind the VAO
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // 2.1 bind the VBO with information about its type
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // 2.2 set VBO data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)nullptr); // 3.1 put VBO POSITION data into VAO i.e. into one of the attribute lists (attribute list # 0) of the VAO
    glEnableVertexAttribArray(0); // 3.2 enable the vertex attribute array to which VBO is registered i.e. 0
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float))); // 3.3 put VBO COLOR data into VAO i.e. into one of the attribute lists (attribute list # 1) of the VAO
    glEnableVertexAttribArray(1); // 3.4 enable the vertex attribute array to which VBO is registered i.e. 1
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6*sizeof(float))); // 3.5 put VBO TEXTURE COORD data into VAO i.e. into one of the attribute lists (attribute list # 2) of the VAO
    glEnableVertexAttribArray(2); // 3.6 enable the vertex attribute array to which VBO is registered i.e. 2
    glBindBuffer(GL_ARRAY_BUFFER, 0); // 4. unbind VBO
    glBindVertexArray(0); // 5. unbind VAO

    GLuint EBO;
    glGenBuffers(1, &EBO); // generate an EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // bind the EBO with information about its type
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); // set EBO data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // unbind EBO

    // load and create a texture
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    unsigned char *data = stbi_load((relPathExePro+std::string("../common/resources/textures/container.jpg")).c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

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

        // bind Texture
        glBindTexture(GL_TEXTURE_2D, texture);

        // render container
        ShalderProgram.use();
        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // bind the EBO while a VAO is active as it is required to associate the correct indices of the vertices in the VBO stored in the VAO
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr); // glDrawElements makes use of the indices in EBO and associates them with the vertices
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // de-allocate all resources once they've outlived their purpose:
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}
