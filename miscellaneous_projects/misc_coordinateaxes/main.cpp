// Draw coordinate axes in hello_camera_class project

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <shader.h>
#include <camera.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace
{
    std::string project_folder = "miscellaneous_projects";
    std::string project_name = "misc_coordinateaxes";

    // screen settings
    const GLuint SCR_WIDTH = 800;
    const GLuint SCR_HEIGHT = 600;

    Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

    // initial mix value for how much we're seeing of either texture
    float mixValueFromKey = 0.2f;

    // setup for keyboard (w-a-s-d) movement
    float deltaTime = 0.0f;	// Time between current frame and last frame
    float lastFrame = 0.0f; // Time of last frame

    // setup for mouse movement
    float lastX = SCR_WIDTH/2.0f;
    float lastY = SCR_HEIGHT/2.0f;
    bool firstTimeMouseMoved(true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window, bool keyboardCamera = true);

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xScrollOffset, double yScrollOffset);

int drawCoordinateAxes();

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window, bool keyboardCamera)
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

    if(keyboardCamera) {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera.ProcessKeyboard(FORWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera.ProcessKeyboard(BACKWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera.ProcessKeyboard(LEFT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera.ProcessKeyboard(RIGHT, deltaTime);
    }

//    // if the camera should be restricted to the ground level
//    if(keyboardCamera) {
//        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
//            camera.ProcessKeyboard(FORWARD, deltaTime, true);
//        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
//            camera.ProcessKeyboard(BACKWARD, deltaTime, true);
//        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
//            camera.ProcessKeyboard(LEFT, deltaTime, true);
//        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
//            camera.ProcessKeyboard(RIGHT, deltaTime, true);
//    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if(firstTimeMouseMoved)
    {
        lastX = xpos;
        lastY = ypos;
        firstTimeMouseMoved = false;
    }

    float xMouseOffset = xpos - lastX;
    float yMouseOffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xMouseOffset, yMouseOffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
void scroll_callback(GLFWwindow* window, double xScrollOffset, double yScrollOffset)
{
    camera.ProcessMouseScroll(yScrollOffset);
}

//  ----------------------------------------MAIN METHOD--------------------------------------------

int main()
{
    return drawCoordinateAxes();
}

//  -----------------------------------------------------------------------------------------------

int drawCoordinateAxes()
{
    // Drawing coordinate axes along with multiple textured cubes (exact same cube) using VAO, VBO and different cube positions using only glm::translate (using no extra VBO etc. for drawing other cubes)
    // and a mouse controlled camera object

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
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
//      glm::vec3( 0.0f,  0.0f,  0.0f),
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

    // For drawing coordinate axes
    float c = 0.5;
    const GLfloat verticesAxes[] = {
           0.0f,    0.0f,  0.0f,  0.0f, 0.0f,
         c*0.95f,    0.0f,  0.0f,  0.0f, 0.0f,
         c*0.95f,    0.0f,  0.0f,  0.0f, 0.0f,
         c*0.9f,  c*0.05f,  0.0f,  0.0f, 0.0f,
         c*0.95f,    0.0f,  0.0f,  0.0f, 0.0f,
         c*0.9f, c*-0.05f,  0.0f,  0.0f, 0.0f,

           0.0f,    0.0f,  0.0f,  0.0f, 0.0f,
           0.0f,  c*0.95f,  0.0f,  0.0f, 0.0f,
           0.0f,  c*0.95f,  0.0f,  0.0f, 0.0f,
         c*0.05f,  c*0.9f,  0.0f,  0.0f, 0.0f,
           0.0f,  c*0.95f,  0.0f,  0.0f, 0.0f,
        c*-0.05f,  c*0.9f,  0.0f,  0.0f, 0.0f,

           0.0f,     0.0f,    0.0f,  0.0f, 0.0f,
           0.0f,     0.0f,  c*0.95f,  0.0f, 0.0f,
           0.0f,     0.0f,  c*0.95f,  0.0f, 0.0f,
           0.0f,   c*0.05f,  c*0.9f,  0.0f, 0.0f,
           0.0f,     0.0f,  c*0.95f,  0.0f, 0.0f,
           0.0f,  c*-0.05f,  c*0.9f,  0.0f, 0.0f,
    };


    GLuint VAO_coordaxes;
    glGenVertexArrays(1, &VAO_coordaxes); // generate a VAO for coordinateaxes

    GLuint VBO_coordaxes;
    glGenBuffers(1, &VBO_coordaxes); // generate a VBO for coordinateaxes

    glBindVertexArray(VAO_coordaxes); // 1. bind the VAO
    glBindBuffer(GL_ARRAY_BUFFER, VBO_coordaxes); // 2.1 bind the VBO with information about its type
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesAxes), verticesAxes, GL_STATIC_DRAW); // 2.2 set VBO data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)nullptr); // 3.1 put VBO POSITION data into VAO i.e. into one of the attribute lists (attribute list # 0) of the VAO
    glEnableVertexAttribArray(0); // 3.2 enable the vertex attribute array to which VBO is registered i.e. 0
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float))); // 3.3 put VBO TEXTURE COORD data into VAO i.e. into one of the attribute lists (attribute list # 1) of the VAO
    glEnableVertexAttribArray(1); // 3.4 enable the vertex attribute array to which VBO is registered i.e. 1
    glBindBuffer(GL_ARRAY_BUFFER, 0); // 4. unbind VBO
    glBindVertexArray(0); // 5. unbind VAO


    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // tell opengl for each sampler (uniforms) to which texture unit it belongs to
    ShaderProgram.use(); // activate the shader before setting uniforms
    glUniform1i(glGetUniformLocation(ShaderProgram.ID, "texture1"), 0); // set it manually
    ShaderProgram.setInt("texture2", 1); // or with shader class uniform utility function

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        processInput(window);
//        processInput(window, false); // disabling keyboard camera movement through w-a-s-d

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

        // view matrix : world (model) space to view (camera) space
        glm::mat4 view = glm::mat4(1.0f);
        view = camera.GetViewMatrix();
        ShaderProgram.setMat4("view", view);

        // projection matrix: view (camera) space to clip space
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), static_cast<float>(SCR_WIDTH/SCR_HEIGHT), 0.1f, 100.0f);
        ShaderProgram.setMat4("projection", projection);

        glBindVertexArray(VAO);

        // draw containers in a loop and use different transforms (matrices) for using different coordinate systems
        for(int i = 0; i < 9; ++i)
        {
            // create transformations and send them to the shader program by setting the uniforms

            // model matrix : local space to world (model) space
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            model = glm::rotate(model, glm::radians(20.0f*i), glm::vec3(1.0f, 0.0f, 0.0f));
            ShaderProgram.setMat4("model", model);

            // view matrix: has been set outside this loop as it's the same for all the containers

            // projection matrix: has been set outside this loop and not outside the render loop as it could change every frame due to zooming (scrolling)

            glDrawArrays(GL_TRIANGLES, 0, 36); // glDrawArrays makes use of the vertices directly stored in the VBO
        }

        glBindVertexArray(0);

        // Draw coordinate axes
        glBindVertexArray(VAO_coordaxes);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3( 0.0f,  0.0f, 0.0f));
        ShaderProgram.setMat4("model", model);
        glDrawArrays(GL_LINES, 0, 18);
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
