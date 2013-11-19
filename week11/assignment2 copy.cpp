//========================================================================
// Week 10 - Lighting Part 1
//
// Example 4 - Per-pixel specular
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
#include "W_BufferManager.h"
#include "W_VertexDeclaration.h"
#include "W_ProgramManager.h"
#include "W_TextureManager.h"
#include "PVRTModelPOD.h"
#include "W_MaterialManager.h"

#include "W_Building.h"
#include "W_Model.h"

// Define the Building
static wolf::Building* building = 0;

// Define the programs
static wolf::Program* program; //q_pProgram;

// Define the general matrices
static glm::mat4 projectionMatrix = glm::perspective(45.0f, 640.0f / 480.0f, 0.1f, 1000.0f);
static glm::mat4 viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,1.0f,0.0f));

// Window size
static float windowWidth  = 1280.0f;                    // Width of our window
static float windowHeight = 800.0f;                    // Heightof our window
static GLint midWindowX = windowWidth  / 2;         // Middle of the window horizontally
static GLint midWindowY = windowHeight / 2;         // Middle of the window vertically

static wolf::VertexBuffer* g_pVB = 0;
static wolf::IndexBuffer* g_pIB = 0;
static wolf::VertexDeclaration* g_pDecl = 0;
static wolf::Material* g_pMat = 0;
static bool g_bDepthOn = true;
static bool g_bDepthKeyDown = false;

static wolf::Model* g_pModel;

void InitAssignment2()
{

    glEnable(GL_DEPTH_TEST);
    // Hide the mouse cursor
    glfwDisable(GLFW_MOUSE_CURSOR);
    // Set the mouse cursor to the centre of our window
    glfwSetMousePos(midWindowX, midWindowY);
    /*
    CPVRTModelPOD pod;
	pod.ReadFromFile("data/week9/test.pod");
    
    SPODMesh* pMesh = &pod.pMesh[0];
    g_pVB = wolf::BufferManager::CreateVertexBuffer(pMesh->pInterleaved, pMesh->nNumVertex * pMesh->sVertex.nStride);
    g_pIB = wolf::BufferManager::CreateIndexBuffer(pMesh->nNumFaces*3);
    g_pIB->Write(pMesh->sFaces.pData);
    
    g_pDecl = new wolf::VertexDeclaration;
    g_pDecl->Begin();
    g_pDecl->AppendAttribute(wolf::AT_Position, pMesh->sVertex.n, wolf::CT_Float);
    g_pDecl->AppendAttribute(wolf::AT_Normal, pMesh->sNormals.n, wolf::CT_Float);
    g_pDecl->AppendAttribute(wolf::AT_TexCoord1, pMesh->psUVW[0].n, wolf::CT_Float);
    g_pDecl->SetVertexBuffer(g_pVB);
    g_pDecl->SetIndexBuffer(g_pIB);
    g_pDecl->End();
    
    g_pMat = wolf::MaterialManager::CreateMaterial("myMat");
    g_pMat->SetProgram("data/week11/assignment2.vsh", "data/week11/assignment2.fsh");
	wolf::Texture* pTex = wolf::TextureManager::CreateTexture("data/week10/MaskMain.tga");
	pTex->SetWrapMode(wolf::Texture::WM_Repeat);
    */
    // Instantiate the programs
    //program       = wolf::ProgramManager::CreateProgram("data/week11/polyhedron.vsh", "data/week11/polyhedron.fsh");
    program         = wolf::ProgramManager::CreateProgram("data/week11/cube.vsh", "data/week11/cube.fsh");
    // Instantiate the Cube
    building = new wolf::Building();
    
    
    //
    //g_pModel = new wolf::Model("data/week9/building/building.pod","data/week9/building/");
    //g_pModel = new wolf::Model("data/week11/Build11/Build1.pod","data/week11/Build11/");
    //g_pModel = new wolf::Model("data/week11/house2.pod","data/week11/house2/");
    //g_pModel = new wolf::Model("data/week11/barclay/model.pod","data/week11/barclay/");
    //g_pModel = new wolf::Model("data/week11/ChryslerBuilding/model.pod","data/week11/ChryslerBuilding/");
}

void RenderAssignment2()
{
    /*
    static float s_fRot = 0;
	glm::mat4 projectionMatrix = glm::perspective(45.0f, 1280.0f / 720.0f, 0.1f, 1000.0f);
	glm::mat4 viewMatrix = glm::lookAt(glm::vec3(0.0f,30.0f,60.0f), glm::vec3(0.0f,30.0f,0.0f), glm::vec3(0.0f,1.0f,0.0f));
	glm::mat4 worldMatrix = glm::rotate(s_fRot, 0.0f, 1.0f, 0.0f);
	s_fRot += 1.0f;
    
	// Bind Uniforms
	glm::mat4 mWVP = projectionMatrix * viewMatrix * worldMatrix;
	glm::mat3 worldMatrixIT(worldMatrix);
	worldMatrixIT = glm::inverse(worldMatrixIT);
	worldMatrixIT = glm::transpose(worldMatrixIT);
    
    g_pMat->SetUniform("WorldViewProj", mWVP);
	g_pMat->SetUniform("WorldIT", worldMatrixIT);
    g_pMat->SetUniform("Light1Dir", -glm::vec3(0.0f,0.0f,-1.0f));
    g_pMat->SetUniform("Light1Color", glm::vec4(0.6f,0.2f,0.2f,1.0f));
    g_pMat->SetUniform("ViewDir", glm::vec3(0.0f,0.0f,1.0f));
	g_pMat->SetUniform("SpecularColor", glm::vec3(1.0f,1.0f,1.0f));
	g_pMat->SetUniform("Shininess", 200.0f);
    g_pMat->SetUniform("AmbientColor", glm::vec4(0.5,0.5,0.5,1));
    
	// Use shader program.
	g_pMat->Apply();
    
    
	// Set up source data
	g_pDecl->Bind();
    
    // Draw!
	glDrawElements(GL_TRIANGLES, g_pIB->GetNumIndices(), GL_UNSIGNED_SHORT, 0);
    
    g_pModel->Render(worldMatrix,viewMatrix,projectionMatrix);
    */
    
    static float s_fRot = 0.0f;
    static glm::vec3 cameraPosition(0.0f, 2.0f, 5.0f);
    //static glm::vec3 cameraPosition(0.0f, 0.0f, 500.0f);
    //static glm::vec3 cameraPosition(0.0f, 2.0f, 100.0f);

    

    
    /*
    if( glfwGetKey( GLFW_KEY_DOWN ) == GLFW_PRESS )
		cameraPosition.z += 0.1f;
	else if( glfwGetKey( GLFW_KEY_UP ) == GLFW_PRESS )
		cameraPosition.z -= 0.1f;
    if( glfwGetKey( GLFW_KEY_LEFT ) == GLFW_PRESS )
		cameraPosition.x -= 0.1f;
    if( glfwGetKey( GLFW_KEY_RIGHT ) == GLFW_PRESS )
		cameraPosition.x += 0.1f;
    */
    
    if( glfwGetKey( 'S' ) == GLFW_PRESS )
		cameraPosition.z += 0.1f;
	else if( glfwGetKey( 'W' ) == GLFW_PRESS )
		cameraPosition.z -= 0.1f;
    if( glfwGetKey( 'A' ) == GLFW_PRESS )
		cameraPosition.x -= 0.1f;
    if( glfwGetKey( 'D' ) == GLFW_PRESS )
		cameraPosition.x += 0.1f;
    
    //s_fRot += 1.0f;

	glm::mat4 projectionMatrix = glm::perspective(45.0f, windowWidth / windowHeight, 0.1f, 1000.0f);
	glm::mat4 viewMatrix = glm::lookAt(cameraPosition, cameraPosition - glm::vec3(0.0f,0.0f,1.0f), glm::vec3(0.0f,1.0f,0.0f));
	glm::mat4 worldMatrix = glm::rotate(s_fRot, 0.0f, 1.0f, 0.0f);
	
	//g_pModel->Render(worldMatrix,viewMatrix,projectionMatrix);
    
    // Draw building 1
    building->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    building->SetScale(1.0f, 1.0f, 1.0f);
    //building->SetRotation(s_fRot, 0.0f, 1.0f, 0.0f);
    //building->Render(program, &projectionMatrix, &viewMatrix, worldMatrix);
    
    
    for (int j = 0, posJ = 0; j < 22; j++, posJ++) {
        for (int i = 0, posI = 0; i < 22; i++, posI++) {
            building->SetPosition(glm::vec3(posI, 0.0f, posJ));
            building->Render(program, &projectionMatrix, &viewMatrix, worldMatrix);
            
            if (i % 2 != 0)
                posI += 1;
        }
        
        if (j % 2 != 0)
            posJ += 1;
    }
    
}


