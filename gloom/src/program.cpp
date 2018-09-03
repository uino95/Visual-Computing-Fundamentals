// Local headers
#include "program.hpp"
#include "gloom/gloom.hpp"
#include "gloom/shader.hpp"
#include <math.h>
#include <iostream>

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
        0.5, 0.0, 0.0,
        0.5, 0.730, 0.0,
        0.0, 0.730, 0.0,
    };

    float coordinates5[] = {
        0.0, 0.0, 0.0,
        0.5, 0.0, 0.0,
        0.0, 0.730, 0.0,
    };

    float coordinates[] = {
        -1.2, -0.8, 0.6,
        1.2, -0.2, -0.8,
        0.0, 0.4, 0.0,
        
    };

    int index[] = {0,1,2};

    //Set up the Vertex Array Objects
    unsigned int vaoID = setUpVAO(coordinates1, index, sizeof(coordinates), sizeof(index));
    // unsigned int vaoID1 = setUpVAO(coordinates1, index, sizeof(coordinates1), sizeof(index));
    // unsigned int vaoID2 = setUpVAO(coordinates2, index, sizeof(coordinates2), sizeof(index));
    // unsigned int vaoID3 = setUpVAO(coordinates3, index, sizeof(coordinates3), sizeof(index));
    // unsigned int vaoID4 = setUpVAO(coordinates4, index, sizeof(coordinates4), sizeof(index));
    // unsigned int vaoID5 = setUpVAO(coordinates5, index, sizeof(coordinates5), sizeof(index));

    // Load the two shader
    Gloom::Shader shader;

    // Compile and link the two shader
    shader.makeBasicShader("/Users/uino/Dev/gloom/gloom/shaders/simple.vert", "/Users/uino/Dev/gloom/gloom/shaders/simple.frag");
    
    // Activate the two shaders
    shader.activate();

    // Rendering Loop
    while (!glfwWindowShouldClose(window))
    {
        // Clear colour and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw your scene here
        // Bind the current Vertex Array Object
        glBindVertexArray(vaoID);
        
        // // Draw the current Vertex Array Object
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

        // glBindVertexArray(vaoID2);
        // glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

        // glBindVertexArray(vaoID3);
        // glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

        // glBindVertexArray(vaoID4);
        // glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

        // glBindVertexArray(vaoID5);
        // glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);


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

	// Allocate space in memory for the VAO, VBO and the index buffer
    unsigned int vaoID = 0;
    unsigned int vboID = 0;
    unsigned int indexID = 0;

    // Generate and bind the vertex array object
    glGenVertexArrays(1, &vaoID);
    glBindVertexArray(vaoID);

    // Generate and bind the Vertex Buffer Object
    glGenBuffers(1, &vboID);
    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    // Fill the buffer with the actual data
    glBufferData(GL_ARRAY_BUFFER, cCount*sizeof(float), coordinates, GL_STATIC_DRAW);

    int attributeIndex = 0;
    // Fill the table of the VAO attributes with: index, number of objects, type of the values, normalised?, stride, pointer 
    glVertexAttribPointer(attributeIndex, 3, GL_FLOAT, GL_FALSE, 0, 0);
    // Enable the VAO
    glEnableVertexAttribArray(attributeIndex);

    // Generate and bind the index buffer
    glGenBuffers(1, &indexID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexID);
    // Fill the buffer with the actual data;
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, iCount*sizeof(int), index, GL_STATIC_DRAW);

    // Return the Vao ID, in order to let the program designing it later 
    return vaoID;
}

