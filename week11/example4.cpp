//========================================================================
// Week 11 - Lighting Part 2
//
// Example 4: Normal mapping with directional light
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

struct PointLight
{
	glm::vec3		m_vPosition;
	glm::vec3		m_vAttenuation;
	float			m_fRange;
    wolf::Color4    m_ambient;
	wolf::Color4	m_diffuse;
	wolf::Color4	m_specular;
    PointLight() : m_fRange(0), m_diffuse(0,0,0,0), m_specular(0,0,0,0), m_ambient(0,0,0,0) {}    
};

static wolf::VertexBuffer* g_pVB = 0;
static wolf::VertexDeclaration* g_pDecl = 0;
static wolf::Material* g_pMatBump = 0;
static wolf::Material* g_pMatFlat = 0;
static SurfaceMaterial g_surface;
static PointLight g_light;

struct Vertex
{
	GLfloat x,y,z;
	GLfloat u, v;
	GLfloat nx, ny, nz;
	GLfloat tx, ty, tz;
};

static const Vertex g_aVertices[] = 
{
	// Front
	{ -0.5f, -0.5f, 0.5f,  0, 1,		0, 0, 1,		1, 0, 0 }, 
	{ -0.5f,  0.5f, 0.5f,  0, 0,		0, 0, 1,		1, 0, 0 }, 
	{  0.5f,  0.5f, 0.5f,  1, 0,		0, 0, 1,		1, 0, 0 }, 
	{  0.5f,  0.5f, 0.5f,  1, 0,		0, 0, 1,		1, 0, 0 }, 
	{  0.5f, -0.5f, 0.5f,  1, 1,		0, 0, 1,		1, 0, 0 }, 
	{ -0.5f, -0.5f, 0.5f,  0, 1,		0, 0, 1,		1, 0, 0 }, 
};

void InitExample4()
{
    glEnable(GL_DEPTH_TEST);
    
    g_pVB = wolf::BufferManager::CreateVertexBuffer(g_aVertices, sizeof(Vertex)*3*2);
    
    g_pDecl = new wolf::VertexDeclaration;
    g_pDecl->Begin();
    g_pDecl->AppendAttribute(wolf::AT_Position, 3, wolf::CT_Float);
    g_pDecl->AppendAttribute(wolf::AT_TexCoord1, 2, wolf::CT_Float);
    g_pDecl->AppendAttribute(wolf::AT_Normal, 3, wolf::CT_Float);
    g_pDecl->AppendAttribute(wolf::AT_Tangent, 3, wolf::CT_Float);
    g_pDecl->SetVertexBuffer(g_pVB);
    g_pDecl->End();
    
    g_pMatBump = wolf::MaterialManager::CreateMaterial("bumpy");
	g_pMatBump->SetProgram("data/week11/dir_nmap.vsh", "data/week11/dir_nmap.fsh");
    g_pMatBump->SetTexture("texture", wolf::TextureManager::CreateTexture("data/week11/rock.tga"));
    g_pMatBump->SetTexture("nmap", wolf::TextureManager::CreateTexture("data/week11/rockNormal.tga"));

    g_pMatFlat = wolf::MaterialManager::CreateMaterial("flat");
    g_pMatFlat->SetProgram("data/week11/point_light.vsh", "data/week11/point_light.fsh");
    g_pMatFlat->SetTexture("texture", wolf::TextureManager::CreateTexture("data/week11/rock.tga"));
    g_pMatFlat->SetTexture("nmap", wolf::TextureManager::CreateTexture("data/week11/rockNormal.tga"));

	// Initialize the surface material
	g_surface.m_diffuse = wolf::Color4(1,1,1,1);
	g_surface.m_ambient = wolf::Color4(1,1,1,1);
	g_surface.m_specular = wolf::Color4(1,1,1,40);
	
	// Initialize the light parameters
	g_light.m_diffuse = wolf::Color4(1,0.8,0.8,1);
	g_light.m_ambient = wolf::Color4(0.3,0.3,0.3,1);
	g_light.m_specular = wolf::Color4(1,0,0,1);
	g_light.m_vPosition = glm::vec3(0.0f,0.0f,2.0f);
	g_light.m_fRange = 250.0f;
	g_light.m_vAttenuation = glm::vec3(1.0f,0.0f,0.0f);
}

void RenderExample4()
{
    static float s_fRot = 0.0f;
	glm::mat4 mProj = glm::perspective(45.0f, 1280.0f / 720.0f, 0.1f, 1000.0f);
	glm::mat4 mView = glm::lookAt(glm::vec3(0.0f,0.0f,2.5f), glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,1.0f,0.0f));
	glm::mat4 mWorld = glm::rotate(s_fRot, 0.0f, 1.0f, 0.0f);
    
	if( glfwGetKey( GLFW_KEY_LSHIFT ) == GLFW_PRESS )
	{
		// Move the light left and right
		if( glfwGetKey( GLFW_KEY_LEFT ) == GLFW_PRESS )
			g_light.m_vPosition.x -= 0.5f;
		else if( glfwGetKey( GLFW_KEY_RIGHT ) == GLFW_PRESS )
			g_light.m_vPosition.x += 0.5f;

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

	wolf::Material* pMat = g_pMatBump;
	if( glfwGetKey('F') == GLFW_PRESS )
		pMat = g_pMatFlat;

    pMat->SetUniform("WorldViewProj", mWVP);
	pMat->SetUniform("World", mWorld);
	pMat->SetUniform("WorldIT", mWorldIT);
    pMat->SetUniform("ViewDir", glm::vec3(0.0f,0.0f,1.0f));

	// Set the light parameters
	pMat->SetUniform("LightAmbient", g_light.m_ambient);
	pMat->SetUniform("LightDiffuse", g_light.m_diffuse);
	pMat->SetUniform("LightSpecular", g_light.m_specular);
	pMat->SetUniform("LightPos", g_light.m_vPosition);
	pMat->SetUniform("LightAttenuation", g_light.m_vAttenuation);
	pMat->SetUniform("LightRange", g_light.m_fRange);

	// Set the surface parameters
	pMat->SetUniform("MatAmbient", g_surface.m_ambient);
	pMat->SetUniform("MatDiffuse", g_surface.m_diffuse);
	pMat->SetUniform("MatSpecular", g_surface.m_specular);
    
	// Use shader program.
	pMat->Apply();
    
	// Set up source data
	g_pDecl->Bind();

    // Draw!
	glDrawArrays(GL_TRIANGLES, 0, 3*2);
}


