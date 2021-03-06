# LearnOpenGL
 My projects based on Joey de Vries' online material and tutorials for OpenGL https://learnopengl.com/
 
 The projects have been written in Qt Creator on a Linux system. 
 
 As per the instructions in the tutorial (see https://learnopengl.com/Getting-started/Creating-a-window):
 
 1. For rendering OpenGL context I use the GLFW library - built the library from its source repository (https://github.com/glfw/glfw) using cmake.
 2. I use GLAD (https://github.com/Dav1dde/glad) as a Loader-Generator for OpenGL.
 
## Projects

### Getting started (hello) projects:

- hello_world : Project demonstrating basic opengl commands on how to create an empty window with a background color.
- hello_triangle: Drawing triangles in a window using vertex and fragment shaders, Vertex Buffer (VBO) and Attribute (VAO) Objects.
- hello_rectangle: Drawing a rectangle using the Element Buffer Object (EBO) along with shaders, VBO and VAO. 
- hello_texture: Drawing a rectangle (shaders, EBO, VBO, VAO) with texture object(s).
- hello_transformation: Drawing rectangle(s) with multiple textures and tranformations (scaling, rotation, translation).
- hello_coordinatesystem: Drawing rectangular object/cube(s) with multiple textures (wire-frame) and using different coordinate system matrices and tranformations.
- hello_camera: Drawing cubes with multiple textures and implementing camera functionality with simple revolving camera (in xz plane), keyboard-mouse controlled camera using different coordinate system matrices and tranformations.
- hello_camera_class: Drawing cubes with multiple textures and implementing a separate camera class object.


### Miscellaneous (misc) projects:

- misc_coordinateaxes : Project for drawing coordinate axes in hello_camera_class project.
