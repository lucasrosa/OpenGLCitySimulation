//
//  W_Building.cpp
//  week11
//
//  Created by Lucas Furlani on 11/13/2013.
//
//

#include "W_Building.h"
#include <stdio.h>
namespace wolf {
    
    struct Vertex
    {
        GLfloat x,y,z;
        GLfloat u,v;
    };
    
    static const Vertex cubeVertices[] = {
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
        /*
        // Top
        { -0.5f,  0.5f, 0.5f, 0, 0 },
        { -0.5f,  0.5f,-0.5f, 0, 1 },
        {  0.5f,  0.5f,-0.5f, 1, 1 },
        {  0.5f,  0.5f,-0.5f, 1, 1 },
        {  0.5f,  0.5f, 0.5f, 1, 0 },
        { -0.5f,  0.5f, 0.5f, 0, 0 },
        
        // Bottom
        { -0.5f, -0.5f, 0.5f, 0, 0 },
        {  0.5f, -0.5f, 0.5f, 1, 0 },
        {  0.5f, -0.5f,-0.5f, 1, 1 },
        {  0.5f, -0.5f,-0.5f, 1, 1 },
        { -0.5f, -0.5f,-0.5f, 0, 1 },
        { -0.5f, -0.5f, 0.5f, 0, 0 },
         */
    };
    //static GLuint tex;
    static wolf::Texture* wallTexture[8];
    // Define the roof of the building
    static wolf::Roof* roof = 0;
    static int numberOfRoofTextures = 2;
    //----------------------------------------------------------
    // Constructor
    //----------------------------------------------------------
    //              qpVB                qpDecl              q_pProgram
    Building::Building() : vertexBuffer(0), vertexDeclaration(0), scaleMatrix(0), projectionMatrix(0), viewMatrix(0)
    {
        
        
        vertexBuffer = wolf::BufferManager::CreateVertexBuffer(cubeVertices, sizeof(Vertex) * 4 * 3 * 2);
        
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
        
        for (int i = 0; i < 8; i++) {
            //std::string s = string(itoa(i));
            //std::string textureName = std::string("data/week11/building/") + std::string(texture);
            char buffer [50];
            char char_texture = i + '0';
            sprintf (buffer, "data/week11/textures/building/%c.tga", char_texture);
            
            //wallTexture[i] = wolf::TextureManager::CreateTexture(sprintf("data/week11/building/%hd.tga", texture));
            wallTexture[i] = wolf::TextureManager::CreateTexture(buffer);
        }
        
        
        //g_pTexture->SetWrapMode(wolf::Texture::WM_Repeat, wolf::Texture::WM_Repeat);
        
        // Initialize matrices values
        translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
        scaleMatrix       = glm::scale(1.0f, 1.0f, 1.0f);
        rotationMatrix    = glm::rotate(0.0f, 1.0f, 0.0f, 0.0f);
        
        
        //--------------
        // Roof
        //--------------
        roof = new wolf::Roof();
        
    }
    
    void Building::SetPosition(const glm::vec3& position) {
        translationMatrix = glm::translate(glm::mat4(1.0f), position);;
    }
    
    glm::mat4 Building::GetPosition() {
        return translationMatrix;
    }
    
    void Building::SetScale(float x, float y, float z) {
        scaleMatrix = glm::scale(x, y, z);
    }
    
    void Building::SetRotation(float angle, float x, float y, float z) {
        rotationMatrix = glm::rotate(angle, x, y, z);
    }
    
    void Building::SetTexture(short _texture) {
        texture = _texture;
    }
    
    void Building::SetType(short _type) {
        type = _type;
    }
    
    void Building::SetNumberOfFloors(short _numberOfFloors){
        numberOfFloors = _numberOfFloors;
    }
    
    void Building::Build(wolf::Program* program, glm::mat4* _projectionMatrix, glm::mat4* _viewMatrix, glm::mat4 _worldMatrix, const glm::vec3& _position, short _roofTexture)
    {
        
        roof->SetTexture(_roofTexture);
        // Add the building
        for (float i = 0; i < numberOfFloors; i++) {
            this->SetPosition(glm::vec3(_position.x, i, _position.z));
            this->Render(program, _projectionMatrix, _viewMatrix, _worldMatrix);
        }
        roof->SetPosition(glm::vec3(_position.x, numberOfFloors, _position.z));
        roof->Render(program, _projectionMatrix, _viewMatrix, _worldMatrix);
        
        // Add a building to the south
        if (type == 1 || type == 3 || type == 4 ||  type == 6 || type == 7) {
            for (float i = 0; i < numberOfFloors; i++) {
                this->SetPosition(glm::vec3(_position.x, i, _position.z+1.0f));
                this->Render(program, _projectionMatrix, _viewMatrix, _worldMatrix);
            }
            
            roof->SetPosition(glm::vec3(_position.x, numberOfFloors, _position.z+1.0f));
            roof->Render(program, _projectionMatrix, _viewMatrix, _worldMatrix);
        }
        
        // Add a building to the east
        if (type == 2 || type == 4 || type == 5 || type == 7) {
            for (float i = 0; i < numberOfFloors; i++) {
                this->SetPosition(glm::vec3(_position.x+1.0f, i, _position.z));
                this->Render(program, _projectionMatrix, _viewMatrix, _worldMatrix);
            }
            
            roof->SetPosition(glm::vec3(_position.x+1.0f, numberOfFloors, _position.z));
            roof->Render(program, _projectionMatrix, _viewMatrix, _worldMatrix);
        }
        
        // Add a building to the southeast
        if (type == 5 || type == 7) {
            for (float i = 0; i < numberOfFloors; i++) {
                this->SetPosition(glm::vec3(_position.x+1.0f, i, _position.z+1.0f));
                this->Render(program, _projectionMatrix, _viewMatrix, _worldMatrix);
            }
            
            roof->SetPosition(glm::vec3(_position.x+1.0f, numberOfFloors, _position.z+1.0f));
            roof->Render(program, _projectionMatrix, _viewMatrix, _worldMatrix);
        }
        
        // Add a building to the southwest
        if (type == 6) {
            for (float i = 0; i < numberOfFloors; i++) {
                this->SetPosition(glm::vec3(_position.x-1.0f, i, _position.z+1.0f));
                this->Render(program, _projectionMatrix, _viewMatrix, _worldMatrix);
            }
            
            roof->SetPosition(glm::vec3(_position.x-1.0f, numberOfFloors, _position.z+1.0f));
            roof->Render(program, _projectionMatrix, _viewMatrix, _worldMatrix);
        }
    }
    void Building::Render(wolf::Program* program, glm::mat4* _projectionMatrix, glm::mat4* _viewMatrix, glm::mat4 _worldMatrix)
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
        glDrawArrays(GL_TRIANGLES, 0, 4 * 3 * 2);
    }
}