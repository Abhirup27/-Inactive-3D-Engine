#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
const char *vertexShaderSource =
#include "shaders/test.vs"
;
const char *fragmentShaderSource =
#include "shaders/Tfrag.fs"
;
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        std::cout<<glfwGetCurrentContext()<<std::endl;
        glViewport(0, 0, width, height);
    }

void processInput(GLFWwindow *window)
    {

        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {

            glfwSetWindowShouldClose(window,true);
        }
    }

void getShCompileError(unsigned int& Shader)
{

    int success;
    char infoLog[512];
    glGetShaderiv(Shader, GL_COMPILE_STATUS, &success);

    if(!success)
    {

        glGetShaderInfoLog(Shader, 512, NULL, infoLog);
        std::cout<<"ERROR:: SHADER::VERTEX_AND_FRAGMENT::COMPILATION_FAILED\n"<<
                    infoLog<<std::endl;
    }
}

void getShProgLinkError(unsigned int& Program)
{
    int success;
    char infoLog[512];
    glGetProgramiv(Program, GL_LINK_STATUS, &success);

    if(!success)
    {
        glGetProgramInfoLog(Program,512,NULL, infoLog);
        std::cout<<"ERROR:: SHADER_PROGRAM:: LINKING_FAILED\n"<<
                    infoLog<<std::endl;
    }
}
int main()
{
    GLfloat rvalue=0.1f,gvalue =0.2f, bvalue= 0.3f;
    float vertices[] = {
        0.5f, 0.5f,0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f,-0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f
    };
    unsigned int indices[] = {
    0, 1, 3,
    1, 2, 3
    };
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4.6);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1920, 1080, "Enlist 3D", NULL,NULL);
    if(window == NULL)
    {

        std::cout<<"Failed to create a window"<<std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {

        std::cout<<"Failed to initialize GLAD"<<std::endl;
        return -1;
    }
    glViewport(0, 0, 1920, 1080);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    //Creating buffer object VBO ||Generating buffer, giving it an ID then setting the buffer type to array buffer(because it is a vertex buffer)
    unsigned int VBO;
    glGenBuffers(1, &VBO); // using OpenGL object and creating a single buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO); //the function binds VBO to ARRAY BUFFER which is a vertex type of buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),vertices, GL_STATIC_DRAW); //any buffer calls we make with ARRAY BUFFER as parameter/target will be configured/changed to the bounded buffer which is VBO right now, the vertex data is now stored in VBO

    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices),indices, GL_STATIC_DRAW);

    //Creating Vertex Shader Object
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER); //creating object and telling it is a vertex
    glShaderSource(vertexShader,1,&vertexShaderSource,NULL); //binding the ShaderSourceCode to vertexShader object for compiling, the "1" means there is only 1 string of Source code we are passing
    glCompileShader(vertexShader); //openGL compiles it in the (GPU maybe??)
    getShCompileError(vertexShader);

    //creating Fragment Shader Object
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader,1, &fragmentShaderSource,NULL);
    glCompileShader(fragmentShader);
    getShCompileError(fragmentShader);

    //creating shader program object, linking outputs of vertex shader to its next shader and fragment shader to next shader
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    getShProgLinkError(shaderProgram);


    glVertexAttribPointer(0,3, GL_FLOAT, GL_FALSE, 3* sizeof(float),(void*)0);  //Now we provide the vertex data from the buffer to the attribute of the shader program
    glEnableVertexAttribArray(0);


    glUseProgram(shaderProgram); //Use shaderProgram object in shader or render calls
    glDeleteShader(vertexShader); //deleting shader objects since we have linked the 2 shader programs
    glDeleteShader(fragmentShader);
    glBindVertexArray(0);

    //Call Back functions
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); //after the window is created, SetFramebufferSizeCallback checks if the window size is changed and calls frame_size_callback inside its definition providing width & height as parameters
    while(!glfwWindowShouldClose(window))
    {
        rvalue += 0.001f, gvalue += 0.0005f, bvalue += 0.0001f;
        //check for inputs
        processInput(window);

        //rendering commands here

        glClearColor(rvalue,gvalue,bvalue,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //ALWAYS DRAW AFTER UPDATING THE BUFFER!
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
         glBindVertexArray(0);
        //check and call for events and swap the front and back buffer
        glfwPollEvents();
        glfwSwapBuffers(window);

    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
