//
//  W_Roof.cpp
//  week11
//
//  Created by Lucas Furlani on 11/19/2013.
//
//

#include "W_Roof.h"
#include <stdio.h>
namespace wolf {
    
    struct Vertex
    {
        GLfloat x,y,z;
        GLfloat u,v;
    };
    
    static const Vertex cubeVertices[] = {
        /*
         // Front
         { -0.5f, -0.5f, 0.5f, 0, 0 },
         { -0.5f,  0.5f, 0.5f, 0, 1 },
         {  0.5f,  0.5f, 0.5f, 1, 1 },
         {  0.5f,  0.5f, 0.5f, 1, 1 },
         {  0.5f, -0.5f, 0.5f, 1, 0 },
         { -0.5f, -0.5f, 0.5f, 0, 0 },
         
         // Back
         {  0.5f,  0.5f,-0.5f, 1, 1 },
         { -0.5f,  0.5f,-0.5f, 0, 1 },
         { -0.5f, -0.5f,-0.5f, 0, 0 },
         { -0.5f, -0.5f,-0.5f, 0, 0 },
         {  0.5f, -0.5f,-0.5f, 1, 0 },
         {  0.5f,  0.5f,-0.5f, 1, 1 },
         
         // Left
         { -0.5f,  0.5f,-0.5f, 0, 1 },
         { -0.5f,  0.5f, 0.5f, 1, 1 },
         { -0.5f, -0.5f, 0.5f, 1, 0 },
         { -0.5f, -0.5f, 0.5f, 1, 0 },
         { -0.5f, -0.5f,-0.5f, 0, 0 },
         { -0.5f,  0.5f,-0.5f, 0, 1 },
         
         // Right
         {  0.5f,  0.5f, 0.5f, 0, 1 },
         {  0.5f,  0.5f,-0.5f, 1, 1 },
         {  0.5f, -0.5f,-0.5f, 1, 0 },
         {  0.5f, -0.5f,-0.5f, 1, 0 },
         {  0.5f, -0.5f, 0.5f, 0, 0 },
         {  0.5f,  0.5f, 0.5f, 0, 1 },
         
         // Top
         { -0.5f,  0.5f, 0.5f, 0, 0 },
         { -0.5f,  0.5f,-0.5f, 0, 1 },
         {  0.5f,  0.5f,-0.5f, 1, 1 },
         {  0.5f,  0.5f,-0.5f, 1, 1 },
         {  0.5f,  0.5f, 0.5f, 1, 0 },
         { -0.5f,  0.5f, 0.5f, 0, 0 },
         */
        // Bottom
        { -0.5f, -0.5f, 0.5f, 0, 0 },
        {  0.5f, -0.5f, 0.5f, 1, 0 },
        {  0.5f, -0.5f,-0.5f, 1, 1 },
        {  0.5f, -0.5f,-0.5f, 1, 1 },
        { -0.5f, -0.5f,-0.5f, 0, 1 },
        { -0.5f, -0.5f, 0.5f, 0, 0 },
    };
    //static GLuint tex;
    static wolf::Texture* wallTexture[8];
    //----------------------------------------------------------
    // Constructor
    //----------------------------------------------------------
    //              qpVB                qpDecl              q_pProgram
    Roof::Roof() : vertexBuffer(0), vertexDeclaration(0), scaleMatrix(0), projectionMatrix(0), viewMatrix(0)
    {
        
        
        vertexBuffer = wolf::BufferManager::CreateVertexBuffer(cubeVertices, sizeof(Vertex) * 1 * 3 * 2);
        
        vertexDeclaration = new wolf::VertexDeclaration();
        vertexDeclaration->Begin();
        vertexDeclaration->AppendAttribute(wolf::AT_Position, 3, wolf::CT_Float);
        //vertexDeclaration->AppendAttribute(wolf::AT_Color, 4, wolf::CT_UByte);
        vertexDeclaration->AppendAttribute(wolf::AT_TexCoord1, 2, wolf::CT_Float);
        vertexDeclaration->SetVertexBuffer(vertexBuffer);
        vertexDeclaration->End();
        
        /*
         // Handle texture
         glGenTextures(1, &tex);
         glBindTexture(GL_TEXTURE_2D, tex);
         
         GLFWimage img;
         glfwReadImage( "data/week11/building1.tga", &img, 0 );
         glTexImage2D(GL_TEXTURE_2D, 0, img.Format, img.Width, img.Height, 0, img.Format, GL_UNSIGNED_BYTE, img.Data);
         glfwFreeImage(&img);
         
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
         */
        //wolf::Texture* pTex = wolf::TextureManager::CreateTexture("data/week11/building1.tga");
        // TODO: MAKE AN ARRAY
        for (int i = 0; i < 1; i++) {
            //std::string s = string(itoa(i));
            //std::string textureName = std::string("data/week11/building/") + std::string(texture);
            char buffer [50];
            char char_texture = i + '0';
            sprintf (buffer, "data/week11/ceiling/%c.tga", char_texture);
            
            //wallTexture[i] = wolf::TextureManager::CreateTexture(sprintf("data/week11/building/%hd.tga", texture));
            wallTexture[i] = wolf::TextureManager::CreateTexture(buffer);
        }
        
        
        //g_pTexture->SetWrapMode(wolf::Texture::WM_Repeat, wolf::Texture::WM_Repeat);
        
        // Initialize matrices values
        translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
        scaleMatrix       = glm::scale(1.0f, 1.0f, 1.0f);
        rotationMatrix    = glm::rotate(0.0f, 1.0f, 0.0f, 0.0f);
    }
    
    void Roof::SetPosition(const glm::vec3& position) {
        translationMatrix = glm::translate(glm::mat4(1.0f), position);;
    }
    
    glm::mat4 Roof::GetPosition() {
        return translationMatrix;
    }
    
    void Roof::SetScale(float x, float y, float z) {
        scaleMatrix = glm::scale(x, y, z);
    }
    
    void Roof::SetRotation(float angle, float x, float y, float z) {
        rotationMatrix = glm::rotate(angle, x, y, z);
    }
    
    void Roof::SetTexture(short _texture) {
        texture = _texture;
    }
    
    void Roof::SetType(short _type) {
        type = _type;
    }
    
    void Roof::Render(wolf::Program* program, glm::mat4* _projectionMatrix, glm::mat4* _viewMatrix, glm::mat4 _worldMatrix)
    {
        projectionMatrix = _projectionMatrix;
        viewMatrix = _viewMatrix;
        worldMatrix = translationMatrix * scaleMatrix * rotationMatrix;
        
        
        // Use shader program.
        program->Bind();
        
        // Bind texture
        wallTexture[texture]->Bind();
        
        // Bind Uniforms
        program->SetUniform("projection", *projectionMatrix);
        program->SetUniform("view", *viewMatrix);
        program->SetUniform("world", worldMatrix);
        program->SetUniform("texture1", 0);
        // Set up source data
        vertexDeclaration->Bind();
        
        // Draw!
        glDrawArrays(GL_TRIANGLES, 0, 1 * 3 * 2);
    }
}