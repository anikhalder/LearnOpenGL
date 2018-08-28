// Draw Rectangular object/cube with multiple textures and use different coordinate systems matrices with camera implementation
// Use: Shader class and files, VBO, VAO, EBO, textures, glm library for transformations,
//      coordinate system matrices and camera implementation

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
    std::string project_name = "hello_camera";

    // initial mix value for how much we're seeing of either texture
    float mixValueFromKey = 0.2f;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
int DrawTexturedRectangleWithCoordinateSystem();
int DrawWireFrameCubeWithCoordinateSystem();
int DrawTexturedCubeWithCoordinateSystem();
int DrawMultipleTexturedCubesWithCoordinateSystem();

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
//    return DrawTexturedRectangleWithCoordinateSystem();
//    return DrawWireFrameCubeWithCoordinateSystem();
//    return DrawTexturedCubeWithCoordinateSystem();
    return DrawMultipleTexturedCubesWithCoordinateSystem();
}

//  -----------------------------------------------------------------------------------------------

int DrawTexturedRectangleWithCoordinateSystem()
{
    // Drawing rectangle using VAO, VBO, EBO, with multiple textures and coordinate system matrices

    // set relative path of project from the location of the executable file
    std::string relPathExePro("../../LearnOpenGL/"+project_name+"/");

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
         0.5f,  0.5f, 0.0f, 1.0f, 1.0f,  // top right - 0
        -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, // top left - 1
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom left - 2
         0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // bottom right - 3
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)nullptr); // 3.1 put VBO POSITION data into VAO i.e. into one of the attribute lists (attribute list # 0) of the VAO
    glEnableVertexAttribArray(0); // 3.2 enable the vertex attribute array to which VBO is registered i.e. 0
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float))); // 3.3 put VBO TEXTURE COORD data into VAO i.e. into one of the attribute lists (attribute list # 1) of the VAO
    glEnableVertexAttribArray(1); // 3.4 enable the vertex attribute array to which VBO is registered i.e. 1
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
    data = stbi_load((relPathExePro+std::string("../common/resources/textures/awesomeface.png")).c_str(), &width, &height, &nrChannels, 0);
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

        // draw container and use different transforms (matrices) for using different coordinate systems
        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // bind the EBO while a VAO is active as it is required to associate the correct indices of the vertices in the VBO stored in the VAO

        // create transformations and send them to the shader program by setting the uniforms

        // model matrix : local space to world (model) space
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        ShaderProgram.setMat4("model", model);

        // view matrix : world (model) space to view (camera) space
        glm::mat4 view = glm::mat4(1.0f);
        view  = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        ShaderProgram.setMat4("view", view);

        // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
        // projection matrix: view (camera) space to clip space
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(45.0f), static_cast<float>(SCR_WIDTH/SCR_HEIGHT), 0.1f, 100.0f);
        ShaderProgram.setMat4("projection", projection);

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


int DrawWireFrameCubeWithCoordinateSystem()
{
    // Drawing wire-frame cube using VAO, VBO, EBO (loading textures but is actually useless in this case)

    // set relative path of project from the location of the executable file
    std::string relPathExePro("../../LearnOpenGL/"+project_name+"/");

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

    // configure global opengl state
    glEnable(GL_DEPTH_TEST);

    // build and compile the shader program
    Shader ShaderProgram((relPathExePro+project_name+".vert").c_str(), (relPathExePro+project_name+".frag").c_str());

    // set up vertex data (vertex ordering from top right and go counter clockwise)

    // All the texture coordinates do not matter (they are actually incorrect as each vertex on every face has different texture coordinates) as we draw the cube in wire-frame mode
    const GLfloat vertices[] = {
        // front face
         0.5f,  0.5f, 0.5f, 1.0f, 1.0f,  // top right - 0
        -0.5f,  0.5f, 0.5f, 0.0f, 1.0f, // top left - 1
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // bottom left - 2
         0.5f, -0.5f, 0.5f, 1.0f, 0.0f, // bottom right - 3
        // back face
        0.5f,  0.5f, -0.5f, 1.0f, 1.0f,  // top right - 4
       -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, // top left - 5
       -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // bottom left - 6
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, // bottom right - 7
    };
    const GLuint indices[] = {  // note that we start from 0!
        0, 1, 2, 2, 3, 0, // front face
        4, 0, 3, 3, 7, 4, // right face
        4, 5, 6, 6, 7, 4, // back face
        5, 1, 2, 2, 6, 5, // left face
        4, 5, 1, 1, 0, 4, // top face
        7, 6, 2, 2, 3, 7, // bottom face
    };

    GLuint VAO;
    glGenVertexArrays(1, &VAO); // generate a VAO

    GLuint VBO;
    glGenBuffers(1, &VBO); // generate a VBO

    glBindVertexArray(VAO); // 1. bind the VAO
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // 2.1 bind the VBO with information about its type
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // 2.2 set VBO data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)nullptr); // 3.1 put VBO POSITION data into VAO i.e. into one of the attribute lists (attribute list # 0) of the VAO
    glEnableVertexAttribArray(0); // 3.2 enable the vertex attribute array to which VBO is registered i.e. 0
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float))); // 3.3 put VBO TEXTURE COORD data into VAO i.e. into one of the attribute lists (attribute list # 1) of the VAO
    glEnableVertexAttribArray(1); // 3.4 enable the vertex attribute array to which VBO is registered i.e. 1
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
    data = stbi_load((relPathExePro+std::string("../common/resources/textures/awesomeface.png")).c_str(), &width, &height, &nrChannels, 0);
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
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // tell opengl for each sampler (uniforms) to which texture unit it belongs to
    ShaderProgram.use(); // activate the shader before setting uniforms
    glUniform1i(glGetUniformLocation(ShaderProgram.ID, "texture1"), 0); // set it manually
    ShaderProgram.setInt("texture2", 1); // or with shader class uniform utility function

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);

        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

        // activate texture units and bind them
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textures[0]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textures[1]);

        // render container
        ShaderProgram.use();
        // set the texture mix value in the shader
        ShaderProgram.setFloat("mixValue", mixValueFromKey);

        // draw container and use different transforms (matrices) for using different coordinate systems
        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // bind the EBO while a VAO is active as it is required to associate the correct indices of the vertices in the VBO stored in the VAO

        // create transformations and send them to the shader program by setting the uniforms

        // model matrix : local space to world (model) space
        glm::mat4 model = glm::mat4(1.0f);
//        model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, static_cast<float>(glfwGetTime()) * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
        ShaderProgram.setMat4("model", model);

        // view matrix : world (model) space to view (camera) space
        glm::mat4 view = glm::mat4(1.0f);
        view  = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        ShaderProgram.setMat4("view", view);

        // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
        // projection matrix: view (camera) space to clip space
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(45.0f), static_cast<float>(SCR_WIDTH/SCR_HEIGHT), 0.1f, 100.0f);
        ShaderProgram.setMat4("projection", projection);

        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr); // glDrawElements makes use of the indices in EBO and associates them with the vertices
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

int DrawTexturedCubeWithCoordinateSystem()
{
    // Drawing textured cube using VAO, VBO

    // set relative path of project from the location of the executable file
    std::string relPathExePro("../../LearnOpenGL/"+project_name+"/");

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

    // configure global opengl state
    glEnable(GL_DEPTH_TEST);

    // build and compile the shader program
    Shader ShaderProgram((relPathExePro+project_name+".vert").c_str(), (relPathExePro+project_name+".frag").c_str());

    // set up vertex data (retrieved directly from LearnOpenGL)
    // not really efficient as vertices are duplicated (however needs to be done as each vertex has different texture coordinates)
    const GLfloat vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    GLuint VAO;
    glGenVertexArrays(1, &VAO); // generate a VAO

    GLuint VBO;
    glGenBuffers(1, &VBO); // generate a VBO

    glBindVertexArray(VAO); // 1. bind the VAO
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // 2.1 bind the VBO with information about its type
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // 2.2 set VBO data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)nullptr); // 3.1 put VBO POSITION data into VAO i.e. into one of the attribute lists (attribute list # 0) of the VAO
    glEnableVertexAttribArray(0); // 3.2 enable the vertex attribute array to which VBO is registered i.e. 0
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float))); // 3.3 put VBO TEXTURE COORD data into VAO i.e. into one of the attribute lists (attribute list # 1) of the VAO
    glEnableVertexAttribArray(1); // 3.4 enable the vertex attribute array to which VBO is registered i.e. 1
    glBindBuffer(GL_ARRAY_BUFFER, 0); // 4. unbind VBO
    glBindVertexArray(0); // 5. unbind VAO

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
    data = stbi_load((relPathExePro+std::string("../common/resources/textures/awesomeface.png")).c_str(), &width, &height, &nrChannels, 0);
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

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);

        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

        // activate texture units and bind them
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textures[0]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textures[1]);

        // render container
        ShaderProgram.use();
        // set the texture mix value in the shader
        ShaderProgram.setFloat("mixValue", mixValueFromKey);

        // draw container and use different transforms (matrices) for using different coordinate systems
        glBindVertexArray(VAO);

        // create transformations and send them to the shader program by setting the uniforms

        // model matrix : local space to world (model) space
        glm::mat4 model = glm::mat4(1.0f);
//        model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, static_cast<float>(glfwGetTime()) * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
        ShaderProgram.setMat4("model", model);

        // view matrix : world (model) space to view (camera) space
        glm::mat4 view = glm::mat4(1.0f);
        view  = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        ShaderProgram.setMat4("view", view);

        // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
        // projection matrix: view (camera) space to clip space
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(45.0f), static_cast<float>(SCR_WIDTH/SCR_HEIGHT), 0.1f, 100.0f);
        ShaderProgram.setMat4("projection", projection);

        glDrawArrays(GL_TRIANGLES, 0, 36); // glDrawArrays makes use of the vertices directly stored in the VBO
        glBindVertexArray(0);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // de-allocate all resources once they've outlived their purpose:
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}

int DrawMultipleTexturedCubesWithCoordinateSystem()
{
    // Drawing multiple textured cubes (exact same cube) using VAO, VBO and different cube positions using only glm::translate (using no extra VBO etc. for drawing other cubes)

    // set relative path of project from the location of the executable file
    std::string relPathExePro("../../LearnOpenGL/"+project_name+"/");

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

    // configure global opengl state
    glEnable(GL_DEPTH_TEST);

    // build and compile the shader program
    Shader ShaderProgram((relPathExePro+project_name+".vert").c_str(), (relPathExePro+project_name+".frag").c_str());

    // set up vertex data (retrieved directly from LearnOpenGL)
    // not really efficient as vertices are duplicated (however needs to be done as each vertex has different texture coordinates)
    const GLfloat vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    glm::vec3 cubePositions[] = {
      glm::vec3( 0.0f,  0.0f,  0.0f),
      glm::vec3( 2.0f,  5.0f, -15.0f),
      glm::vec3(-1.5f, -2.2f, -2.5f),
      glm::vec3(-3.8f, -2.0f, -12.3f),
      glm::vec3( 2.4f, -0.4f, -3.5f),
      glm::vec3(-1.7f,  3.0f, -7.5f),
      glm::vec3( 1.3f, -2.0f, -2.5f),
      glm::vec3( 1.5f,  2.0f, -2.5f),
      glm::vec3( 1.5f,  0.2f, -1.5f),
      glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    GLuint VAO;
    glGenVertexArrays(1, &VAO); // generate a VAO

    GLuint VBO;
    glGenBuffers(1, &VBO); // generate a VBO

    glBindVertexArray(VAO); // 1. bind the VAO
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // 2.1 bind the VBO with information about its type
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // 2.2 set VBO data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)nullptr); // 3.1 put VBO POSITION data into VAO i.e. into one of the attribute lists (attribute list # 0) of the VAO
    glEnableVertexAttribArray(0); // 3.2 enable the vertex attribute array to which VBO is registered i.e. 0
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float))); // 3.3 put VBO TEXTURE COORD data into VAO i.e. into one of the attribute lists (attribute list # 1) of the VAO
    glEnableVertexAttribArray(1); // 3.4 enable the vertex attribute array to which VBO is registered i.e. 1
    glBindBuffer(GL_ARRAY_BUFFER, 0); // 4. unbind VBO
    glBindVertexArray(0); // 5. unbind VAO

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
    data = stbi_load((relPathExePro+std::string("../common/resources/textures/awesomeface.png")).c_str(), &width, &height, &nrChannels, 0);
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

    // projection matrix: view (camera) space to clip space
    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), static_cast<float>(SCR_WIDTH/SCR_HEIGHT), 0.1f, 100.0f);
    ShaderProgram.setMat4("projection", projection);

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);

        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

        // activate texture units and bind them
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textures[0]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textures[1]);

        // render container
        ShaderProgram.use();
        // set the texture mix value in the shader
        ShaderProgram.setFloat("mixValue", mixValueFromKey);

        glBindVertexArray(VAO);

        // draw containers in a loop and use different transforms (matrices) for using different coordinate systems
        for(int i = 0; i < 10; ++i)
        {
            // create transformations and send them to the shader program by setting the uniforms

            // model matrix : local space to world (model) space
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            if(i % 3 == 0) // make every third container rotate over time
                model = glm::rotate(model, static_cast<float>(glfwGetTime()) * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
            else // rest of the containers are just static
                model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

            ShaderProgram.setMat4("model", model);

            // view matrix : world (model) space to view (camera) space (in this case can also be set outside the loop as it's the same for all the containers
            glm::mat4 view = glm::mat4(1.0f);
            view  = glm::translate(view,  glm::vec3(0.0f, 0.0f, -3.0f));
            ShaderProgram.setMat4("view", view);
            glDrawArrays(GL_TRIANGLES, 0, 36); // glDrawArrays makes use of the vertices directly stored in the VBO

            // projection matrix: has been already set outside the main loop
        }

        glBindVertexArray(0);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // de-allocate all resources once they've outlived their purpose:
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}
