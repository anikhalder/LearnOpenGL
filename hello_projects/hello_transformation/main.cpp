// Draw Rectangle with multiple textures and apply tranformations
// Use: Shader class and files, VBO, VAO, EBO, textures and glm library for transformations

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <shader.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace
{
    std::string project_folder = "hello_projects";
    std::string project_name = "hello_transformation";

    // initial mix value for how much we're seeing of either texture
    float mixValueFromKey = 0.2f;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
int DrawTransformedRectangleMultipleTextures();

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        mixValueFromKey += 0.005f;
        if(mixValueFromKey >= 1.0f)
            mixValueFromKey = 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        mixValueFromKey -= 0.005f;
        if (mixValueFromKey <= 0.0f)
            mixValueFromKey = 0.0f;
    }
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
    return DrawTransformedRectangleMultipleTextures();
}

//  -----------------------------------------------------------------------------------------------

int DrawTransformedRectangleMultipleTextures()
{
    // Drawing transformed rectangle (scaled, rotated, translated) using VAO, VBO, EBO along with multiple textures

    // set relative path of project from the location of the executable file
    std::string relPathExePro("../../LearnOpenGL/"+project_folder+"/"+project_name+"/");

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
    Shader ShaderProgram((relPathExePro+project_name+".vert").c_str(), (relPathExePro+project_name+".frag").c_str());

    // set up vertex data (vertex ordering from top right and go counter clockwise)
    const GLfloat vertices[] = {
         0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,   1.0f, 1.0f,  // top right - 0
        -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f,   0.0f, 1.0f, // top left - 1
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left - 2
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right - 3
    };
    const GLuint indices[] = {  // note that we start from 0!
        0, 1, 2,  // first Triangle
        2, 3, 0   // second Triangle
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

    // load and create multiple textures
    GLuint textures[2];
    glGenTextures(2, textures);

    // first texture setup
    glBindTexture(GL_TEXTURE_2D, textures[0]); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis
    unsigned char *data = stbi_load((relPathExePro+std::string("../../common/resources/textures/container.jpg")).c_str(), &width, &height, &nrChannels, 0);
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
    glBindTexture(GL_TEXTURE_2D, 0); // unbind texture


    // second texture setup
    glBindTexture(GL_TEXTURE_2D, textures[1]); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    data = stbi_load((relPathExePro+std::string("../../common/resources/textures/awesomeface.png")).c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0); // unbind texture


    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // tell opengl for each sampler (uniforms) to which texture unit it belongs to
    ShaderProgram.use(); // activate the shader before setting uniforms
    glUniform1i(glGetUniformLocation(ShaderProgram.ID, "texture1"), 0); // set it manually
    ShaderProgram.setInt("texture2", 1); // or with shader class uniform utility function

    // scale and rotate by 90 degrees
//    glm::mat4 scaleRot = glm::mat4(1.0f);
//    scaleRot = glm::rotate(scaleRot, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
//    scaleRot = glm::scale(scaleRot, glm::vec3(0.5, 0.5, 0.5));
//    ShaderProgram.setMat4("transform", scaleRot);

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);

        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // activate texture units and bind them
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textures[0]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textures[1]);

        // render container
        ShaderProgram.use();
        // set the texture mix value in the shader
        ShaderProgram.setFloat("mixValue", mixValueFromKey);

        // draw first container
        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // bind the EBO while a VAO is active as it is required to associate the correct indices of the vertices in the VBO stored in the VAO
        // scale, rotate (with time) and translate
        glm::mat4 scaleRotTrans = glm::mat4(1.0f);
        scaleRotTrans = glm::translate(scaleRotTrans, glm::vec3(0.5f, -0.5f, 0.0f));
        scaleRotTrans = glm::rotate(scaleRotTrans, (float)glfwGetTime(), glm::vec3(0.0, 0.0, 1.0));
        scaleRotTrans = glm::scale(scaleRotTrans, glm::vec3(0.5, 0.5, 0.5));
        ShaderProgram.setMat4("transform", scaleRotTrans);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr); // glDrawElements makes use of the indices in EBO and associates them with the vertices
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        // draw second container (ONLY BY USING translate and glDrawELements - no need for extra buffer objects or shaders)
        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // bind the EBO while a VAO is active as it is required to associate the correct indices of the vertices in the VBO stored in the VAO
        // scale (with time) and translate
        glm::mat4 scaleTrans = glm::mat4(1.0f);
        scaleTrans = glm::translate(scaleTrans, glm::vec3(-0.5f, 0.5f, 0.0f));
        scaleTrans = glm::scale(scaleTrans, glm::vec3(sin(glfwGetTime()), sin(glfwGetTime()), sin(glfwGetTime())));
        ShaderProgram.setMat4("transform", scaleTrans);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr); // glDrawElements makes use of the indices in EBO and associates them with the vertices
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

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
