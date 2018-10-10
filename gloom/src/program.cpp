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

#include "sceneGraph.hpp"

#define PI 3.14159265

void runProgram(GLFWwindow *window)
{
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

    // // Compile and link the two shader
    // // make sure to select the right shader
    // //          - simple.vert, basic vertex shader
    // //          - mirror.vert, vertex shader which mirror the object both respect the x and y-axis
    // //           - transformation, vertex shader which allow to move the camera applying the transformation
    // //          - simple.frag, basic fragment shader
    // //          - texture.frag, fragment shader which add a checkerboard texture on the object
    // //          - changeColorInTime, fragment shader which change the color of the object during the time
    shader.makeBasicShader("../gloom/shaders/transformation.vert", "../gloom/shaders/simple.frag");

    // // Activate the two shaders
    shader.activate();

    // // get the location of the uniform variable (I've to do like this because glsl version 330 doesn't support direct use of layout(location = 0)))
    //int uniformLocation = glGetUniformLocation(shader.get(), "colorTimeOut");
    int uniformMatrixLocation = glGetUniformLocation(shader.get(), "transformMatrix");

    // Uncomment one of this to draw the corresponding object

    //drawFiveTriangles(window);                                        //shaders: simple.vert and simple.frag
    //drawSingleTriangle(window);                                       //shaders: simple.vert and simple.frag
    //drawCircle(window, 0.0, 0.0, 0.5);                                //shaders: simple.vert and simple.frag
    //drawSpiral(window, 0.0, 0.0, 0.5, 5);                             //shaders: simple.vert and simple.frag
    //drawChangingColorInTime(window, uniformLocation);                 //shaders: simple.vert and changeColorInTime.frag
    //drawTrheeOverlappingTriangle(window);                             //shaders: simple.vert and simple.frag
    //drawTransformation(window, uniformMatrixLocation);                //shaders: transformation.vert and simple.frag
    //camera(window, uniformMatrixLocation);                            //shaders: transformation.vert and simple.frag
    //drawSteve(window, uniformMatrixLocation);                         //shaders: transformation.vert and simple.frag

    //printScene(constructSceneGraph());
    drawScene(window, uniformMatrixLocation);                           //shaders: transfromation.vert and simple.frag
}

void draw(GLFWwindow *window, unsigned int vaoID, int number_of_vertices, int mode)
{

    // Rendering Loop
    while (!glfwWindowShouldClose(window))
    {
        // Clear colour and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw your scene here

        // Bind the current Vertex Array Object
        glBindVertexArray(vaoID);

        // switch case to select the correct mode of drawing (I was't able to pass the enum mode as a parameter)

        switch(mode)
        {
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

unsigned int setUpVAO(float *coordinates, int *index, int cCount, int iCount, int number_of_dimension)
{

    // Allocate space in memory for the VAO, VBO and the index buffer
    unsigned int vaoID = 0;
    unsigned int coordinatesID = 0;
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

unsigned int setUpVAOWithColor(float *coordinates, int *index, int cCount, int iCount, int number_of_dimension, float *RGBAcolor, int colorCount)
{

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

    // Generate and bind the Vertex Buffer Object for colors
    glGenBuffers(1, &colorID);
    glBindBuffer(GL_ARRAY_BUFFER, colorID);
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

unsigned int setUpVAOWithColorFloat4(std::vector<float4> vertices, unsigned int *index, int cCount, int iCount, int number_of_dimension, std::vector<float4> colors, int colorCount)
{

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

    std::vector<float> v;
    for (unsigned int i = 0; i < vertices.size(); i++)
    {
        v.push_back(vertices.at(i).x);
        v.push_back(vertices.at(i).y);
        v.push_back(vertices.at(i).z);
        v.push_back(vertices.at(i).w);
    }
    // Fill the buffer with the actual data
    glBufferData(GL_ARRAY_BUFFER, cCount * sizeof(float), &v[0], GL_STATIC_DRAW);

    int attributeIndex = 0;
    // Fill the table of the VAO
    glVertexAttribPointer(attributeIndex, number_of_dimension, GL_FLOAT, GL_FALSE, 0, 0);
    // Enable the VAO
    glEnableVertexAttribArray(attributeIndex);

    // Generate and bind the Vertex Buffer Object for colors
    glGenBuffers(1, &colorID);
    glBindBuffer(GL_ARRAY_BUFFER, colorID);

    std::vector<float> c;
    for (unsigned int i = 0; i < colors.size(); ++i)
    {
        c.push_back(colors.at(i).x);
        c.push_back(colors.at(i).y);
        c.push_back(colors.at(i).z);
        c.push_back(colors.at(i).w);
    }

    // Fill the buffer with the actual data
    glBufferData(GL_ARRAY_BUFFER, colorCount * sizeof(float), &c[0], GL_STATIC_DRAW);
    // Fill the table of the VAO
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    // Generate and bind the index buffer
    glGenBuffers(1, &indexID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexID);
    // Fill the buffer with the actual data;
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, iCount * sizeof(unsigned int), index, GL_STATIC_DRAW);

    // Return the Vao ID, in order to let the program designing it later
    return vaoID;
}

void drawFiveTriangles(GLFWwindow *window)
{

    //5 triangles coordinates to be changed if you want to draw differents triangles.
    float coordinates[] =
    {
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
    int index[] = {0, 1, 2, 2, 3, 4, 4, 5, 6, 6, 7, 0, 8, 9, 10};

    int number_of_triangles = 5;

    // Set up the Vertex Array Objects to draw 5 triangles
    unsigned int vaoID = setUpVAO(coordinates, index, sizeof(coordinates), sizeof(index), 3);

    // Effective draw of the 5 triangles
    draw(window, vaoID, number_of_triangles * 3, 0);
}

void drawSingleTriangle(GLFWwindow *window)
{

    float coordinates[] =
    {
        -1.0, -0.8, 0.0,
            1.0, -0.2, 0.0,
            0.0, 0.4, 0.0,
        };

    int index[] = {0, 1, 2};

    float RGBAcolor[] = {1.0, 0.5, 0.5, 0.5,
                         0.5, 0.5, 1.0, 0.3,
                         0.5, 1.0, 0.5, 0.8
                        };

    unsigned int vaoID = setUpVAOWithColor(coordinates, index, sizeof(coordinates), sizeof(index), 3, RGBAcolor, sizeof(RGBAcolor));

    draw(window, vaoID, 3, 0);
}

void drawCircle(GLFWwindow *window, float cx, float cy, float r)
{

    int number_of_triangles = 90;
    int number_of_dimension = 2;
    int num_segments = number_of_triangles + 2;
    float coordinates[number_of_triangles * 3];
    int index[number_of_triangles * 3];

    int n = 2;
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
        coordinates[n + 1] = y + cy;

        n += 2;

        index[ii + 1] = ii + 1;

    }

    unsigned int vaoID = setUpVAO(coordinates, index, number_of_triangles * 3 * number_of_dimension, number_of_triangles * 3, number_of_dimension);

    draw(window, vaoID, number_of_triangles * 3, 1);
}

void drawSpiral(GLFWwindow *window, float cx, float cy, float r, int times)
{
    int number_of_dimension = 2;
    int number_of_vertices = 1200;
    int num_segments = number_of_vertices;

    float coordinates[number_of_vertices * number_of_dimension];
    int index[number_of_vertices];

    int n = 0;
    for(int ii = 0; ii < num_segments; ii++)
    {
        float theta = 2.0f * 3.14169f * float(ii) / float(num_segments / times); //get the current angle

        float x = r * cosf(theta);//calculate the x component
        float y = r * sinf(theta);//calculate the y component

        coordinates[n] = x + cx;
        coordinates[n + 1] = y + cy;

        n += 2;

        index[ii] = ii;

        r = r - r * times / num_segments;
    }

    unsigned int vaoID = setUpVAO(coordinates, index, number_of_vertices * number_of_dimension, number_of_vertices, number_of_dimension);

    draw(window, vaoID, number_of_vertices, 2);
}

void drawChangingColorInTime(GLFWwindow *window, int uniformLocation)
{
    float coordinates[] =
    {
        0.0, 0.0, 0.0,
        1.0, 0.0, 0.0,
        0.0, 1.0, 0.0,
    };

    int index[] = {0, 1, 2};

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
        if (color >= 1.0f)
        {
            color = 0.0f;
        }

        // Handle other events
        glfwPollEvents();
        handleKeyboardInput(window);

        // Flip buffers
        glfwSwapBuffers(window);
    }

}

void drawTrheeOverlappingTriangle(GLFWwindow *window)
{
    //3 triangles coordinates to be changed if you want to draw differents triangles.
    float coordinates[] =
    {
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
    int index[] = {0, 1, 2, 3, 4, 5, 6, 7, 8};

    float RGBAcolor[] = {0.5, 1.0, 0.0, 0.3,
                         0.5, 1.0, 0.0, 0.3,
                         0.5, 1.0, 0.0, 0.3,
                         0.5, 0.5, 1.0, 0.5,
                         0.5, 0.5, 1.0, 0.5,
                         0.5, 0.5, 1.0, 0.5,
                         1.0, 0.5, 0.5, 0.8,
                         1.0, 0.5, 0.5, 0.8,
                         1.0, 0.5, 0.5, 0.8
                        };

    int number_of_triangles = 3;

    // Set up the Vertex Array Objects to draw 3 triangles
    unsigned int vaoID = setUpVAOWithColor(coordinates, index, sizeof(coordinates), sizeof(index), 3, RGBAcolor, sizeof(RGBAcolor));

    // Effective draw of the 3 triangles
    draw(window, vaoID, number_of_triangles * 3, 0);
}

void drawTransformation(GLFWwindow *window, int uniformLocation)
{
    //3 triangles coordinates to be changed if you want to draw differents triangles.
    float coordinates[] =
    {
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
    int index[] = {6, 7, 8, 3, 4, 5, 0, 1, 2};

    float RGBAcolor[] = {0.5, 1.0, 0.0, 1.0,
                         0.5, 1.0, 0.0, 1.0,
                         0.5, 1.0, 0.0, 1.0,
                         0.5, 0.5, 1.0, 1.0,
                         0.5, 0.5, 1.0, 1.0,
                         0.5, 0.5, 1.0, 1.0,
                         1.0, 0.5, 0.5, 1.0,
                         1.0, 0.5, 0.5, 1.0,
                         1.0, 0.5, 0.5, 1.0
                        };

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

        if (i >= 0.5f)
        {
            isOpposite = 1;
        };

        if (i < 0.0f)
        {
            isOpposite = 0;
        }

        if(isOpposite)
        {
            i = i - 0.01f;
        }
        else
        {
            i = i + 0.01f;
        }

        // Modify the uniform value
        glUniformMatrix4fv(uniformLocation, 1, 0, matrix);

        // Handle other events
        glfwPollEvents();
        handleKeyboardInput(window);

        // Flip buffers
        glfwSwapBuffers(window);
    }
}

void cameraMovement(GLFWwindow *window, int uniformLocation, float *motion)
{
    // Start with and identity matrix
    glm::mat4x4 matrix = glm::mat4();

    // Build the perspective matrix
    glm::mat4x4 matrixPerspective = glm::perspective(glm::pi<float>() * 0.5f, float(windowWidth/windowHeight), 1.0f, 150.0f);

    // Build the view matrix
    glm::vec3 TVector = glm::vec3(motion[0], motion[1], motion[2]);
    glm::mat4x4 TMatrix = glm::translate(matrix, TVector);
    glm::mat4x4 RXMatrix = glm::rotate(matrix, motion[3], glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4x4 RYMatrix = glm::rotate(matrix, motion[4], glm::vec3(0.0f, 1.0f, 0.0f));

    // Compute the final matrix
    matrix = matrixPerspective * RXMatrix * RYMatrix * TMatrix;

    glUniformMatrix4fv(uniformLocation, 1, 0, glm::value_ptr(matrix));

    // Handle other events
    glfwPollEvents();
    handleKeyboardInputMotion(window, motion);
}

void camera(GLFWwindow *window, int uniformLocation)
{

    //3 triangles coordinates to be changed if you want to draw differents triangles.
    float coordinates[] =
    {
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
    int index[] = {6, 7, 8, 3, 4, 5, 0, 1, 2};

    float RGBAcolor[] = {0.5, 1.0, 0.0, 1.0,
                         0.5, 1.0, 0.0, 1.0,
                         0.5, 1.0, 0.0, 1.0,
                         0.5, 0.5, 1.0, 1.0,
                         0.5, 0.5, 1.0, 1.0,
                         0.5, 0.5, 1.0, 1.0,
                         1.0, 0.5, 0.5, 1.0,
                         1.0, 0.5, 0.5, 1.0,
                         1.0, 0.5, 0.5, 1.0
                        };

    int number_of_triangles = 3;

    MinecraftCharacter steve = loadMinecraftCharacterModel("../gloom/res/steve.obj");

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

        cameraMovement(window, uniformLocation, motion);

        // Flip buffers
        glfwSwapBuffers(window);
    }
}

void drawSteve(GLFWwindow *window, int uniformLocation)
{

    // Load the minecraft object TODO ask where is the correct place for handout
    MinecraftCharacter steve = loadMinecraftCharacterModel("../gloom/res/steve.obj");

    Mesh terrain = generateChessboard(5, 5, 15.0f, float4(1.0f, 0.0f, 0.0f, 1.0f), float4(0.0f, 1.0f, 0.0f, 1.0f));

    unsigned int leftLegID = setUpVAOWithColorFloat4(steve.leftLeg.vertices, &steve.leftLeg.indices[0], steve.leftLeg.vertices.size() * 4, steve.leftLeg.indices.size(), 4, steve.leftLeg.colours, steve.leftLeg.colours.size() * 4);
    unsigned int leftArmID = setUpVAOWithColorFloat4(steve.leftArm.vertices, &steve.leftArm.indices[0], steve.leftArm.vertices.size() * 4, steve.leftArm.indices.size(), 4, steve.leftArm.colours, steve.leftArm.colours.size() * 4);
    unsigned int rightLegID = setUpVAOWithColorFloat4(steve.rightLeg.vertices, &steve.rightLeg.indices[0], steve.rightLeg.vertices.size() * 4, steve.rightLeg.indices.size(), 4, steve.rightLeg.colours, steve.rightLeg.colours.size() * 4);
    unsigned int rightArmID = setUpVAOWithColorFloat4(steve.rightArm.vertices, &steve.rightArm.indices[0], steve.rightArm.vertices.size() * 4, steve.rightArm.indices.size(), 4, steve.rightArm.colours, steve.rightArm.colours.size() * 4);
    unsigned int torsoID = setUpVAOWithColorFloat4(steve.torso.vertices, &steve.torso.indices[0], steve.torso.vertices.size() * 4, steve.torso.indices.size(), 4, steve.torso.colours, steve.torso.colours.size() * 4);
    unsigned int headID = setUpVAOWithColorFloat4(steve.head.vertices, &steve.head.indices[0], steve.head.vertices.size() * 4, steve.head.indices.size(), 4, steve.head.colours, steve.head.colours.size() * 4);
    unsigned int terrainID = setUpVAOWithColorFloat4(terrain.vertices, &terrain.indices[0], terrain.vertices.size() * 4, terrain.indices.size(), 4, terrain.colours, terrain.colours.size() * 4);
    // x, y, z, x angle, y angle;
    float motion[7] = {0.0f, -3.0f, -32.0f, 0.0f, 0.0f};

    while (!glfwWindowShouldClose(window))
    {
        // Clear colour and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Bind the current Vertex Array Object
        glBindVertexArray(leftLegID);
        // Draw the current Vertex Array Object using mode GL_TRIANGLES
        glDrawElements(GL_TRIANGLES, steve.leftLeg.vertices.size() * 4, GL_UNSIGNED_INT, 0);

        glBindVertexArray(leftArmID);
        glDrawElements(GL_TRIANGLES, steve.leftArm.vertices.size() * 4, GL_UNSIGNED_INT, 0);

        glBindVertexArray(rightLegID);
        glDrawElements(GL_TRIANGLES, steve.rightLeg.vertices.size() * 4, GL_UNSIGNED_INT, 0);

        glBindVertexArray(rightArmID);
        glDrawElements(GL_TRIANGLES, steve.rightArm.vertices.size() * 4, GL_UNSIGNED_INT, 0);

        glBindVertexArray(torsoID);
        glDrawElements(GL_TRIANGLES, steve.torso.vertices.size() * 4, GL_UNSIGNED_INT, 0);

        glBindVertexArray(headID);
        glDrawElements(GL_TRIANGLES, steve.head.vertices.size() * 4, GL_UNSIGNED_INT, 0);

        glBindVertexArray(terrainID);
        glDrawElements(GL_TRIANGLES, terrain.vertices.size() * 4, GL_UNSIGNED_INT, 0);

        cameraMovement(window, uniformLocation, motion);

        // Flip buffers
        glfwSwapBuffers(window);
    }
}

void handleKeyboardInputMotion(GLFWwindow *window, float *motion)
{
    // Use escape key for terminating the GLFW window
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    //right
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        motion[0] = motion[0] - 0.1;
    }

    //left
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        motion[0] = motion[0] + 0.1;
    }

    // Up
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        motion[1] = motion[1] - 0.1;
    }

    // Down
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        motion[1] = motion[1] + 0.1;
    }

    // Forward
    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        motion[2] = motion[2] + 0.1;
    }

    // Backward
    if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        motion[2] = motion[2] - 0.1;
    }

    // Rotate up
    if(glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
    {
        motion[3] = motion[3] - 0.01f;
    }

    // Rotate down
    if(glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
    {
        motion[3] = motion[3] + 0.01f;
    }

    // Rotate right
    if(glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
    {
        motion[4] = motion[4] + 0.01f;

    }

    // Rotate left
    if(glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
    {
        motion[4] = motion[4] - 0.01f;

    }
}

void handleKeyboardInput(GLFWwindow *window)
{
    // Use escape key for terminating the GLFW window
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

SceneNode *constructSceneGraph(MinecraftCharacter &steve, Mesh &terrain, float3 initialPosition)
{
    // Generate one SceneNode for each object
    SceneNode *rootNode = createSceneNode();
    SceneNode *leftLegNode = createSceneNode();
    SceneNode *leftArmNode = createSceneNode();
    SceneNode *rightLegNode = createSceneNode();
    SceneNode *rightArmNode = createSceneNode();
    SceneNode *torsoNode = createSceneNode();
    SceneNode *headNode = createSceneNode();
    SceneNode *terrainNode = createSceneNode();

    // Assign a name to all the node
    torsoNode->name = "torso";
    headNode->name = "head";
    leftLegNode->name = "leftLeg";
    leftArmNode->name = "leftArm";
    rightLegNode->name = "rightLeg";
    rightArmNode->name = "rightArm";
    terrainNode->name = "terrain";
    rootNode->name = "root";

    // Organise the objects into a Scene Graph
    addChild(torsoNode, headNode);
    addChild(torsoNode, leftArmNode);
    addChild(torsoNode, leftLegNode);
    addChild(torsoNode, rightArmNode);
    addChild(torsoNode, rightLegNode);
    addChild(rootNode, torsoNode);
    addChild(rootNode, terrainNode);

    // Initialise the values in the SceneNode data structure
    torsoNode->vertexArrayObjectID = setUpVAOWithColorFloat4(steve.torso.vertices, &steve.torso.indices[0], steve.torso.vertices.size() * 4, steve.torso.indices.size(), 4, steve.torso.colours, steve.torso.colours.size() * 4);
    leftLegNode->vertexArrayObjectID = setUpVAOWithColorFloat4(steve.leftLeg.vertices, &steve.leftLeg.indices[0], steve.leftLeg.vertices.size() * 4, steve.leftLeg.indices.size(), 4, steve.leftLeg.colours, steve.leftLeg.colours.size() * 4);
    leftArmNode->vertexArrayObjectID = setUpVAOWithColorFloat4(steve.leftArm.vertices, &steve.leftArm.indices[0], steve.leftArm.vertices.size() * 4, steve.leftArm.indices.size(), 4, steve.leftArm.colours, steve.leftArm.colours.size() * 4);
    rightLegNode->vertexArrayObjectID = setUpVAOWithColorFloat4(steve.rightLeg.vertices, &steve.rightLeg.indices[0], steve.rightLeg.vertices.size() * 4, steve.rightLeg.indices.size(), 4, steve.rightLeg.colours, steve.rightLeg.colours.size() * 4);
    rightArmNode->vertexArrayObjectID = setUpVAOWithColorFloat4(steve.rightArm.vertices, &steve.rightArm.indices[0], steve.rightArm.vertices.size() * 4, steve.rightArm.indices.size(), 4, steve.rightArm.colours, steve.rightArm.colours.size() * 4);
    headNode->vertexArrayObjectID = setUpVAOWithColorFloat4(steve.head.vertices, &steve.head.indices[0], steve.head.vertices.size() * 4, steve.head.indices.size(), 4, steve.head.colours, steve.head.colours.size() * 4);
    terrainNode->vertexArrayObjectID = setUpVAOWithColorFloat4(terrain.vertices, &terrain.indices[0], terrain.vertices.size() * 4, terrain.indices.size(), 4, terrain.colours, terrain.colours.size() * 4);

    torsoNode->VAOIndexCount = steve.torso.indices.size();
    headNode->VAOIndexCount = steve.head.indices.size();
    leftLegNode->VAOIndexCount = steve.leftLeg.indices.size();
    leftArmNode->VAOIndexCount = steve.leftArm.indices.size();
    rightLegNode->VAOIndexCount = steve.rightLeg.indices.size();
    rightArmNode->VAOIndexCount = steve.rightArm.indices.size();
    terrainNode->VAOIndexCount = terrain.indices.size();

    torsoNode->position = initialPosition;

    // Set the reference point of each object
    leftLegNode->referencePoint = float3(-2, 12, 0) ;
    rightLegNode->referencePoint = float3(2, 12, 0) ;
    leftArmNode->referencePoint = float3(-4, 22, 0) ;
    rightArmNode->referencePoint = float3(4, 22, 0) ;
    headNode->referencePoint = float3(0, 24, 0) ;
    torsoNode->referencePoint = float3(0, 12, 0) + initialPosition ;
    terrainNode->referencePoint = float3(0, 0, 0) ;

    printScene(rootNode);
    // Return the root node
    return rootNode;
}

void printScene(SceneNode *rootNode)
{
    printNode(rootNode);
    for(unsigned int i = 0; i < rootNode->children.size(); i++)
    {
        printScene(rootNode->children.at(i));
    }
}

void visitSceneNode(SceneNode *node, glm::mat4 transformationThusFar, float rotation, float2 movement, float angle, std::stack<glm::mat4> *stack)
{
    // Update the position, rotation and reference point of the interested node
    if(node->name == "torso")
    {
        node->position.x += movement.x;
        node->position.z += movement.y;
        node->referencePoint.x += movement.x;
        node->referencePoint.z += movement.y;
        node->rotation.y = angle;
    }

    if((node->name == "leftArm") || (node->name == "rightLeg"))
    {
        node->rotation.x += rotation;
    }

    if(node->name == "rightArm" || node->name == "leftLeg")
    {
        node->rotation.x -= rotation;
    }

    // Start with the identity matrix
    glm::mat4 matrix = glm::mat4();

    // Compute the translation matrix of the node
    glm::mat4x4 TMatrix = glm::translate(matrix, glm::vec3(node->position.x, node->position.y, node->position.z));

    // Compute the matrices needed to rotate around the reference point of the node
    glm::mat4x4 TReferenceMatrix = glm::translate(matrix, glm::vec3(node->referencePoint.x, node->referencePoint.y, node->referencePoint.z));
    glm::mat4x4 inverseTReferenceMatrix = glm::translate(matrix, glm::vec3( - node->referencePoint.x, - node->referencePoint.y, - node->referencePoint.z));
    glm::mat4x4 RXMatrix = glm::rotate(matrix, node->rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4x4 RYMatrix = glm::rotate(matrix, node->rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4x4 RZMatrix = glm::rotate(matrix, node->rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

    // Compute the model matrix of the node
    node->currentTransformationMatrix = TReferenceMatrix * RXMatrix * RYMatrix * RZMatrix * inverseTReferenceMatrix * TMatrix;

    // Multiply the model matrix of the current node for the model matrix of the parent node
    node->currentTransformationMatrix =  transformationThusFar * node->currentTransformationMatrix;

    // Push the model matrix on the stack
    pushMatrix(stack, node->currentTransformationMatrix);

    // visit all the children node
    for(SceneNode *child : node->children)
    {
        visitSceneNode(child, node->currentTransformationMatrix, rotation, movement, angle, stack);
    }

    // pop the model matrix of the current node from the stack
    popMatrix(stack);
}

void drawSceneNode(SceneNode *node, float *motion, int uniformLocation)
{
    if(node->name != "root")
    {
        // Start with and identity matrix
        glm::mat4x4 matrix = glm::mat4x4();

        // Build the perspective matrx
        glm::mat4x4 matrixPerspective = glm::perspective(glm::pi<float>() * 0.5f, float(windowHeight / windowWidth), 1.0f, 150.0f);

        // Build the view matrix
        glm::vec3 TVector = glm::vec3(motion[0], motion[1], motion[2]);
        glm::mat4x4 T1Matrix = glm::translate(matrix, TVector);
        glm::mat4x4 RX1Matrix = glm::rotate(matrix, motion[3], glm::vec3(1.0f, 0.0f, 0.0f));
        glm::mat4x4 RY1Matrix = glm::rotate(matrix, motion[4], glm::vec3(0.0f, 1.0f, 0.0f));

        // Compute the MVP matrix
        matrix = matrixPerspective * RX1Matrix * RY1Matrix * T1Matrix * node->currentTransformationMatrix;

        // Update the uniform variable in the vertex shader
        glUniformMatrix4fv(uniformLocation, 1, 0, glm::value_ptr(matrix));

        // Draw the current node
        glBindVertexArray(node->vertexArrayObjectID);
        glDrawElements(GL_TRIANGLES, node->VAOIndexCount, GL_UNSIGNED_INT, 0);
    }

    for(SceneNode *child : node->children)
    {
        drawSceneNode(child, motion, uniformLocation);
    }
}

void computeWalkingParameter(float Dx, float Dy, float2 &movement, float &rotation, float2 &currentWaypoint, float &increment)
{
    
    movement.x = (Dx != 0) ? (0.1 * (Dx / abs(Dx))) : 0;
    movement.y = (Dy != 0) ? (0.1 * (Dy / abs(Dy))) : 0;

    currentWaypoint += movement;

    rotation -= increment;

    increment = (rotation >= 0.5 || rotation <= -0.5) ? (increment * (-1)) : increment;

}

void computeAngleNextWaypoint(float dx, float dy, float &angle)
{
    angle = atan2(abs(dy), abs(dx));
    if(dx != 0)
    {
        angle = (dy <= 0 ) ? ((PI / 2 + angle) * (dx / abs(dx))) : (angle * (dx / abs(dx)));
    }
    else
    {
        angle = (dy < 0) ? (angle + PI / 2) : 0;
    }
}

void drawScene(GLFWwindow *window, int uniformLocation)
{
    // Load the minecraft characters
    MinecraftCharacter steve = loadMinecraftCharacterModel("../gloom/res/steve.obj");

    // Create the terrain upon what the character walk
    float tileWidth = 15.0f;
    int terrain_width = 5;
    int terrain_height = 5;
    float4 color1 = float4(1.0f, 0.0f, 0.0f, 1.0f);
    float4 color2 = float4(0.0f, 1.0f, 0.0f, 1.0f);
    Mesh terrain = generateChessboard(terrain_width, terrain_height, tileWidth, color1, color2);

    // Load the path that the character will follow and get the next way point
    Path path("coordinates_0.txt");
    float2 currentWaypoint = path.getCurrentWaypoint(tileWidth);
    path.advanceToNextWaypoint();
    float2 nextWaypoint = path.getCurrentWaypoint(tileWidth);
    float2 movement;

    // Construct the scene graph
    float3 initialPosition = float3(currentWaypoint.x, 0.0f, currentWaypoint.y);
    SceneNode *rootNode = constructSceneGraph(steve, terrain, initialPosition);

    // Create the stack for the transform matrices
    std::stack<glm::mat4> *stack = createEmptyMatrixStack();

    // Initialize the stack matrices with the identity matrix of the rootNode
    rootNode->currentTransformationMatrix = glm::mat4();
    pushMatrix(stack, rootNode->currentTransformationMatrix);

    // x, y, z, x angle, y angle, for the camera movement;
    float motion[7] = {-80.0f, -20.0f, -85.0f, 0.20f, -0.80f};

    double total = 0.0;
    double treshold = 0.005;
    float rotation = 0.0f;
    float increment = 0.01f;
    float angle = 0.0f;
    short stop = 0;
    short count = 0;
    float dx = nextWaypoint.x - currentWaypoint.x;
    float dy = nextWaypoint.y - currentWaypoint.y;

    computeAngleNextWaypoint(dx, dy, angle);

    while (!glfwWindowShouldClose(window))
    {
        // Clear colour and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        total += getTimeDeltaSeconds();
        if(total >= treshold)
        {

            if(path.hasWaypointBeenReached(currentWaypoint, tileWidth))
            {
                currentWaypoint = nextWaypoint;
                path.advanceToNextWaypoint();
                nextWaypoint = path.getCurrentWaypoint(tileWidth);

                dx = nextWaypoint.x - currentWaypoint.x;
                dy = nextWaypoint.y - currentWaypoint.y;
                computeAngleNextWaypoint(dx, dy, angle);
                //     stop = 1;
                //     increment = 0;
                //     movement = 0;
                // }
                // else if(stop)
                // {
                //     count ++;
                //     if (count == 50){
                //         stop = 0;
                //         increment = 0.01;
                //         count = 0;
                //     }
            }
            else
            {
                dx = nextWaypoint.x - currentWaypoint.x;
                dy = nextWaypoint.y - currentWaypoint.y;
                computeWalkingParameter(dx, dy, movement, rotation, currentWaypoint, increment);
            }

        }

        // Visit the scene graph and compute the transformation matrix of each node
        visitSceneNode(rootNode, rootNode->currentTransformationMatrix, increment, movement, angle, stack);

        // Visit the scene graph and draw each node
        drawSceneNode(rootNode, motion, uniformLocation);

        // Handle other events
        glfwPollEvents();
        handleKeyboardInputMotion(window, motion);

        // Flip buffers
        glfwSwapBuffers(window);
    }
}

