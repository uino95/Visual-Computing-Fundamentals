// Local headers
#include "program.hpp"
#include "gloom/gloom.hpp"
#include "gloom/shader.hpp"

unsigned int setUpVAO(float* coordinates, int* index, int cCount, int iCount);

void runProgram(GLFWwindow* window)
{
    // Enable depth (Z) buffer (accept "closest" fragment)
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Configure miscellaneous OpenGL settings
    glEnable(GL_CULL_FACE);

    // Set default colour after clearing the colour buffer
    glClearColor(0.1f, 0.3f, 0.9f, 1.0f);

    // Set up your scene here (create Vertex Array Objects, etc.)
    float coordinates1[] = {
        1.0, 0.0, 0.0,
        0.6, 0.6, 0.0,
        0.0, 0.6, 0.0,
    };

    float coordinates2[] = {
        -1.0f, 0.0f, 0.0f,
        0.0f, 0.6f, 0.0f,
        -0.6f, 0.6f, 0.0f,
    };

    float coordinates3[] = {
       -1.0f, 0.0f, 0.0f,
        -0.6f, -0.6f, 0.0f,
        0.0f, -0.6f, 0.0f,
    };

    float coordinates4[] = {
        1.0f, 0.0f, 0.0f,
        0.0f, -0.6f, 0.0f,
        0.6f, -0.6f, 0.0f,
    };

    float coordinates5[] = {
        -0.3, 0.0, 0.0,
        0.3, 0.0, 0.0,
        0.0, 0.3, 0.0,
    };

    float coordinates[] = {
        0.6, -0.8, -1.2,
        0.0, 0.4, 0.0,
        -0.8, -0.2, 0.0,
    };

    int index[] = {1,2,0};

    //int attributeIndex = glGetAttribLocation(shader.get(), "vec3");

    // unsigned int vaoID1 = setUpVAO(coordinates, index, 9, 3);
    unsigned int vaoID1 = setUpVAO(coordinates1, index, 9, 3);
    unsigned int vaoID2 = setUpVAO(coordinates2, index, 9, 3);
    unsigned int vaoID3 = setUpVAO(coordinates3, index, 9, 3);
    unsigned int vaoID4 = setUpVAO(coordinates4, index, 9, 3);
    unsigned int vaoID5 = setUpVAO(coordinates5, index, 9, 3);

    Gloom::Shader shader;
    // shader.attach("/Users/uino/Dev/gloom/gloom/shaders/simple.vert");
    // shader.attach("/Users/uino/Dev/gloom/gloom/shaders/simple.frag");
    // shader.link();
    shader.makeBasicShader("/Users/uino/Dev/gloom/gloom/shaders/simple.vert", "/Users/uino/Dev/gloom/gloom/shaders/simple.frag");
    shader.activate();


    // Rendering Loop
    while (!glfwWindowShouldClose(window))
    {
        // Clear colour and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw your scene here
        glBindVertexArray(vaoID1);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

        glBindVertexArray(vaoID2);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

        glBindVertexArray(vaoID3);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

        glBindVertexArray(vaoID4);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

        glBindVertexArray(vaoID5);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);


        // Handle other events
        glfwPollEvents();
        handleKeyboardInput(window);

        // Flip buffers
        glfwSwapBuffers(window);
    }
}


void handleKeyboardInput(GLFWwindow* window)
{
    // Use escape key for terminating the GLFW window
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

unsigned int setUpVAO(float* coordinates, int* index, int cCount, int iCount){

    unsigned int vaoID = 0;
    unsigned int vboID = 0;
    unsigned int indexID = 0;

    glGenVertexArrays(1, &vaoID);
    glBindVertexArray(vaoID);

    glGenBuffers(1, &vboID);
    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glBufferData(GL_ARRAY_BUFFER, cCount*sizeof(float), coordinates, GL_STATIC_DRAW);

    int attributeIndex = 0;
    glVertexAttribPointer(attributeIndex, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(attributeIndex);

    glGenBuffers(1, &indexID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, iCount*sizeof(int), index, GL_STATIC_DRAW);

    return vaoID;
}
