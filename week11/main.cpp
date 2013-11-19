//========================================================================
// Week 11 - lighting part 2
//========================================================================
#define GLFW_INCLUDE_GL3
#define GLFW_NO_GLU
#include <stdio.h>
#include <stdlib.h>
#ifndef __APPLE__
#include <GL/glew.h>
#endif
#include <GL/glfw.h>
#include "W_Common.h"
#include "examples.h"
#include <glm/glm.hpp>

int g_iExample = 0;

int main( void )
{
    int width, height, x;
    double t;
    
    // Initialise GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        exit( EXIT_FAILURE );
    }
    
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 2);
    glfwOpenWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    // Open a window and create its OpenGL context
    //if( !glfwOpenWindow( 1280, 720, 0,0,0,0, 24,8, GLFW_WINDOW ) )
    if( !glfwOpenWindow( 1280, 720, 0,0,0,0, 24,0, GLFW_WINDOW ) )
    {
        fprintf( stderr, "Failed to open GLFW window\n" );
        
        glfwTerminate();
        exit( EXIT_FAILURE );
    }
    
#ifndef __APPLE__
	glewInit();
#endif

    glfwSetWindowTitle( "Week 11" );
    
    // Ensure we can capture the escape key being pressed below
    glfwEnable( GLFW_STICKY_KEYS );
    
    // Enable vertical sync (on cards that support it)
    glfwSwapInterval( 1 );
    
	switch( g_iExample )
	{
        case 0: InitAssignment2(); break;
		case 1: InitExample1(); break;
		case 2: InitExample2(); break;
		case 3: InitExample3(); break;
		case 4: InitExample4(); break;
		case 5: InitExample5(); break;
		case 6: InitExample6(); break;
	}

    do
    {
        t = glfwGetTime();
        glfwGetMousePos( &x, NULL );
        
        // Get window size (may be different than the requested size)
        glfwGetWindowSize( &width, &height );
        
        // Special case: avoid division by zero below
        height = height > 0 ? height : 1;
        
        glViewport( 0, 0, width, height );
        
        // Clear color buffer to black
        //glClearColor( 0.3f, 0.5f, 0.8f, 0.0f );
        glClearColor( 0.4f, 0.4f, 0.4f, 0.0f );
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        
		switch( g_iExample )
		{
			case 0: RenderAssignment2(); break;
            case 1: RenderExample1(); break;
			case 2: RenderExample2(); break;
			case 3: RenderExample3(); break;
			case 4: RenderExample4(); break;
			case 5: RenderExample5(); break;
			case 6: RenderExample6(); break;
		}

        // Swap buffers
        glfwSwapBuffers();
        
    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey( GLFW_KEY_ESC ) != GLFW_PRESS &&
          glfwGetWindowParam( GLFW_OPENED ) );
    
    // Close OpenGL window and terminate GLFW
    glfwTerminate();
    
    exit( EXIT_SUCCESS );
}


