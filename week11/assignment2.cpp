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
#include "W_Road.h"
#include "W_Model.h"

#include <time.h>       /* time */ // For random number generation
#include <list> // list

// Define number of blocks
int numberOfBlockLines   = 11;
int numberOfBlockColumns = 11;
short numberOfTextures     = 8;
short numberOfRoofTextures = 2;
short numberOfFloors       = 8;

//**********************************
// SUN
float sun[2];
short sunReachedUp = 0;
//**********************************

// Regenerate the city
short r_button_down = 0;

struct blockType {
    short numberOfBuildings;
    short buildingTypes[4];
};

struct buildingInformation {
    short numberOfFloors;
    short texture;
    short roofTexture;
    short type;
    float positionX;
    float positionZ;
};

static struct blockType blockTypes[12];

// Define the buildings information list
static std::list<buildingInformation> buildingsInformation;

void initCityPlan() {
    // Types:
    // 0: Normal sized Building
    // 1: Double sized Building Vertical
    // 2: Double sized Building Horizontal
    // 3: Triple sized (L shaped) Building (rotated 0 degrees)
    // 4: Triple sized (L shaped) Building (rotated 90 degrees)
    // 5: Triple sized (L shaped) Building (rotated 180 degrees)
    // 6: Triple sized (L shaped) Building (rotated 270 degrees)
    // 7: Quadruple sized Building : 7
    
    int blockNumber = 0;
    //--------- Block type 0 ----------> Simple block, 4 buildings of normal size
    // Visual representation:   1 2
    //                          3 4
    blockTypes[blockNumber].numberOfBuildings = 4;
    blockTypes[blockNumber].buildingTypes[0] = 0;
    blockTypes[blockNumber].buildingTypes[1] = 0;
    blockTypes[blockNumber].buildingTypes[2] = 0;
    blockTypes[blockNumber].buildingTypes[3] = 0;
    blockNumber++;
    
    //--------- Block type 1 ----------> 3 buildings: 2 of normal size and one of double size, double size on the south
    // Visual representation:   1 2
    //                          3 3
    blockTypes[blockNumber].numberOfBuildings = 3;
    blockTypes[blockNumber].buildingTypes[0] = 0;
    blockTypes[blockNumber].buildingTypes[1] = 0;
    blockTypes[blockNumber].buildingTypes[2] = 2;
    blockTypes[blockNumber].buildingTypes[3] = -1;
    blockNumber++;
    
    //--------- Block type 2 ----------> 3 buildings: 2 of normal size and one of double size, double size on the north
    // Visual representation:   1 1
    //                          2 3
    blockTypes[blockNumber].numberOfBuildings = 3;
    blockTypes[blockNumber].buildingTypes[0] = 2;
    blockTypes[blockNumber].buildingTypes[1] = -1;
    blockTypes[blockNumber].buildingTypes[2] = 0;
    blockTypes[blockNumber].buildingTypes[3] = 0;
    blockNumber++;
    
    //--------- Block type 3 ----------> 3 buildings: 2 of normal size and one of double size, double size on the right
    // Visual representation:   1 2
    //                          3 2
    blockTypes[blockNumber].numberOfBuildings = 3;
    blockTypes[blockNumber].buildingTypes[0] = 0;
    blockTypes[blockNumber].buildingTypes[1] = 1;
    blockTypes[blockNumber].buildingTypes[2] = 0;
    blockTypes[blockNumber].buildingTypes[3] = -1;
    blockNumber++;
    
    //--------- Block type 4 ----------> 3 buildings: 2 of normal size and one of double size, double size on the left
    // Visual representation:   1 2
    //                          1 3
    blockTypes[blockNumber].numberOfBuildings = 3;
    blockTypes[blockNumber].buildingTypes[0] = 1;
    blockTypes[blockNumber].buildingTypes[1] = 0;
    blockTypes[blockNumber].buildingTypes[2] = -1;
    blockTypes[blockNumber].buildingTypes[3] = 0;

    blockNumber++;
    
    //--------- Block type 5 ----------> 2 buildings: 2 double size vertically positioned
    // Visual representation:   1 2
    //                          1 2
    blockTypes[blockNumber].numberOfBuildings = 2;
    blockTypes[blockNumber].buildingTypes[0] = 1;
    blockTypes[blockNumber].buildingTypes[1] = 1;
    blockTypes[blockNumber].buildingTypes[2] = -1;
    blockTypes[blockNumber].buildingTypes[3] = -1;
    blockNumber++;
    
    //--------- Block type 6 ----------> 2 buildings: 2 double size horizontally positioned
    // Visual representation:   1 1
    //                          2 2
    blockTypes[blockNumber].numberOfBuildings = 2;
    blockTypes[blockNumber].buildingTypes[0] = 2;
    blockTypes[blockNumber].buildingTypes[1] = -1;
    blockTypes[blockNumber].buildingTypes[2] = 2;
    blockTypes[blockNumber].buildingTypes[3] = -1;
    blockNumber++;
    
    //--------- Block type 7 ----------> 2 buildings: 1 of triple size rotated 180 degrees and 1 of normal size
    // Visual representation:   1 1
    //                          2 1
    blockTypes[blockNumber].numberOfBuildings = 2;
    blockTypes[blockNumber].buildingTypes[0] = 5;
    blockTypes[blockNumber].buildingTypes[1] = -1;
    blockTypes[blockNumber].buildingTypes[2] = 0;
    blockTypes[blockNumber].buildingTypes[3] = -1;
    blockNumber++;
    
    //--------- Block type 8 ----------> 2 buildings: 1 of normal size and one of triple size rotated 270 degrees
    // Visual representation:   1 2
    //                          2 2
    blockTypes[blockNumber].numberOfBuildings = 2;
    blockTypes[blockNumber].buildingTypes[0] = 0;
    blockTypes[blockNumber].buildingTypes[1] = 6;
    blockTypes[blockNumber].buildingTypes[2] = -1;
    blockTypes[blockNumber].buildingTypes[3] = -1;
    blockNumber++;
    
    //--------- Block type 9 ----------> 2 buildings: 1 of triple size and one of normal size
    // Visual representation:   1 2
    //                          1 1
    blockTypes[blockNumber].numberOfBuildings = 2;
    blockTypes[blockNumber].buildingTypes[0] = 3;
    blockTypes[blockNumber].buildingTypes[1] = 0;
    blockTypes[blockNumber].buildingTypes[2] = -1;
    blockTypes[blockNumber].buildingTypes[3] = -1;
    blockNumber++;
    
    //--------- Block type 10 ----------> 2 buildings: 1 of triple size rotated 90 degrees and one of normal size
    // Visual representation:   1 1
    //                          1 2
    blockTypes[blockNumber].numberOfBuildings = 2;
    blockTypes[blockNumber].buildingTypes[0] = 4;
    blockTypes[blockNumber].buildingTypes[1] = -1;
    blockTypes[blockNumber].buildingTypes[2] = -1;
    blockTypes[blockNumber].buildingTypes[3] = 0;
    blockNumber++;
    
    //--------- Block type 11 ----------> 1 building of quadruple size
    // Visual representation:   1 1
    //                          1 1
    blockTypes[blockNumber].numberOfBuildings = 1;
    blockTypes[blockNumber].buildingTypes[0] = 7;
    blockTypes[blockNumber].buildingTypes[1] = -1;
    blockTypes[blockNumber].buildingTypes[2] = -1;
    blockTypes[blockNumber].buildingTypes[3] = -1;
}

void generateCityPlan() {
    printf("LOG: generateCityPlan() called.\n");
    // Clean the list that stores each building information
    buildingsInformation.clear();
    //int *worstCaseBuildings = new int[(numberOfBlockLines*numberOfBlockColumns*4)];
    // initialize random seed
    srand ((int) time(NULL));
    
    int randomBlock;
    for (int line = 0, blockLinePosition = 0; line <= numberOfBlockLines; line++) {
        for (int column = 0, blockColumnPosition = 0; column <= numberOfBlockColumns; column++) {
            
            // Restore position values to the beginning of the next block in this row (line)
            blockColumnPosition = column * 3;
            blockLinePosition = line * 3;
            //printf("%d x %d \n", blockLinePosition, blockColumnPosition);
            randomBlock = rand() % 12;
            //for (int building = 0; building < blockTypes[randomBlock].numberOfBuildings; building++) {
            for (int building = 0; building < 4; building++) {
                if (blockTypes[randomBlock].buildingTypes[building] == -1) {
                    continue;
                }
                // Randomically generate building information
                int randomNumberOfFloors = (rand() % numberOfFloors+1) + 1; // Generates a number between 1 and 15 (there's no 0 floors);
                int randomTexture = rand() % numberOfTextures;
                int randomRoofTexture = rand() % numberOfRoofTextures;
                
                //Assign the building information to a temporary variable and add it to the list
                struct buildingInformation thisBuilding;
                thisBuilding.numberOfFloors = randomNumberOfFloors;
                thisBuilding.texture        = randomTexture;
                thisBuilding.roofTexture    = randomRoofTexture;
                thisBuilding.type           = blockTypes[randomBlock].buildingTypes[building];
                //printf("blockTypes[%d].buildingTypes[%d] = %hd\n", randomBlock, building, blockTypes[randomBlock].buildingTypes[building]);
                
                
                //-----------
                // Calculate the exact position of the building in the block
                //-----------
                thisBuilding.positionX = blockColumnPosition;
                thisBuilding.positionZ = blockLinePosition;
                
                
                // This switch sets position of the next building in this block
                switch (thisBuilding.type) {
                    case 0:
                        if (building == 1) { // case 1
                            blockColumnPosition--;
                            blockLinePosition++;
                        } else if (building == 0 || building == 2){ // case 0: case 2:
                            blockColumnPosition++;
                        }
                        break;
                    case 2: case 5:
                        if (building == 0) {
                            blockLinePosition++;
                        } else if (thisBuilding.type == 5 && building == 1) {
                            blockLinePosition++;
                        }
                        break;
                    case 1: case 3:
                        if (building == 0) {
                            blockColumnPosition++;
                        }
                        break;
                    case 4:
                        if (building == 0) {
                            blockLinePosition++;
                            blockColumnPosition++;
                        }
                        break;
                    default:
                        break;
                }
                //BuildBuilding(blockPosition, blockTypes[randomBlock].buildingTypes[building], positionInsideBlock = building, randomNumberOfFloors,randomTexture)
                
                buildingsInformation.push_back(thisBuilding);
                //printf("Building %d  of block (%dx%d) has texture %d and %d floors.\n", building, line, column, randomTexture, randomNumberOfFloors);
            }
        }
    }
}


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
GLfloat camXPos = 15.0f;
GLfloat camYPos = 0.0f;
GLfloat camZPos = 45.0f;

// Camera movement speed
GLfloat camXSpeed = 0.0f;
GLfloat camYSpeed = 0.0f;
GLfloat camZSpeed = 0.0f;


// How fast we move (higher values mean we move and strafe faster)
//GLfloat movementSpeedFactor = .125f;
GLfloat movementSpeedFactor = .75;

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
// Define the Road
static wolf::Road* road = 0;

// Define the programs

static wolf::Program* buildingProgram; //q_pProgram;

// Define the general matrices
static glm::mat4 projectionMatrix = glm::perspective(45.0f, 640.0f / 480.0f, 0.1f, 1000.0f);
static glm::mat4 viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,1.0f,0.0f));


void InitAssignment2()
{
    sun[0] = -1.0f;
    sun[1] = 0.0f;
    initCityPlan();
    generateCityPlan();
    
    glEnable(GL_DEPTH_TEST);
    // Hide the mouse cursor
    glfwDisable(GLFW_MOUSE_CURSOR);
    // Set the mouse cursor to the centre of our window
    glfwSetMousePos(midWindowX, midWindowY);
    
    // Instantiate the programs
    //program       = wolf::ProgramManager::CreateProgram("data/week11/polyhedron.vsh", "data/week11/polyhedron.fsh");
    //program         = wolf::ProgramManager::CreateProgram("data/week11/cube.vsh", "data/week11/cube.fsh");
    buildingProgram         = wolf::ProgramManager::CreateProgram("data/week11/building.vsh", "data/week11/building.fsh");
    // Instantiate the Building
    building = new wolf::Building();
    // Instantiate the Road
    road = new wolf::Road();
    
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
    // Calculating the sun sun
    sun[0] += 0.01f;
    if (sun[0]  >= 1.0f) {
        sun[0] = -1.0f;
    }
    if (sunReachedUp) {
        sun[1] -= 0.01f;
    } else {
        sun[1] += 0.01f;
    }
    
    if (sun[1]  >= 1.0f) {
        sunReachedUp = 1;
    } else if (sun[1]  <= 0.0f) {
        sunReachedUp = 0;
    }
        
    // Regenerate city based on press of key R
    if (glfwGetKey( 'R' ) == GLFW_PRESS) {
        // If the R key was not being pressed before, change to the next mode
        if (r_button_down == 0) {
            generateCityPlan();
        }
        // Identifies that the R key is being pressed
        r_button_down = 1;
    } else {
        // Identifies that the R key is not being pressed
        r_button_down = 0;
    }
    
    
    static float s_fRot = 0.0f;
    
    glm::mat4 T     = glm::translate(glm::mat4(1.0f),glm::vec3(0.0f, 0.0f, 0.0f)); // glTranslatef(0,0,dist);
    glm::mat4 Rx    = glm::rotate(T,  camXRot, glm::vec3(1.0f, 0.0f, 0.0f)); //glRotatef(rX,1,0,0);
    glm::mat4 V     = glm::rotate(Rx, camYRot, glm::vec3(0.0f, 1.0f, 0.0f)); //glRotatef(rY,0,1,0);
    V = V * glm::translate(glm::mat4(1.0f),glm::vec3(-camXPos, -camYPos, -camZPos));
    // get obj matrix from object position
    
	glm::mat4 projectionMatrix = glm::perspective(45.0f, windowWidth / windowHeight, 0.1f, 1000.0f);
	glm::mat4 worldMatrix = glm::rotate(s_fRot, 0.0f, 1.0f, 0.0f);
	glm::mat4 viewMatrix = V;
    
    
    
    std::list<buildingInformation>::iterator buildingsInformationIterator;
    // Iterate through the buildings list
    
    for (buildingsInformationIterator = buildingsInformation.begin() ; buildingsInformationIterator != buildingsInformation.end(); buildingsInformationIterator++){
        //building->SetPosition(glm::vec3((*buildingsInformationIterator).positionX, 0.0f, (*buildingsInformationIterator).positionZ));
        building->SetTexture((*buildingsInformationIterator).texture);
        building->SetType((*buildingsInformationIterator).type);
        building->SetNumberOfFloors((*buildingsInformationIterator).numberOfFloors);
        //building->Render(program, &projectionMatrix, &viewMatrix, worldMatrix);
        float x_position = (*buildingsInformationIterator).positionX;
        float z_position = (*buildingsInformationIterator).positionZ;
        short roofTexture = (*buildingsInformationIterator).roofTexture;
        building->Build(buildingProgram,
                        &projectionMatrix,
                        &viewMatrix,
                        worldMatrix,
                        glm::vec3(x_position, 0.0f, z_position),
                        roofTexture,
                        sun
                        );
        
        
    }
    
    // Draw the roads
    for (int line = 0; line < numberOfBlockLines+1; line++) {
        float zPosition = (line)*3-1;
        for (int column = 0; column < numberOfBlockColumns * 3 + 2; column++) {
            float xPosition = column;
            //printf("Road: %fx%f\n", zPosition, xPosition);
            //printf("%f !=")
            //float intersection = (column*3-1);
            if ((column+1) % 3 != 0) { // Intersection
                road->SetTexture(0);
            } else {
                road->SetTexture(1);
                for (int i = 1; i <= 2; i++) {
                    road->SetPosition(glm::vec3(xPosition, 0.0f, zPosition+i));
                    road->Render(buildingProgram, &projectionMatrix, &viewMatrix, worldMatrix, sun);
                }
                
                road->SetTexture(2);
            }
            if (zPosition > -1) {
                road->SetPosition(glm::vec3(xPosition, 0.0f, zPosition));
                road->Render(buildingProgram, &projectionMatrix, &viewMatrix, worldMatrix, sun);
            }
        }
    }
    // Calculate our camera movement
    calculateCameraMovement();
    
    // Move our camera
    moveCamera();
}


