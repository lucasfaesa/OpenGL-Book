#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const std::string vertex_shader_source = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        void main()
        {
            gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
        }
    )";

const std::string fragment_shader_source = R"(
        #version 330 core
        out vec4 FragColor;
        void main()
        {
            FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
        }
    )";

const std::string fragment_shader_source_red = R"(
        #version 330 core
        out vec4 FragColor;
        void main()
        {
            FragColor = vec4(1.0f, 0.f, 0.f, 1.0f);
        }
    )";

int main() {

    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //setting the opengl being used as ver. 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //setting the opengl profile as Core

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char* vertex_shader_code = vertex_shader_source.c_str();
    glShaderSource(vertexShader, 1, &vertex_shader_code, NULL);
    glCompileShader(vertexShader);

    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);;
    const char* fragment_shader_code = fragment_shader_source.c_str();
    glShaderSource(fragmentShader, 1, &fragment_shader_code, NULL);
    glCompileShader(fragmentShader);

    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    //glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //red frag shader
    unsigned int redFragShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* redFragShaderCode = fragment_shader_source_red.c_str();
    glShaderSource(redFragShader, 1, &redFragShaderCode, NULL);
    glCompileShader(redFragShader);

    unsigned int redShaderProgram = glCreateProgram();
    glAttachShader(redShaderProgram, vertexShader);
    glAttachShader(redShaderProgram, redFragShader);
    glLinkProgram(redShaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(redFragShader);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] ={
         0.5f, 0.5f, 0.0f, // top right
         0.5f, -0.5f, 0.0f, // bottom right
         -0.5f, -0.5f, 0.0f, // bottom left
         -0.5f,  0.5f, 0.0f  // top left
    };

    //indices for the element buffer (EBO)
    unsigned int indices[] = {
        0, 1, 3, //first triangle
        1, 2, 3 // second triangle
    };

    float triangleVertices[] = {
        -0.5f, -0.5f, 0.1f,
        0.5f, -0.5f, 0.1f,
        0.0f, 0.5f, 0.1f
    };

    //VBO - Moves your vertex data from CPU memory to GPU memory, the GPU can only work with whats already in its own memory, fast access once its there
    //VAO - Once the data is on the GPU, OpenGL still doesnt know how to read it, are those numbers positions? colors? how many per vertex? The VAO stores that answer.
    //EBO - Lets you reuse vertices. Instead of repeating shared corners, you write each vertex once and reference them by number. A rectangle needs 4 vertices, not 6.
    //Vertex shader - Runs once per vertex. It's job: take a 3D point and figure out where it lands on your 2D screen

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //second triangle
    unsigned int secondTriangleVBO, secondTriangleVAO;
    glGenVertexArrays(1, &secondTriangleVAO);
    glGenBuffers(1, &secondTriangleVBO);

    glBindVertexArray(secondTriangleVAO);

    glBindBuffer(GL_ARRAY_BUFFER, secondTriangleVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //wireframe mode
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        process_input(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawElements(GL_TRIANGLES,6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        //drawing triangle old way
        glUseProgram(redShaderProgram);
        glBindVertexArray(secondTriangleVAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0,0,width, height);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void process_input(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

