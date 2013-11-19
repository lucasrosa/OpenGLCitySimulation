//========================================================================
// Week 11 - Lighting Part 2
//
// Example 3: Spot Light
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

struct SurfaceMaterial
{
	wolf::Color4	m_ambient;    
	wolf::Color4	m_diffuse;
	wolf::Color4	m_specular; // w stores the specular power
    SurfaceMaterial() : m_diffuse(0,0,0,0), m_specular(0,0,0,0), m_ambient(0,0,0,0) {}
};

struct SpotLight
{
	glm::vec3		m_vPosition;
	glm::vec3		m_vDirection;
	glm::vec3		m_vAttenuation;
	float			m_fRange;
	float			m_fSpot;
    wolf::Color4    m_ambient;
	wolf::Color4	m_diffuse;
	wolf::Color4	m_specular;
    SpotLight() : m_fRange(0), m_fSpot(0), m_diffuse(0,0,0,0), m_specular(0,0,0,0), m_ambient(0,0,0,0) {}    
};


static wolf::VertexBuffer* g_pVB = 0;
static wolf::IndexBuffer* g_pIB = 0;
static wolf::VertexDeclaration* g_pDecl = 0;
static wolf::Material* g_pMat = 0;
static SurfaceMaterial g_maskSurface;
static SpotLight g_light;

void InitExample3()
{
    glEnable(GL_DEPTH_TEST);
    
    CPVRTModelPOD pod;
	pod.ReadFromFile("data/week11/test.pod");

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
    g_pMat->SetProgram("data/week11/spot_light.vsh", "data/week11/spot_light.fsh");
	wolf::Texture* pTex = wolf::TextureManager::CreateTexture("data/week11/MaskMain.tga");
	pTex->SetWrapMode(wolf::Texture::WM_Repeat);

	// Initialize the surface material
	g_maskSurface.m_diffuse = wolf::Color4(1,1,1,1);
	g_maskSurface.m_ambient = wolf::Color4(1,1,1,1);
	g_maskSurface.m_specular = wolf::Color4(1,1,1,200);
	
	// Initialize the light parameters
	g_light.m_diffuse = wolf::Color4(0,1,0,1);
	g_light.m_ambient = wolf::Color4(0.1,0.1,0.1,1);
	g_light.m_specular = wolf::Color4(1,1,0,1);
	g_light.m_vPosition = glm::vec3(0.0f,27.5f,40.0f);
	g_light.m_vDirection = glm::vec3(0.0f,0.0f,-1.0f);
	g_light.m_fRange = 250.0f;
	g_light.m_fSpot = 50;
	g_light.m_vAttenuation = glm::vec3(0.0f,0.05f,0.0f);
}

void RenderExample3()
{
    static float s_fRot = 0.0f;
	glm::mat4 mProj = glm::perspective(45.0f, 1280.0f / 720.0f, 0.1f, 1000.0f);
	glm::mat4 mView = glm::lookAt(glm::vec3(0.0f,30.0f,60.0f), glm::vec3(0.0f,30.0f,0.0f), glm::vec3(0.0f,1.0f,0.0f));
	glm::mat4 mWorld = glm::rotate(s_fRot, 0.0f, 1.0f, 0.0f);
    
	if( glfwGetKey( GLFW_KEY_LSHIFT ) == GLFW_PRESS )
	{
		// Move the light left and right
		if( glfwGetKey( GLFW_KEY_LEFT ) == GLFW_PRESS )
			g_light.m_fSpot -= 1.0f;
		else if( glfwGetKey( GLFW_KEY_RIGHT ) == GLFW_PRESS )
			g_light.m_fSpot += 1.0f;

		// Move the light up and down
		if( glfwGetKey( GLFW_KEY_UP ) == GLFW_PRESS )
			g_light.m_vPosition.y += 0.5f;
		else if( glfwGetKey( GLFW_KEY_DOWN ) == GLFW_PRESS )
			g_light.m_vPosition.y -= 0.5f;
    }
	else
	{
		// Rotate the model
		if( glfwGetKey( GLFW_KEY_LEFT ) == GLFW_PRESS )
			s_fRot -= 1.0f;
		else if( glfwGetKey( GLFW_KEY_RIGHT ) == GLFW_PRESS )
			s_fRot += 1.0f;

		// Move the light forward and back
		if( glfwGetKey( GLFW_KEY_UP ) == GLFW_PRESS )
			g_light.m_vPosition.z -= 0.5f;
		else if( glfwGetKey( GLFW_KEY_DOWN ) == GLFW_PRESS )
			g_light.m_vPosition.z += 0.5f;
	}
    
	// Bind Uniforms
	glm::mat4 mWVP = mProj * mView * mWorld;
	glm::mat3 mWorldIT(mWorld);
	mWorldIT = glm::inverse(mWorldIT);
	mWorldIT = glm::transpose(mWorldIT);

    g_pMat->SetUniform("WorldViewProj", mWVP);
    g_pMat->SetUniform("World", mWorld);
	g_pMat->SetUniform("WorldIT", mWorldIT);
    g_pMat->SetUniform("ViewDir", glm::vec3(0.0f,0.0f,1.0f));

	// Set the light parameters
	g_pMat->SetUniform("LightAmbient", g_light.m_ambient);
	g_pMat->SetUniform("LightDiffuse", g_light.m_diffuse);
	g_pMat->SetUniform("LightSpecular", g_light.m_specular);
	g_pMat->SetUniform("LightPos", g_light.m_vPosition);
	g_pMat->SetUniform("LightDir", g_light.m_vDirection);
	g_pMat->SetUniform("LightAttenuation", g_light.m_vAttenuation);
	g_pMat->SetUniform("LightRange", g_light.m_fRange);
	g_pMat->SetUniform("LightSpot", g_light.m_fSpot);

	// Set the surface parameters
	g_pMat->SetUniform("MatAmbient", g_maskSurface.m_ambient);
	g_pMat->SetUniform("MatDiffuse", g_maskSurface.m_diffuse);
	g_pMat->SetUniform("MatSpecular", g_maskSurface.m_specular);
    
	// Use shader program.
	g_pMat->Apply();
    
	// Set up source data
	g_pDecl->Bind();

    // Draw!
	glDrawElements(GL_TRIANGLES, g_pIB->GetNumIndices(), GL_UNSIGNED_SHORT, 0);
}


