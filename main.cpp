

#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
#include <GL/gl3w.h>            // Initialize with gl3wInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
#include <GL/glew.h>            // Initialize with glewInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
#include <glad/glad.h>          // Initialize with gladLoadGL()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD2)
#include <glad/gl.h>            // Initialize with gladLoadGL(...) or gladLoaderLoadGL()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING2)
#define GLFW_INCLUDE_NONE       // GLFW including OpenGL headers causes ambiguity or multiple definition errors.
#include <glbinding/Binding.h>  // Initialize with glbinding::Binding::initialize()
#include <glbinding/gl/gl.h>
using namespace gl;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING3)
#define GLFW_INCLUDE_NONE       // GLFW including OpenGL headers causes ambiguity or multiple definition errors.
#include <glbinding/glbinding.h>// Initialize with glbinding::initialize()
#include <glbinding/gl/gl.h>
using namespace gl;
#endif


#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include<SHADER_S.h>

#include<iostream>

#include<GLFW/glfw3.h>
#include<math.h>

/*
const char *vertexShaderSource =
#include "shaders/test.vs"
;
const char *fragmentShaderSource =
#include "shaders/Tfrag.fs"
;
*/
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif
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
    //positions        //Colors
        0.5f, 0.5f,0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f,-0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f, 0.0f, 0.1f, 0.15f, 0.5f
    };
    unsigned int indices[] = {
    0, 1, 3,
    1, 2, 3
    };
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4.6);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    const char* glsl_version = "#version 130";
    GLFWwindow* window = glfwCreateWindow(1920, 1080, "Enlist 3D", NULL,NULL);

    if(window == NULL)
    {

        std::cout<<"Failed to create a window"<<std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    #if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
    bool err = gl3wInit() != 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
    bool err = gladLoadGL() == 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD2)
    bool err = gladLoadGL(glfwGetProcAddress) == 0; // glad2 recommend using the windowing library loader instead of the (optionally) bundled one.
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING2)
    bool err = false;
    glbinding::Binding::initialize();
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING3)
    bool err = false;
    glbinding::initialize([](const char* name) { return (glbinding::ProcAddress)glfwGetProcAddress(name); });
#else
    bool err = false; // If you use IMGUI_IMPL_OPENGL_LOADER_CUSTOM, your loader is likely to requires some form of initialization.
#endif
    if (err)
    {
        fprintf(stderr, "Failed to initialize OpenGL loader!\n");
        return 1;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

     // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {

        std::cout<<"Failed to initialize GLAD"<<std::endl;
        return -1;
    }
    glViewport(0, 0, 1920, 1080);

     // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);

    ImGui_ImplOpenGL3_Init(glsl_version);
    io.Fonts->AddFontFromFileTTF("OpenSans-Regular.ttf", 38.0f);

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


    Shader ourShader("shaders/test.vs","shaders/Tfrag.fs");

    /*
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

    */
    glVertexAttribPointer(0,3, GL_FLOAT, GL_FALSE, 6* sizeof(float),(void*)0);  //Now we provide the vertex data from the buffer to the attribute of the shader program
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,3, GL_FLOAT, GL_FALSE, 6* sizeof(float),(void*)(3*(sizeof(float))));
    glEnableVertexAttribArray(1);

    //glUseProgram(shaderProgram); //Use shaderProgram object in shader or render calls
    //glDeleteShader(vertexShader); //deleting shader objects since we have linked the 2 shader programs
    //glDeleteShader(fragmentShader);
    glBindVertexArray(0);


    // Our state
    bool show_demo_window = true;
    bool show_another_window = true;
    bool use_sin_func =false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);



    //Call Back functions
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); //after the window is created, SetFramebufferSizeCallback checks if the window size is changed and calls frame_size_callback inside its definition providing width & height as parameters
    while(!glfwWindowShouldClose(window))
    {

        //rvalue += 0.001f, gvalue += 0.0005f, bvalue += 0.0001f;
        //check for inputs
        processInput(window);

        //rendering commands here

        glClearColor(rvalue,gvalue,bvalue,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        float blueValue= 1.0f;
        float timeValue = glfwGetTime();
        float greenValue = 0.2f;  //(sin(timeValue)/ 2.0f) +0.5f;
        float redValue = 0.5f; //(sin(timeValue)/ 2.5f) *1.86f;
        //int vertexColorLocation = glGetUniformLocation(shaderProgram, "GreenVal");
        //int vertexColorLocation2 = glGetUniformLocation(shaderProgram, "RedVal");

        if(use_sin_func== true)
        {
        rvalue = (sin(timeValue)/ 2.0f) +0.7f;
        gvalue = (sin(timeValue)/ 2.09f) +0.5f;
        bvalue = (sin(timeValue)/ 2.3f) +0.5f;
        }

        //ALWAYS DRAW AFTER UPDATING THE BUFFER!
        //glUseProgram(shaderProgram);
        //glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
        //glUniform1f(vertexColorLocation, greenValue);
        //glUniform1f(vertexColorLocation2, redValue);
        glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
         glBindVertexArray(0);
        //check and call for events and swap the front and back buffer
        glfwPollEvents();

        // Start the Dear ImGui frame

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("WINDOW SETTINGS");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("TEST WINDOW.",48.0f);               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);
            ImGui::Checkbox("Use sine function ", &use_sin_func);
            ImGui::SliderFloat("Window Red Color", (float*)&rvalue, 0.0f, 1.0f);
            ImGui::SliderFloat("Window Green Color", (float*)&gvalue, 0.0f, 1.0f);
            ImGui::SliderFloat("Window Blue Color", (float*)&bvalue, 0.0f, 1.0f);
            ImGui::SliderFloat3("Rectangle RGB", (float*)&clear_color, 0.0f, 1.0f);
                       // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        // 3. Show another simple window.
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }
        ourShader.use();
        ourShader.setFloat("GreenVal",clear_color.y);
        ourShader.setFloat("RedVal", clear_color.x);
        ourShader.setFloat("BluVal", clear_color.z);
        // Rendering
        ImGui::Render();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);

    }
        ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();


    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
//    glDeleteProgram(shaderProgram);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
