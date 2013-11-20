//
//  W_Road.h
//  week11
//
//  Created by Lucas Furlani on 11/20/2013.
//
//

#ifndef __week11__W_Road__
#define __week11__W_Road__

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


namespace wolf
{
    class Road
    {
	public:
		//-------------------------------------------------------------------------
		// PUBLIC INTERFACE
		//-------------------------------------------------------------------------
        //Road(glm::mat4* _projectionMatrix, glm::mat4* _viewMatrix);
        Road();
        // Draw the ceiling in the screen
        void Render(wolf::Program* program, glm::mat4* _projectionMatrix, glm::mat4* _viewMatrix, glm::mat4 worldMatrix);
		void SetPosition(const glm::vec3& position);
        glm::mat4 GetPosition();
        
        void SetScale(float x, float y, float z);
        void SetRotation(float angle, float x, float y, float z);
        void SetTexture(short _texture);
        void SetType(short _type);
        
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
        
        // Road parameters
        short texture;
        short type;
		//-------------------------------------------------------------------------
        
    };
    
}

#endif /* defined(__week11__W_Road__) */
