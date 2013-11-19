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

// Window size
static float windowWidth  = 1280.0f;                    // Width of our window
static float windowHeight = 800.0f;                    // Heightof our window
static GLint midWindowX = windowWidth  / 2;         // Middle of the window horizontally
static GLint midWindowY = windowHeight / 2;         // Middle of the window vertically

// ------
// The value of 1 degree in radians
const float TO_RADS = 3.141592654f / 180.0f;

GLfloat fieldOfView = 45.0f;                 // Define our field of view (i.e. how quickly foreshortening occurs)
GLfloat near        = 1.0f;                  // The near (Z Axis) point of our viewing frustrum (default 1.0f)
GLfloat far         = 1500.0f;               // The far  (Z Axis) point of our viewing frustrum (default 1500.0f)

// Camera rotation
GLfloat camXRot = 0.0f;
GLfloat camYRot = 0.0f;
GLfloat camZRot = 0.0f;

// Camera position
GLfloat camXPos = 0.0f;
GLfloat camYPos = 0.0f;
GLfloat camZPos = 0.0f;

// Camera movement speed
GLfloat camXSpeed = 0.0f;
GLfloat camYSpeed = 0.0f;
GLfloat camZSpeed = 0.0f;

GLint frameCount = 0; // How many frames have we drawn?

// How fast we move (higher values mean we move and strafe faster)
GLfloat movementSpeedFactor = .5f;

// Hoding any keys down?
bool holdingForward     = false;
bool holdingBackward    = false;
bool holdingLeftStrafe  = false;
bool holdingRightStrafe = false;

// Function to convert degrees to radians
float toRads(const float &theAngleInDegrees)
{
    return theAngleInDegrees * TO_RADS;
}


// Function to move the camera the amount we've calculated in the calculateCameraMovement function
void moveCamera()
{
    camXPos += camXSpeed;
    camYPos += camYSpeed;
    camZPos += camZSpeed;
}

// Function to deal with mouse position changes, called whenever the mouse cursorm moves
void handleMouseMove(int mouseX, int mouseY)
{
    GLfloat vertMouseSensitivity  = 10.0f;
    GLfloat horizMouseSensitivity = 10.0f;
    
    //cout << "Mouse cursor is at position (" << mouseX << ", " << mouseY << endl;
    
    int horizMovement = mouseX - midWindowX;
    int vertMovement  = mouseY - midWindowY;
    
    camXRot += vertMovement / vertMouseSensitivity;
    camYRot += horizMovement / horizMouseSensitivity;
    
    // Control looking up and down with the mouse forward/back movement
    // Limit loking up to vertically up
    if (camXRot < -90.0f)
    {
        camXRot = -90.0f;
    }
    
    // Limit looking down to vertically down
    if (camXRot > 90.0f)
    {
        camXRot = 90.0f;
    }
    
    // Looking left and right. Keep the angles in the range -180.0f (anticlockwise turn looking behind) to 180.0f (clockwise turn looking behind)
    if (camYRot < -180.0f)
    {
        camYRot += 360.0f;
    }
    
    if (camYRot > 180.0f)
    {
        camYRot -= 360.0f;
    }
    
    // Reset the mouse position to the centre of the window each frame
    glfwSetMousePos(midWindowX, midWindowY);
}

// Function to calculate which direction we need to move the camera and by what amount
void calculateCameraMovement()
{
    // Break up our movement into components along the X, Y and Z axis
    float camMovementXComponent = 0.0f;
    float camMovementYComponent = 0.0f;
    float camMovementZComponent = 0.0f;
    
    if (holdingForward == true)
    {
        // Control X-Axis movement
        float pitchFactor = cos(toRads(camXRot));
        camMovementXComponent += ( movementSpeedFactor * float(sin(toRads(camYRot))) ) * pitchFactor;
        
        // Control Y-Axis movement
        camMovementYComponent += movementSpeedFactor * float(sin(toRads(camXRot))) * -1.0f;
        
        // Control Z-Axis movement
        float yawFactor = float(cos(toRads(camXRot)));
        camMovementZComponent += ( movementSpeedFactor * float(cos(toRads(camYRot))) * -1.0f ) * yawFactor;
    }
    
    if (holdingBackward == true)
    {
        // Control X-Axis movement
        float pitchFactor = cos(toRads(camXRot));
        camMovementXComponent += ( movementSpeedFactor * float(sin(toRads(camYRot))) * -1.0f) * pitchFactor;
        
        // Control Y-Axis movement
        camMovementYComponent += movementSpeedFactor * float(sin(toRads(camXRot)));
        
        // Control Z-Axis movement
        float yawFactor = float(cos(toRads(camXRot)));
        camMovementZComponent += ( movementSpeedFactor * float(cos(toRads(camYRot))) ) * yawFactor;
    }
    
    if (holdingLeftStrafe == true)
    {
        // Calculate our Y-Axis rotation in radians once here because we use it twice
        float yRotRad = toRads(camYRot);
        
        camMovementXComponent += -movementSpeedFactor * float(cos(yRotRad));
        camMovementZComponent += -movementSpeedFactor * float(sin(yRotRad));
    }
    
    if (holdingRightStrafe == true)
    {
        // Calculate our Y-Axis rotation in radians once here because we use it twice
        float yRotRad = toRads(camYRot);
        
        camMovementXComponent += movementSpeedFactor * float(cos(yRotRad));
        camMovementZComponent += movementSpeedFactor * float(sin(yRotRad));
    }
    
    // After combining our movements for any & all keys pressed, assign them to our camera speed along the given axis
    camXSpeed = camMovementXComponent;
    camYSpeed = camMovementYComponent;
    camZSpeed = camMovementZComponent;
    
    // Cap the speeds to our movementSpeedFactor (otherwise going forward and strafing at an angle is twice as fast as just going forward!)
    // X Speed cap
    if (camXSpeed > movementSpeedFactor)
    {
        //cout << "high capping X speed to: " << movementSpeedFactor << endl;
        camXSpeed = movementSpeedFactor;
    }
    if (camXSpeed < -movementSpeedFactor)
    {
        //cout << "low capping X speed to: " << movementSpeedFactor << endl;
        camXSpeed = -movementSpeedFactor;
    }
    
    // Y Speed cap
    if (camYSpeed > movementSpeedFactor)
    {
        //cout << "low capping Y speed to: " << movementSpeedFactor << endl;
        camYSpeed = movementSpeedFactor;
    }
    if (camYSpeed < -movementSpeedFactor)
    {
        //cout << "high capping Y speed to: " << movementSpeedFactor << endl;
        camYSpeed = -movementSpeedFactor;
    }
    
    // Z Speed cap
    if (camZSpeed > movementSpeedFactor)
    {
        //cout << "high capping Z speed to: " << movementSpeedFactor << endl;
        camZSpeed = movementSpeedFactor;
    }
    if (camZSpeed < -movementSpeedFactor)
    {
        //cout << "low capping Z speed to: " << movementSpeedFactor << endl;
        camZSpeed = -movementSpeedFactor;
    }
}

// Function to set flags according to which keys are pressed or released
void handleKeypress(int theKey, int theAction)
{
    // If a key is pressed, toggle the relevant key-press flag
    if (theAction == GLFW_PRESS)
    {
        
        switch(theKey)
        {
            case 'W':
                holdingForward = true;
                break;
                
            case 'S':
                holdingBackward = true;
                break;
                
            case 'A':
                holdingLeftStrafe = true;
                break;
                
            case 'D':
                holdingRightStrafe = true;
                break;
                
            default:
                // Do nothing...
                break;
        }
    }
    else // If a key is released, toggle the relevant key-release flag
    {
        switch(theKey)
        {
            case 'W':
                holdingForward = false;
                break;
                
            case 'S':
                holdingBackward = false;
                break;
                
            case 'A':
                holdingLeftStrafe = false;
                break;
                
            case 'D':
                holdingRightStrafe = false;
                break;
                
            default:
                // Do nothing...
                break;
        }
    }
}

//------------------


// Define the Building
static wolf::Building* building = 0;

// Define the programs
static wolf::Program* program; //q_pProgram;

// Define the general matrices
static glm::mat4 projectionMatrix = glm::perspective(45.0f, 640.0f / 480.0f, 0.1f, 1000.0f);
static glm::mat4 viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,1.0f,0.0f));


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
    
    
    //-------
    // Set the mouse cursor to the centre of our window
    glfwSetMousePos(midWindowX, midWindowY);
    
    // Call our initGL function to set up our OpenGL options
    //initGL();
    
    // Specify the function which should execute when a key is pressed or released
    glfwSetKeyCallback(handleKeypress);
    
    // Specify the function which should execute when the mouse is moved
    glfwSetMousePosCallback(handleMouseMove);
    
    //-------
}

void RenderAssignment2()
{
    
    //------
    // Reset the matrix
    //glMatrixMode(GL_MODELVIEW);
    //glLoadIdentity();
    
    
    //------
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
    //static glm::vec3 cameraPosition(0.0f, 2.0f, 5.0f);
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
    /*
    if( glfwGetKey( 'S' ) == GLFW_PRESS )
		cameraPosition.z += 0.1f;
	else if( glfwGetKey( 'W' ) == GLFW_PRESS )
		cameraPosition.z -= 0.1f;
    if( glfwGetKey( 'A' ) == GLFW_PRESS )
		cameraPosition.x -= 0.1f;
    if( glfwGetKey( 'D' ) == GLFW_PRESS )
		cameraPosition.x += 0.1f;
    */
    //s_fRot += 1.0f;
    
    //glm::mat4 worldMatrix = glm::rotate(s_fRot, 0.0f, 1.0f, 0.0f);
    //setup the viewing transformation
    //glm::mat4 T     = glm::translate(glm::mat4(1.0f),glm::vec3(-camXPos, -camYPos, -camZPos)); // glTranslatef(0,0,dist);
    //glm::mat4 T     = glm::translate(glm::mat4(1.0f),glm::vec3(-camXPos, -camYPos, -camZPos)); // glTranslatef(0,0,dist);
    //glm::mat4 Rx    = glm::rotate(T,  camXRot, glm::vec3(1.0f, 0.0f, 0.0f)); //glRotatef(rX,1,0,0);
    //glm::mat4 V     = glm::rotate(Rx, camYRot, glm::vec3(0.0f, 1.0f, 0.0f)); //glRotatef(rY,0,1,0);
    glm::mat4 T     = glm::translate(glm::mat4(1.0f),glm::vec3(0.0f, 0.0f, 0.0f)); // glTranslatef(0,0,dist);
    glm::mat4 Rx    = glm::rotate(T,  camXRot, glm::vec3(1.0f, 0.0f, 0.0f)); //glRotatef(rX,1,0,0);
    glm::mat4 V     = glm::rotate(Rx, camYRot, glm::vec3(0.0f, 1.0f, 0.0f)); //glRotatef(rY,0,1,0);
    V = V * glm::translate(glm::mat4(1.0f),glm::vec3(-camXPos, -camYPos, -camZPos));
    // get obj matrix from object position
    
	glm::mat4 projectionMatrix = glm::perspective(45.0f, windowWidth / windowHeight, 0.1f, 1000.0f);
	//glm::mat4 viewMatrix = glm::lookAt(cameraPosition, cameraPosition - glm::vec3(0.0f,0.0f,1.0f), glm::vec3(0.0f,1.0f,0.0f));
    //glm::mat4 viewMatrix = glm::lookAt(cameraPosition, cameraPosition - glm::vec3(0.0f,0.0f,1.0f), glm::vec3(0.0f,1.0f,0.0f));
    
	glm::mat4 worldMatrix = glm::rotate(s_fRot, 0.0f, 1.0f, 0.0f);
	glm::mat4 viewMatrix = V;
    
    
    
    // Move the camera to our location in space
    //glRotatef(camXRot, 1.0f, 0.0f, 0.0f);        // Rotate our camera on the x-axis (looking up and down)
    //glRotatef(camYRot, 0.0f, 1.0f, 0.0f);        // Rotate our camera on the  y-axis (looking left and right)
    //glTranslatef(-camXPos,-camYPos,-camZPos);    // Translate the modelviewm matrix to the position of our camera
    //setup the viewing transformation
    //gluLookAt(camX, camY, camZ, camTgtX, camTgtY, camTgtZ,upX, upY, upZ);

    
	//g_pModel->Render(worldMatrix,viewMatrix,projectionMatrix);
    
    // Draw building 1
    building->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    building->SetScale(1.0f, 1.0f, 1.0f);
    //building->SetRotation(s_fRot, 0.0f, 1.0f, 0.0f);
    //building->Render(program, &projectionMatrix, &viewMatrix, worldMatrix);
    
    
    for (int j = 0, posJ = 0; j < 22; j++, posJ++) {
        for (int i = 0, posI = 0; i < 22; i++, posI++) {
            building->SetPosition(glm::vec3(posI, 0.0f, posJ));
            glm::mat4 objectViewMatrix = viewMatrix * building->GetPosition();
            //building->Render(program, &projectionMatrix, &objectViewMatrix, worldMatrix);
            building->Render(program, &projectionMatrix, &viewMatrix, worldMatrix);
            
            if (i % 2 != 0)
                posI += 1;
        }
        
        if (j % 2 != 0)
            posJ += 1;
    }
    
    // Calculate our camera movement
    calculateCameraMovement();
    
    // Move our camera
    moveCamera();
    
    // Increase our frame counter
    frameCount++;
}


