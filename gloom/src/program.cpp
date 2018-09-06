// Local headers
#include "program.hpp"
#include "gloom/gloom.hpp"
#include "gloom/shader.hpp"
#include <math.h>
#include <iostream>

void runProgram(GLFWwindow* window){
    // Enable depth (Z) buffer (accept "closest" fragment)
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Configure miscellaneous OpenGL settings
    glEnable(GL_CULL_FACE);

    // Set default colour after clearing the colour buffer
    glClearColor(0.1f, 0.3f, 0.9f, 1.0f);

    // Load the two shader
    Gloom::Shader shader;

    // Compile and link the two shader
    // make sure to select the right shader
    //          - simple.vert, basic vertex shader
    //          - mirror.vert, vertex shader which mirror the object both respect the x and y-axis
    //          - simple.frag, basic fragment shader
    //          - texture.frag, fragment shader which add a checkerboard texture on the object
    //          - changeColorInTime, fragment shader which change the color of the object during the time
    shader.makeBasicShader("../gloom/shaders/mirror.vert", "../gloom/shaders/changeColorInTime.frag");
    
    // Activate the two shaders
    shader.activate();

    // get the location of the uniform variable (I've to do like this because glsl version 330 doesn't support direct use of layout(location = 0)))
    int uniformLocation = glGetUniformLocation(shader.get(), "colorTimeOut");

    // Uncomment one of this to draw the respective shape

    //drawFiveTriangles(window);
    //drawSingleTriangle(window);
    //drawCircle(window, 0.0, 0.0, 0.5);
    //drawSpiral(window, 0.0, 0.0, 0.5, 5);
    drawChangingColorInTime(window, uniformLocation);

}

void draw(GLFWwindow* window, unsigned int vaoID, int number_of_vertices, int mode){

    // Rendering Loop
    while (!glfwWindowShouldClose(window))
    {
        // Clear colour and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw your scene here
        
        // Bind the current Vertex Array Object
        glBindVertexArray(vaoID);

        // switch case to select the correct mode of drawing (I was't able to pass the enum mode as a parameter)

        switch(mode){
            case 0:
                // Draw the current Vertex Array Object using mode GL_TRIANGLES
                glDrawElements(GL_TRIANGLES, number_of_vertices, GL_UNSIGNED_INT, 0);
                break;
 
            case 1:
                // Draw the current Vertex Array Object using mode GL_TRIANGLE_FAN
                glDrawElements(GL_TRIANGLE_FAN, number_of_vertices, GL_UNSIGNED_INT, 0);
                break;
            
            case 2:
                // Draw the current Vertex Array Object using mode GL_LINE_STRIP
                glDrawElements(GL_LINE_STRIP, number_of_vertices, GL_UNSIGNED_INT, 0);
                break;

            default:
                // Draw the current Vertex Array Object using mode GL_TRIANGLES
                glDrawElements(GL_TRIANGLES, number_of_vertices, GL_UNSIGNED_INT, 0);
                break;
        }

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

unsigned int setUpVAO(float* coordinates, int* index, int cCount, int iCount, int number_of_dimension){

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
    // Fill the table of the VAO
    glVertexAttribPointer(attributeIndex, number_of_dimension, GL_FLOAT, GL_FALSE, 0, 0);
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

void drawFiveTriangles(GLFWwindow* window){

    //5 triangles coordinates to be changed if you want to draw differents triangles.
    float coordinates[] = {
        1.0, 0.0, 0.0,
        0.6, 0.6, 0.0,
        0.0, 0.6, 0.0,
        -0.6f, 0.6f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -0.6f, -0.6f, 0.0f,
        0.0f, -0.6f, 0.0f,
        0.6f, -0.6f, 0.0f,
        0.3f, 0.0f, 0.0f,
        0.0f, 0.3f, 0.0f,
        -0.3f, 0.0f, 0.0f,
    };

    //index buffer to draw 5 triangles
    int index[] = {0,1,2,2,3,4,4,5,6,6,7,0,8,9,10};

    int number_of_triangles = 5;

    // Set up the Vertex Array Objects to draw 5 triangles
    unsigned int vaoID = setUpVAO(coordinates, index, sizeof(coordinates), sizeof(index), 3);

    // Effective draw of the 5 triangles
    draw(window, vaoID, number_of_triangles * 3, 0);
}

void drawSingleTriangle(GLFWwindow* window){

    float coordinates[] = {
        -1.2, -0.8, 0.6,
        1.2, -0.2, -0.8,
        0.0, 0.4, 0.0,
    };

    int index[] = {0,1,2};

    unsigned int vaoID = setUpVAO(coordinates, index, sizeof(coordinates), sizeof(index), 3);

    draw(window, vaoID, 3, 0);
}

void drawCircle(GLFWwindow* window, float cx, float cy, float r)
{
	
    int number_of_triangles = 90;
    int number_of_dimension = 2;
    int num_segments = number_of_triangles + 2;
    float coordinates[number_of_triangles * 3];
    int index[number_of_triangles * 3];

    int n=2;
	//set the centre of the circle 
    coordinates[0] = 0.0; 
    coordinates[1] = 0.0; 	
    index[0] = 0;

    // Compute all the other vertices and fill the coordinates and index arrays
    for(int ii = 0; ii <= num_segments; ii++)
    {
        float theta = 2.0f * 3.14169f * float(ii) / float(num_segments);//get the current angle

        float x = r * cosf(theta);//calculate the x component
        float y = r * sinf(theta);//calculate the y component

        coordinates[n] = x + cx;
        coordinates[n+1] = y + cy;

        n += 2;
        
        index[ii+1]=ii+1;
  
    }

    unsigned int vaoID = setUpVAO(coordinates, index, number_of_triangles * 3 * number_of_dimension, number_of_triangles * 3, number_of_dimension);

    draw(window, vaoID, number_of_triangles * 3, 1);
}

void drawSpiral(GLFWwindow* window, float cx, float cy, float r, int times)
{   
    int number_of_dimension = 2;
    int number_of_vertices = 1200;
    int num_segments = number_of_vertices;

    float coordinates[number_of_vertices * number_of_dimension];
    int index[number_of_vertices];

	int n=0;
    for(int ii = 0; ii < num_segments; ii++)
    {
        float theta = 2.0f * 3.14169f * float(ii) / float(num_segments/times);//get the current angle

        float x = r * cosf(theta);//calculate the x component
        float y = r * sinf(theta);//calculate the y component

        coordinates[n] = x + cx;
        coordinates[n+1] = y + cy;

        n += 2;
        
        index[ii]=ii;

        r = r - r*times/num_segments;
    }

    unsigned int vaoID = setUpVAO(coordinates, index, number_of_vertices * number_of_dimension, number_of_vertices, number_of_dimension);

    draw(window, vaoID, number_of_vertices, 2);
}

void drawChangingColorInTime(GLFWwindow* window, int uniformLocation){
    float coordinates[] = {
        0.0, 0.0, 0.0,
        1.0, 0.0, 0.0,
        0.0, 1.0, 0.0,
    };

    int index[] = {0,1,2};

    unsigned int vaoID = setUpVAO(coordinates, index, sizeof(coordinates), sizeof(index), 3);

    float color = 0.0f;
    // Rendering Loop
    while (!glfwWindowShouldClose(window))
    {
        // Clear colour and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw your scene here
        
        // Bind the current Vertex Array Object
        glBindVertexArray(vaoID);

        // switch case to select the correct mode of drawing (I was't able to pass the enum mode as a parameter)

    
        // Draw the current Vertex Array Object using mode GL_TRIANGLES
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        
        glUniform4f(uniformLocation, 0.0f, color, 0.0f, 0.0f);
        color = color + 0.01f;
        if (color >= 1.0f){
            color = 0.0f;
        }

        // Handle other events
        glfwPollEvents();
        handleKeyboardInput(window);

        // Flip buffers
        glfwSwapBuffers(window);
    }

}