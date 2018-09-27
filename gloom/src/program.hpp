#ifndef PROGRAM_HPP
#define PROGRAM_HPP
#pragma once


// System headers
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <string>


// Main OpenGL program
void runProgram(GLFWwindow* window);

unsigned int setUpVAO(float* coordinates, int* index, int cCount, int iCount, int number_of_dimension);

unsigned int setUpVAOWithColor(float* coordinates, int* index, int cCount, int iCount, int number_of_dimension, float* RGBAcolor, int colorCount);

// Implementatio of the effective draw
void draw(GLFWwindow* window, unsigned int vaoID, int number_of_triangles, int drawing_mode);

void drawFiveTriangles(GLFWwindow* window);

void drawSingleTriangle(GLFWwindow* window);

void drawCircle(GLFWwindow* window, float cx, float cy, float r);

void drawSpiral(GLFWwindow* window, float cx, float cy, float r, int times);

void drawChangingColorInTime(GLFWwindow* window, int uniformLocation);

void drawTrheeOverlappingTriangle(GLFWwindow* window);

void drawTransformation(GLFWwindow* window, int uniformLocation);

void camera(GLFWwindow* window, int uniformLocation);

// Function for handling keypresses
void handleKeyboardInput(GLFWwindow* window);

void handleKeyboardInputMotion(GLFWwindow* window, float* motion);


// Checks for whether an OpenGL error occurred. If one did,
// it prints out the error type and ID
inline void printGLError() {
    int errorID = glGetError();

    if(errorID != GL_NO_ERROR) {
        std::string errorString;

        switch(errorID) {
            case GL_INVALID_ENUM:
                errorString = "GL_INVALID_ENUM";
                break;
            case GL_INVALID_OPERATION:
                errorString = "GL_INVALID_OPERATION";
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                errorString = "GL_INVALID_FRAMEBUFFER_OPERATION";
                break;
            case GL_OUT_OF_MEMORY:
                errorString = "GL_OUT_OF_MEMORY";
                break;
            case GL_STACK_UNDERFLOW:
                errorString = "GL_STACK_UNDERFLOW";
                break;
            case GL_STACK_OVERFLOW:
                errorString = "GL_STACK_OVERFLOW";
                break;
            default:
                errorString = "[Unknown error ID]";
                break;
        }

        fprintf(stderr, "An OpenGL error occurred (%i): %s.\n",
                errorID, errorString.c_str());
    }
}


#endif
