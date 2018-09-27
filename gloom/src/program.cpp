// Local headers
#include "program.hpp"
#include "gloom/gloom.hpp"
#include "gloom/shader.hpp"
#include <math.h>
#include <iostream>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../handout/sourceFiles/OBJLoader.hpp"

void runProgram(GLFWwindow* window){
    // Enable depth (Z) buffer (accept "closest" fragment)
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    //Enable transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Configure miscellaneous OpenGL settings
    glEnable(GL_CULL_FACE);

    // Set default colour after clearing the colour buffer
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    // Load the two shader
    Gloom::Shader shader;

    // Compile and link the two shader
    // make sure to select the right shader
    //          - simple.vert, basic vertex shader
    //          - mirror.vert, vertex shader which mirror the object both respect the x and y-axis
    //			- transformation, vertex shader which allow to move the camera applying the transformation
    //          - simple.frag, basic fragment shader
    //          - texture.frag, fragment shader which add a checkerboard texture on the object
    //          - changeColorInTime, fragment shader which change the color of the object during the time
    shader.makeBasicShader("../gloom/shaders/simple.vert", "../gloom/shaders/simple.frag");
    
    // Activate the two shaders
    shader.activate();

    // get the location of the uniform variable (I've to do like this because glsl version 330 doesn't support direct use of layout(location = 0)))
    int uniformLocation = glGetUniformLocation(shader.get(), "colorTimeOut");
    int uniformMatrixLocation = glGetUniformLocation(shader.get(), "transformMatrix");

    // Uncomment one of this to draw the corresponding object

    //drawFiveTriangles(window); 										//shaders: simple.vert and simple.frag
    //drawSingleTriangle(window); 										//shaders: simple.vert and simple.frag
    //drawCircle(window, 0.0, 0.0, 0.5); 								//shaders: simple.vert and simple.frag
    //drawSpiral(window, 0.0, 0.0, 0.5, 5); 							//shaders: simple.vert and simple.frag
    //drawChangingColorInTime(window, uniformLocation); 				//shaders: simple.vert and changeColorInTime.frag
    //drawTrheeOverlappingTriangle(window); 							//shaders: simple.vert and simple.frag
    //drawTransformation(window, uniformMatrixLocation); 				//shaders: transfromation.vert and simple.frag
    //camera(window, uniformMatrixLocation);                          //shaders: transfromation.vert and simple.frag
    drawSteve(window);
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

unsigned int setUpVAO(float* coordinates, int* index, int cCount, int iCount, int number_of_dimension){

    // Allocate space in memory for the VAO, VBO and the index buffer
    unsigned int vaoID = 0;
    unsigned int coordinatesID = 0;
    unsigned int colorID = 0;
    unsigned int indexID = 0;

    // Generate and bind the vertex array object
    glGenVertexArrays(1, &vaoID);
    glBindVertexArray(vaoID);

    // Generate and bind the Vertex Buffer Object for coordinates
    glGenBuffers(1, &coordinatesID);
    glBindBuffer(GL_ARRAY_BUFFER, coordinatesID);
    // Fill the buffer with the actual data
    glBufferData(GL_ARRAY_BUFFER, cCount, coordinates, GL_STATIC_DRAW);

    int attributeIndex = 0;
    // Fill the table of the VAO
    glVertexAttribPointer(attributeIndex, number_of_dimension, GL_FLOAT, GL_FALSE, 0, 0);
    // Enable the VAO
    glEnableVertexAttribArray(attributeIndex);

    // Generate and bind the index buffer
    glGenBuffers(1, &indexID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexID);
    // Fill the buffer with the actual data;
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, iCount, index, GL_STATIC_DRAW);

    // Return the Vao ID, in order to let the program designing it later 
    return vaoID;
}

unsigned int setUpVAOWithColor(std::vector<float4> vertices, int* index, int cCount, int iCount, int number_of_dimension, std::vector<float4> colors, int colorCount){

    // Allocate space in memory for the VAO, VBO and the index buffer
    unsigned int vaoID = 0;
    unsigned int coordinatesID = 0;
    unsigned int colorID = 0;
    unsigned int indexID = 0;

    // Generate and bind the vertex array object
    glGenVertexArrays(1, &vaoID);
    glBindVertexArray(vaoID);

    // Generate and bind the Vertex Buffer Object for coordinates
    glGenBuffers(1, &coordinatesID);
    glBindBuffer(GL_ARRAY_BUFFER, coordinatesID);
    
    std::vector<float> coordinates;
    for (int i = 0; i < steve.leftLeg.vertices.size(); ++i)
    {
        coordinates.push_back(vertices.at(i).x);
        coordinates.push_back(vertices.at(i).y);
        coordinates.push_back(vertices.at(i).z);
        coordinates.push_back(vertices.at(i).w);
    }
    // Fill the buffer with the actual data
    glBufferData(GL_ARRAY_BUFFER, cCount, coordinates, GL_STATIC_DRAW);

    int attributeIndex = 0;
    // Fill the table of the VAO
    glVertexAttribPointer(attributeIndex, number_of_dimension, GL_FLOAT, GL_FALSE, 0, 0);
    // Enable the VAO
    glEnableVertexAttribArray(attributeIndex);

    // Generate and bind the Vertex Buffer Object for colors
    glGenBuffers(1, &colorID);
    glBindBuffer(GL_ARRAY_BUFFER, colorID);

    std::vector<float> RGBAcolor;
    for (int i = 0; i < steve.leftLeg.vertices.size(); ++i)
    {
        RGBAcolor.push_back(colors.at(i).x);
        RGBAcolor.push_back(colors.at(i).y);
        RGBAcolor.push_back(colors.at(i).z);
        RGBAcolor.push_back(colors.at(i).w);
    }    
    // Fill the buffer with the actual data
    glBufferData(GL_ARRAY_BUFFER, colorCount, RGBAcolor, GL_STATIC_DRAW);
    // Fill the table of the VAO
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    // Generate and bind the index buffer
    glGenBuffers(1, &indexID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexID);
    // Fill the buffer with the actual data;
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, iCount, index, GL_STATIC_DRAW);

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
        -1.0, -0.8, 0.0,
        1.0, -0.2, 0.0,
        0.0, 0.4, 0.0,
    };

    int index[] = {0,1,2};

    float RGBAcolor[] = {1.0, 0.5, 0.5, 0.5,
    					0.5, 0.5, 1.0, 0.3,
    					0.5, 1.0, 0.5, 0.8};

    unsigned int vaoID = setUpVAOWithColor(coordinates, index, sizeof(coordinates), sizeof(index), 3, RGBAcolor, sizeof(RGBAcolor));

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

void drawTrheeOverlappingTriangle(GLFWwindow* window){
	//3 triangles coordinates to be changed if you want to draw differents triangles.
    float coordinates[] = {
        -0.5, 0.5, 0.0,
        -0.5, -0.5, 0.0,
        0.3, 0.0, 0.0,
        -0.5f, 0.7f, -0.3f,
       	0.0f, -0.3f, -0.3f,
        0.5f, 0.7f, -0.3f,
        0.5f, 0.5f, -0.6f,
        -0.3f, 0.0f, -0.6f,
        0.5f, -0.5f, -0.6f,
    };

    //index buffer to draw 3 triangles
    int index[] = {0,1,2,3,4,5,6,7,8};

    float RGBAcolor[] = {0.5, 1.0, 0.0, 0.3,
    					0.5, 1.0, 0.0, 0.3,
    					0.5, 1.0, 0.0, 0.3,
    					0.5, 0.5, 1.0, 0.5,
    					0.5, 0.5, 1.0, 0.5,
    					0.5, 0.5, 1.0, 0.5,
    					1.0, 0.5, 0.5, 0.8,
    					1.0, 0.5, 0.5, 0.8,
    					1.0, 0.5, 0.5, 0.8};

    int number_of_triangles = 3;

    // Set up the Vertex Array Objects to draw 3 triangles
   	unsigned int vaoID = setUpVAOWithColor(coordinates, index, sizeof(coordinates), sizeof(index), 3, RGBAcolor, sizeof(RGBAcolor));

    // Effective draw of the 3 triangles
    draw(window, vaoID, number_of_triangles * 3, 0);
}

void drawTransformation(GLFWwindow* window, int uniformLocation){
     //3 triangles coordinates to be changed if you want to draw differents triangles.
    float coordinates[] = {
        -0.5, 0.5, 0.0,
        -0.5, -0.5, 0.0,
        0.3, 0.0, 0.0,
        -0.5f, 0.7f, -0.2f,
        0.0f, -0.3f, -0.2f,
        0.5f, 0.7f, -0.2f,
        0.5f, 0.5f, -0.4f,
        -0.3f, 0.0f, -0.4f,
        0.5f, -0.5f, -0.4f,
    };

    //index buffer to draw 3 triangles
    int index[] = {6,7,8,3,4,5,0,1,2};

    float RGBAcolor[] = {0.5, 1.0, 0.0, 1.0,
                        0.5, 1.0, 0.0, 1.0,
                        0.5, 1.0, 0.0, 1.0,
                        0.5, 0.5, 1.0, 1.0,
                        0.5, 0.5, 1.0, 1.0,
                        0.5, 0.5, 1.0, 1.0,
                        1.0, 0.5, 0.5, 1.0,
                        1.0, 0.5, 0.5, 1.0,
                        1.0, 0.5, 0.5, 1.0};

    int number_of_triangles = 3;

    // Set up the Vertex Array Objects to draw 3 triangles
    unsigned int vaoID = setUpVAOWithColor(coordinates, index, sizeof(coordinates), sizeof(index), 3, RGBAcolor, sizeof(RGBAcolor));
    
    float i = 0.0f;
    short isOpposite = 0;

    while (!glfwWindowShouldClose(window))
    {
        // Clear colour and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Bind the current Vertex Array Object
        glBindVertexArray(vaoID);
    
        // Draw the current Vertex Array Object using mode GL_TRIANGLES
        glDrawElements(GL_TRIANGLES, number_of_triangles * 3, GL_UNSIGNED_INT, 0);

        // matrix is column major
        float matrix[] = 
        {
         1.0, i, 0.0, 0.0,
         0.0, 1.0, 0.0, 0.0,
         0.0, 0.0, 1.0, 0.0,
         0.0, 0.0, 0.0, 1.0
        };

        if (i >= 0.5f){
         isOpposite = 1;
        };

        if (i < 0.0f){
         isOpposite = 0;
        }

        if(isOpposite){
         i = i - 0.01f;
        } else {
         i = i + 0.01f;
        }

        // Modify the uniform value
        glUniformMatrix4fv(uniformLocation, 1,0, matrix);

        // Handle other events
        glfwPollEvents();
        handleKeyboardInput(window);

        // Flip buffers
        glfwSwapBuffers(window);
    }
}

void camera(GLFWwindow* window, int uniformLocation){

	 //3 triangles coordinates to be changed if you want to draw differents triangles.
    float coordinates[] = {
        -0.5, 0.5, -1.0,
        -0.5, -0.5, -1.0,
        0.3, 0.0, -1.0,
        -0.5f, 0.7f, -1.2f,
       	0.0f, -0.3f, -1.2f,
        0.5f, 0.7f, -1.2f,
        0.5f, 0.5f, -1.4f,
        -0.3f, 0.0f, -1.4f,
        0.5f, -0.5f, -1.4f,
    };

    //index buffer to draw 3 triangles
    int index[] = {6,7,8,3,4,5,0,1,2};

    float RGBAcolor[] = {0.5, 1.0, 0.0, 1.0,
    					0.5, 1.0, 0.0, 1.0,
    					0.5, 1.0, 0.0, 1.0,
    					0.5, 0.5, 1.0, 1.0,
    					0.5, 0.5, 1.0, 1.0,
    					0.5, 0.5, 1.0, 1.0,
    					1.0, 0.5, 0.5, 1.0,
    					1.0, 0.5, 0.5, 1.0,
    					1.0, 0.5, 0.5, 1.0};

    int number_of_triangles = 3;

    // Set up the Vertex Array Objects to draw 3 triangles
   	unsigned int vaoID = setUpVAOWithColor(coordinates, index, sizeof(coordinates), sizeof(index), 3, RGBAcolor, sizeof(RGBAcolor));
    
    // float i = 0.0f;
    // short isOpposite = 0;

    // x, y, z, x angle, y angle;
    float motion[7] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f};

    while (!glfwWindowShouldClose(window))
    {
        // Clear colour and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Bind the current Vertex Array Object
        glBindVertexArray(vaoID);
    
        // Draw the current Vertex Array Object using mode GL_TRIANGLES
        glDrawElements(GL_TRIANGLES, number_of_triangles * 3, GL_UNSIGNED_INT, 0);

        // Build the matrix that we need
        glm::mat4x4 matrix = glm::mat4();
        glm::mat4x4 matrixPerspective = glm::perspective(glm::pi<float>() * 0.5f , float(windowHeight/windowWidth), 1.0f, 100.0f);
        glm::vec3 TVector = glm::vec3(motion[0], motion[1], motion[2]);
        glm::mat4x4 TMatrix = glm::translate(matrix, TVector);
        glm::mat4x4 RXMatrix = glm::rotate(matrix, motion[3], glm::vec3(1.0f, 0.0f, 0.0f));
        glm::mat4x4 RYMatrix = glm::rotate(matrix, motion[4], glm::vec3(0.0f, 1.0f, 0.0f));
      
        // Compute the final matrix
        matrix = matrixPerspective * RXMatrix * RYMatrix * TMatrix;

        glUniformMatrix4fv(uniformLocation, 1,0, glm::value_ptr(matrix));

        // Handle other events
        glfwPollEvents();
        handleKeyboardInputMotion(window, motion);

        // Flip buffers
        glfwSwapBuffers(window);
    }
}

void handleKeyboardInputMotion(GLFWwindow* window, float* motion)
{
	// Use escape key for terminating the GLFW window
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	//right
	if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
		motion[0] = motion[0] - 0.01;
	}

	//left
	if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
		motion[0] = motion[0] + 0.01;
	}

	// Up
	if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
		motion[1] = motion[1] - 0.01;
	}

	// Down
	if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
		motion[1] = motion[1] + 0.01;
	}

	// Forward
	if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
		motion[2] = motion[2] + 0.01;
	}

	// Backward
	if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
		motion[2] = motion[2] - 0.01;
	}

	// Rotate up
	if(glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS){
		motion[3] = motion[3] - 0.01f;
	}

	// Rotate down
	if(glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS){
		motion[3] = motion[3] + 0.01f;
	}

	// Rotate right
	if(glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS){
		motion[4] = motion[4] + 0.01f;
	}

	// Rotate left
	if(glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS){
		motion[4] = motion[4] - 0.01f;
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

void drawSteve(GLFWwindow* window){

     // Load the minecraft object TODO ask where is the correct place for handout
    MinecraftCharacter steve = loadMinecraftCharacterModel("../handout/res/steve.obj"); 

    unsigned int vaoID1 = setUpVAOWithColor(steve.leftLeg.vertices, &steve.leftLeg.indices[0], sizeof(steve.leftLeg.vertices), sizeof(steve.leftLeg.indices), 4, steve.leftLeg.colours, sizeof(steve.leftLeg.colours));
    unsigned int vaoID2 = setUpVAOWithColor(steve.leftArm.vertices, &steve.leftArm.indices[0], sizeof(steve.leftArm.vertices), sizeof(steve.leftArm.indices), 4, steve.leftArm.colours, sizeof(steve.leftArm.colours));
    unsigned int vaoID3 = setUpVAOWithColor(steve.rightLeg.vertices, &steve.rightLeg.indices[0], sizeof(steve.rightLeg.vertices), sizeof(steve.rightLeg.indices), 4, steve.rightLeg.colours, sizeof(steve.rightLeg.colours));
    unsigned int vaoID4 = setUpVAOWithColor(steve.rightArm.vertices, &steve.rightArm.indices[0], sizeof(steve.rightArm.vertices), sizeof(steve.rightArm.indices), 4, steve.rightArm.colours, sizeof(steve.rightArm.colours));
    unsigned int vaoID5 = setUpVAOWithColor(steve.torso.vertices, &steve.torso.indices[0], sizeof(steve.torso.vertices), sizeof(steve.torso.indices), 4, steve.torso.colours, sizeof(steve.torso.colours));
    unsigned int vaoID6 = setUpVAOWithColor(steve.head.vertices, &steve.head.indices[0], sizeof(steve.head.vertices), sizeof(steve.head.indices), 4, steve.head.colours, sizeof(steve.head.colours));

    while (!glfwWindowShouldClose(window))
    {
        // Clear colour and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Bind the current Vertex Array Object
        glBindVertexArray(vaoID1);
        // Draw the current Vertex Array Object using mode GL_TRIANGLES
        glDrawElements(GL_TRIANGLES, steve.leftLeg.vertices.size(), GL_UNSIGNED_INT, 0);

        glBindVertexArray(vaoID2);
        glDrawElements(GL_TRIANGLES, steve.leftArm.vertices.size(), GL_UNSIGNED_INT, 0);

        glBindVertexArray(vaoID3);
        glDrawElements(GL_TRIANGLES, steve.rightLeg.vertices.size(), GL_UNSIGNED_INT, 0);

        glBindVertexArray(vaoID4);
        glDrawElements(GL_TRIANGLES, steve.rightArm.vertices.size(), GL_UNSIGNED_INT, 0);

        glBindVertexArray(vaoID5);
        glDrawElements(GL_TRIANGLES, steve.torso.vertices.size(), GL_UNSIGNED_INT, 0);

        glBindVertexArray(vaoID6);
        glDrawElements(GL_TRIANGLES, steve.head.vertices.size(), GL_UNSIGNED_INT, 0);

        // Handle other events
        glfwPollEvents();
        handleKeyboardInput(window);

        // Flip buffers
        glfwSwapBuffers(window);
    }
}




