//
//  W_Road.cpp
//  week11
//
//  Created by Lucas Furlani on 11/20/2013.
//
//

#include "W_Road.h"
#include <stdio.h>
namespace wolf {
    
    struct Vertex
    {
        GLfloat x,y,z;
        GLfloat u,v;
        GLfloat nx, ny, nz;
    };
    
    static const Vertex cubeVertices[] = {
        // Bottom
        { -0.5f, -0.5f, 0.5f, 0, 0, 0, -1, 0 },
        {  0.5f, -0.5f, 0.5f, 1, 0, 0, -1, 0 },
        {  0.5f, -0.5f,-0.5f, 1, 1, 0, -1, 0 },
        {  0.5f, -0.5f,-0.5f, 1, 1, 0, -1, 0 },
        { -0.5f, -0.5f,-0.5f, 0, 1, 0, -1, 0 },
        { -0.5f, -0.5f, 0.5f, 0, 0, 0, -1, 0 },
    };
    //static GLuint tex;
    #define ROAD_TEXTURE_SIZE 3
    static wolf::Texture* roadTexture[ROAD_TEXTURE_SIZE];
    //----------------------------------------------------------
    // Constructor
    //----------------------------------------------------------
    //              qpVB                qpDecl              q_pProgram
    Road::Road() : vertexBuffer(0), vertexDeclaration(0), scaleMatrix(0), projectionMatrix(0), viewMatrix(0)
    {
        
        
        vertexBuffer = wolf::BufferManager::CreateVertexBuffer(cubeVertices, sizeof(Vertex) * 1 * 3 * 2);
        
        vertexDeclaration = new wolf::VertexDeclaration();
        vertexDeclaration->Begin();
        vertexDeclaration->AppendAttribute(wolf::AT_Position, 3, wolf::CT_Float);
        vertexDeclaration->AppendAttribute(wolf::AT_TexCoord1, 2, wolf::CT_Float);
        vertexDeclaration->AppendAttribute(wolf::AT_Normal, 3, wolf::CT_Float);
        vertexDeclaration->SetVertexBuffer(vertexBuffer);
        vertexDeclaration->End();
        
        for (int i = 0; i < ROAD_TEXTURE_SIZE; i++) {
            char buffer [50];
            char char_texture = i + '0';
            sprintf (buffer, "data/week11/textures/road/%c.tga", char_texture);
            roadTexture[i] = wolf::TextureManager::CreateTexture(buffer);
        }
        
        
        // Initialize matrices values
        translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
        scaleMatrix       = glm::scale(1.0f, 1.0f, 1.0f);
        rotationMatrix    = glm::rotate(0.0f, 1.0f, 0.0f, 0.0f);
    }
    
    void Road::SetPosition(const glm::vec3& position) {
        translationMatrix = glm::translate(glm::mat4(1.0f), position);;
    }
    
    glm::mat4 Road::GetPosition() {
        return translationMatrix;
    }
    
    void Road::SetScale(float x, float y, float z) {
        scaleMatrix = glm::scale(x, y, z);
    }
    
    void Road::SetRotation(float angle, float x, float y, float z) {
        rotationMatrix = glm::rotate(angle, x, y, z);
    }
    
    void Road::SetTexture(short _texture) {
        texture = _texture;
    }
    
    void Road::SetType(short _type) {
        type = _type;
    }
    
    void Road::Render(wolf::Program* program, glm::mat4* _projectionMatrix, glm::mat4* _viewMatrix, glm::mat4 _worldMatrix, float _sun[2])
    {
        projectionMatrix = _projectionMatrix;
        viewMatrix = _viewMatrix;
        worldMatrix = translationMatrix * scaleMatrix * rotationMatrix;
        
        glm::mat3 mWorldIT(worldMatrix);
        mWorldIT = glm::inverse(mWorldIT);
        mWorldIT = glm::transpose(mWorldIT);
        
        
        // Use shader program.
        program->Bind();
        
        // Bind texture
        roadTexture[texture]->Bind();
        
        // Bind Uniforms
        program->SetUniform("projection", *projectionMatrix);
        program->SetUniform("view", *viewMatrix);
        program->SetUniform("world", worldMatrix);
        program->SetUniform("WorldIT", mWorldIT);
        
        program->SetUniform("texture1", 0);
        program->SetUniform("LightDir", -glm::vec3(_sun[0], _sun[1], 0.0f));
        program->SetUniform("LightColor", glm::vec4(0.7f, 0.7f, 0.7f, 1.0f));
        program->SetUniform("AmbientLight", glm::vec4(0.3f, 0.3f, 0.3f, 1.0f));
        // Set up source data
        vertexDeclaration->Bind();
        
        // Draw!
        glDrawArrays(GL_TRIANGLES, 0, 1 * 3 * 2);
    }
}