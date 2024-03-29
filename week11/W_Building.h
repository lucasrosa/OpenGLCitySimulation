//
//  W_Building.h
//  week11
//
//  Created by Lucas Furlani on 11/13/2013.
//
//

#ifndef __week11__W_Building__
#define __week11__W_Building__

#define GLFW_INCLUDE_GL3
#define GLFW_NO_GLU
#include <stdio.h>
#include <stdlib.h>
#ifndef __APPLE__
#include <GL/glew.h>
#endif

#include <iostream>
#include "W_VertexBuffer.h"
#include "W_IndexBuffer.h"
#include "W_Program.h"
#include "W_Common.h"
#include "W_BufferManager.h"
#include "W_VertexDeclaration.h"
#include "W_ProgramManager.h"
#include "W_TextureManager.h"

#include <string>
#include <map>
#include <stdio.h>

#include "W_Roof.h"

namespace wolf
{
    class Building
    {
	public:
		//-------------------------------------------------------------------------
		// PUBLIC INTERFACE
		//-------------------------------------------------------------------------
        //Building(glm::mat4* _projectionMatrix, glm::mat4* _viewMatrix);
        Building();
        // Draw the building in the screen
        void Render(wolf::Program* program, glm::mat4* _projectionMatrix, glm::mat4* _viewMatrix, glm::mat4 worldMatrix, float _sun[2]);
        // Building the building
        void Build (wolf::Program* program, glm::mat4* _projectionMatrix, glm::mat4* _viewMatrix, glm::mat4 _worldMatrix, const glm::vec3& _position, short _roofTexture, float _sun[2]);
		void SetPosition(const glm::vec3& position);
        glm::mat4 GetPosition();
        
        void SetScale(float x, float y, float z);
        void SetRotation(float angle, float x, float y, float z);
        void SetTexture(short _texture);
        void SetType(short _type);
        void SetNumberOfFloors(short _numberOfFloors);
        
		//-------------------------------------------------------------------------
        
	private:
		//-------------------------------------------------------------------------
		// PRIVATE TYPES
		//-------------------------------------------------------------------------
		//-------------------------------------------------------------------------
        
		//-------------------------------------------------------------------------
		// PRIVATE MEMBERS
		//-------------------------------------------------------------------------
		wolf::VertexBuffer*			  vertexBuffer; // qpVB
		wolf::VertexDeclaration* vertexDeclaration; //qpDecl
        
        // Uniforms
        glm::mat4* projectionMatrix;
        glm::mat4* viewMatrix;
        glm::mat4 worldMatrix;
        
        // World Matrix components
        glm::mat4 translationMatrix;
        glm::mat4 scaleMatrix;
        glm::mat4 rotationMatrix;
        
        
        // Building parameters
        short texture;
        short type;
        short numberOfFloors;
		//-------------------------------------------------------------------------
    };
    
}


#endif /* defined(__week5__D_Cube__) */
