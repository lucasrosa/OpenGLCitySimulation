//========================================================================
// Week 11 - Lighting Part 2
//
// Example 6: Lighting the building
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
#include "W_Model.h"

struct SurfaceMaterial
{
	wolf::Color4	m_ambient;    
	wolf::Color4	m_diffuse;
	wolf::Color4	m_specular; // w stores the specular power
    SurfaceMaterial() : m_diffuse(0,0,0,0), m_specular(0,0,0,0), m_ambient(0,0,0,0) {}
};

struct DirectionalLight
{
	glm::vec3		m_vDirection;
    wolf::Color4    m_ambient;
	wolf::Color4	m_diffuse;
	wolf::Color4	m_specular;
    DirectionalLight() : m_diffuse(0,0,0,0), m_specular(0,0,0,0), m_ambient(0,0,0,0) {}    
};

static SurfaceMaterial g_maskSurface;
static DirectionalLight g_light;
static wolf::Model* g_pModel;


void InitExample6()
{
    glEnable(GL_DEPTH_TEST);
    
	g_pModel = new wolf::Model("data/week9/building/building.pod","data/week9/building/");

	// Initialize the surface material
	g_maskSurface.m_diffuse = wolf::Color4(1,1,1,1);
	g_maskSurface.m_ambient = wolf::Color4(1,1,1,1);
	g_maskSurface.m_specular = wolf::Color4(1,1,1,200);
	
	// Initialize the light parameters
	g_light.m_diffuse = wolf::Color4(0.5,0.5,0,1);
	g_light.m_ambient = wolf::Color4(0.1,0.1,0.1,1);
	g_light.m_specular = wolf::Color4(1,0,0,1);
	g_light.m_vDirection = glm::vec3(0.0f,0.0f,-1.0f);
}

void RenderExample6()
{
    static glm::vec3 s_vCamPos(0.0f, 2.0f, 10.0f);
    static float s_fRot = 0.0f;

    if( glfwGetKey( GLFW_KEY_DOWN ) == GLFW_PRESS )
		s_vCamPos.z += 1.0f;
	else if( glfwGetKey( GLFW_KEY_UP ) == GLFW_PRESS )
		s_vCamPos.z -= 1.0f;
    if( glfwGetKey( GLFW_KEY_LEFT ) == GLFW_PRESS )
		s_vCamPos.x -= 1.0f;
    if( glfwGetKey( GLFW_KEY_RIGHT ) == GLFW_PRESS )
		s_vCamPos.x += 1.0f;

    s_fRot += 1.0f;
    
	glm::mat4 mProj = glm::perspective(45.0f, 1280.0f / 720.0f, 0.1f, 1000.0f);
	glm::mat4 mView = glm::lookAt(s_vCamPos, s_vCamPos - glm::vec3(0.0f,0.0f,1.0f), glm::vec3(0.0f,1.0f,0.0f));
	glm::mat4 mWorld = glm::rotate(s_fRot, 0.0f, 1.0f, 0.0f);
	
	g_pModel->Render(mWorld,mView,mProj);

#if 0
    g_pMat->SetUniform("WorldViewProj", mWVP);
	g_pMat->SetUniform("World", mWorld);
	g_pMat->SetUniform("WorldIT", mWorldIT);
    g_pMat->SetUniform("ViewDir", glm::vec3(0.0f,0.0f,1.0f));

	// Set the light parameters
	g_pMat->SetUniform("LightAmbient", g_light.m_ambient);
	g_pMat->SetUniform("LightDiffuse", g_light.m_diffuse);
	g_pMat->SetUniform("LightSpecular", g_light.m_specular);
	g_pMat->SetUniform("LightDir", g_light.m_vDirection);

	// Set the surface parameters
	g_pMat->SetUniform("MatAmbient", g_maskSurface.m_ambient);
	g_pMat->SetUniform("MatDiffuse", g_maskSurface.m_diffuse);
	g_pMat->SetUniform("MatSpecular", g_maskSurface.m_specular);
#endif
}


