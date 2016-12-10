/*
Main.cpp
Class that initializes and displays the tri models.
Comp465 Computer Graphics System and Design
Project Phase 1.
Christopher Bass
10/09/2016
*/

#pragma once
# define __Windows__
# include "../includes465/include465.hpp"
# include "../includes465/texture.hpp"
# include "TriModel.hpp"
# include "Target.hpp"
# include "Collision.hpp"
# include "Camera.hpp"
# include "Light.h"
# include <iostream>

//planets
const int numPlanets = 5;

//cameras
const int nCameras = 5;
Camera * camera;
Camera * cameras[nCameras];
int cameraCount = 0;

//wb speed
int speedCount = 0;
int wbSpeed[3] = { 10, 50, 200 };

//time quantum count
int tqCount = 0;
int tq[4] = { 5, 40, 100, 500 }; 
int time = 0;
int updateCount = 0;
bool perspective = true;
GLfloat aspectRatio;

// window title strings
char titleStr[100];

GLuint shaderProgram;
char * vertexShaderFile = "simpleVertex.glsl";
char * fragmentShaderFile = "simpleFragment.glsl";

//model-view-proj uniforms
GLuint MV;						// ModelView handle
GLuint MVP;						// ModelViewProjection handle
GLuint Normal;					// ModelView handle

// matrices
glm::mat4 modelMatrix;					// set in shape[i]-->updateDraw()
glm::mat4 viewMatrix;					// set in keyboard()
glm::mat4 mvMatrix;						// model-view
glm::mat4 ModelViewProjectionMatrix;	// set in display();
glm::mat4 projectionMatrix;				// set in display
glm::mat4 vpMatrix;
glm::mat4 normalMatrix;

// vectors and values for lookAt
glm::vec3 eye, at, up;

glm::mat4 identity(1.0f);
int timerDelay = tq[0], frameCount = 0, framesPerSecond = 0;
double currentTime, lastTime, timeInterval;

std::vector<Planet*> warpPos;

//create vao and vbos for planet,ship and missle
GLuint VAO[5];
GLuint BUFFER[5];
TriModel * models[5];
Collision * collision;

// Texture size and file name
int width = 1920, height = 1080;
char * textureFile = "stars.raw";

// Texture information
GLuint texture;
GLuint Texture;
GLuint TexturePosition;
GLuint vTextCoord;
GLuint IsTexture;

//lighting
GLuint ruber;
GLuint planet;
GLuint color;

// Vertex Buffer and Array Objects
GLuint textIBO;
GLuint textBuf;
GLuint textVao;
// Default rotation and translation matrices for th texture
glm::mat4 squareRotation = glm::mat4();
glm::mat4 translateSquare = glm::mat4();

int numberOfSquares = 6;		// number of squares that will be drawn
float textRotation = 0.0f;	// default rotation amount for texture
// The locations of the vertices
GLfloat textVertices[16] = {
	-40000.0f, -40000.0f, 0.0f, 1.0f,	// bottom left vertex
	40000.0f, -40000.0f, 0.0f, 1.0f,	// bottom right vertex
	40000.0f, 40000.0f, 0.0f, 1.0f,		// top right vertex
	-40000.0f, 40000.0f, 0.0f, 1.0f		// top left vertex
};

// Order the vertices of the texture to be drawn
int indices[] = {
	0, 1, 2,
	2, 3, 0
};

// The order the texture coordinates are drawn
GLfloat textCoords[] = {
	0.0f, 1.0f,
	1.0f, 1.0f,
	1.0f, 0.0f,
	0.0f, 0.0f,
};

// Translation 6 textures
glm::vec3 skyTranslation[6] = {
	glm::vec3(0.0f, 0.0f, -40000.0f),
	glm::vec3(0.0f, 0.0f, 40000.0f),
	glm::vec3(0.0f, -40000.0f, 0.0f),
	glm::vec3(0.0f, 40000.0f, 0.0f),
	glm::vec3(-40000.0f, 0.0f, 0.0f),
	glm::vec3(40000.0f, 0.0f, 0.0f)
};

// Rrotation for 6 textures
glm::vec3 skyRotation[6] = {
	glm::vec3(0.0f, 1.0f, 0.0f),
	glm::vec3(0.0f, 1.0f, 0.0f),
	glm::vec3(1.0f, 0.0f, 0.0f),
	glm::vec3(1.0f, 0.0f, 0.0f),
	glm::vec3(0.0f, 1.0f, 0.0f),
	glm::vec3(0.0f, 1.0f, 0.0f),

};

/*Lights*/
bool ruberEnabled = true;
bool ambientEnabled = true;
Light ruberLight() {
	return Light{
		glm::vec3(0.0f, 0.0f, 0.0f),	//pos
		glm::vec3(0.0f, 0.0f, 0.0f),	//dir
		glm::vec3(0.4f, 0.4f, 0.4f),	//ambient
		glm::vec3(1.0f, 1.0f, 1.0f),	//diffuse
		glm::vec3(0.0f, 0.0f, 0.0f),	//spec
		.000000003f,					//atten
		ruberEnabled,
	};
}

Light ambientLight() {
	return Light{
		glm::vec3(0.0f, 0.0f, 0.0f),	//pos
		glm::vec3(0.0f, 0.0f, 0.0f),	//dir
		glm::vec3(0.5f, 0.5f, 0.5f),	//ambient
		glm::vec3(0.0f, 0.0f, 0.0f),	//diffuse
		glm::vec3(0.0f, 0.0f, 0.0f),	//spec
		0.0f,							//atten
		ambientEnabled,
	};
}


/* Method that initialized all the object of the program. 
It loads the tri files of the different models into the buffer and binds a vao for each model.
It initializes the two static camera and also dynamic cameras for the models.
*/
void init(void) {

	//loading shader here
	shaderProgram = loadShaders(vertexShaderFile, fragmentShaderFile);
	//generate vertex arrays and buffers
	glGenVertexArrays(5, &VAO[0]);
	glGenBuffers(5, &BUFFER[0]);
	//loaing planet, spaceship, and missle model
	models[0] = new TriModel("spheres.tri", 264 * 3, VAO[0], BUFFER[0], shaderProgram);
	models[1] = new TriModel("ironship.tri", 132 * 3, VAO[1], BUFFER[1], shaderProgram);
	models[2] = new TriModel("greenmiss.tri", 552 * 3, VAO[2], BUFFER[2], shaderProgram);
	models[3] = new TriModel("silo.tri", 88 * 3, VAO[3], BUFFER[3], shaderProgram);
	models[4] = new TriModel("yellowmiss.tri", 552 * 3, VAO[4], BUFFER[4], shaderProgram);
	
	//uniforms
	MV = glGetUniformLocation(shaderProgram, "ModelView");
	MVP = glGetUniformLocation(shaderProgram, "ModelViewProjection");
	Normal = glGetUniformLocation(shaderProgram, "Normal");

	// following line is an example that can help debug problems w/ shaders
	// you can also print the locations of vPosition, vColor, and vNormal
	printf("Print location of shader variable:  MVP = %d \n", MVP);

	glUseProgram(shaderProgram);

	// set render state values
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.7f, 0.7f, 0.7f, 1.0f);

	//add planets
	models[0]->addPlanet(new Planet("Ruber", 2000, models[0]->getBoundingRadius(), 0, 0, false,glm::vec4(1,.11,.1,1)));
	models[0]->addPlanet(new Planet("Unum", 200, models[0]->getBoundingRadius(), 4000, .004, false, glm::vec4(0, .4, 0, 1)));
	models[0]->addPlanet(new Planet("Duo", 400, models[0]->getBoundingRadius(), 9000, .002, false, glm::vec4(0, .4, 0, 1)));
	models[0]->addPlanet(new Planet("Primus", 100, models[0]->getBoundingRadius(), 8100, .004, true, glm::vec4(0, .4, .5, 1)));
	models[0]->addPlanet(new Planet("Secundus", 150, models[0]->getBoundingRadius(), 7250, .002, true, glm::vec4(0, .4, .5, 1)));
	
	//add moons to Duo
	models[0]->addMoon(2, 3);
	models[0]->addMoon(2, 4);

	//add warbird
	models[1]->setWarBird(new WarBird("warbird",5, models[1]->getBoundingRadius(), glm::vec3(5000,1000,5000),10));

	//add missile sites
	models[3]->addMissileSite(new MissileSite(25, models[1]->getBoundingRadius(), 5, 230, &models[0]->getPlanet(1)->orientationMatrix, &models[1]->getWarBird()->orientationMatrix));
	models[3]->addMissileSite(new MissileSite(25, models[1]->getBoundingRadius(), 5, 180, &models[0]->getPlanet(4)->orientationMatrix, &models[1]->getWarBird()->orientationMatrix));


	//make warbird targets... gets orientation matrices of warbirds targets
	std::vector<Target* > warBirdTargets;
	warBirdTargets.push_back(new Target(&models[3]->getMissileSite(0)->orientationMatrix, &models[3]->getMissileSite(0)->destroyed));
	warBirdTargets.push_back(new Target(&models[3]->getMissileSite(1)->orientationMatrix, &models[3]->getMissileSite(1)->destroyed));
	
	//make missile site target... gets orientation matrix of warbird
	std::vector<Target* > missileSitesTargets;
	missileSitesTargets.push_back(new Target(&models[1]->getWarBird()->orientationMatrix, &models[1]->getWarBird()->destroyed));

	//add missile to warbird
	models[2]->addWBMissile(new Missile(2, models[2]->getBoundingRadius(), 0, 
										&models[1]->getWarBird()->orientationMatrix,	//warbird orientation matrix to trace wb
										warBirdTargets),								//targets
										models[1]->getWarBird());
	//add missiles to missile sites
	models[3]->addMSMissile(new Missile(2, models[2]->getBoundingRadius(), 2,
										&models[0]->getPlanet(1)->orientationMatrix,	//planets orientation matrix to trace ms
										missileSitesTargets),							//targets
										models[3]->getMissileSite(0));
	
	models[3]->addMSMissile(new Missile(2, models[2]->getBoundingRadius(), 2,
										&models[0]->getPlanet(4)->orientationMatrix,	//planets orientation matrix to trace ms
										missileSitesTargets),							//targets
										models[3]->getMissileSite(1));

	//test collisions with different models
	collision = new Collision(models[1]->getWarBird(), models[3]->getMissileSites(), models[0]->getPlanets());
		
	//add cameras
	cameras[0] = new Camera("Front", glm::vec3(0.0f, 10000.0f, 20000.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
	cameras[1] = new Camera("Top", glm::vec3(0.0f, 20000.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
	cameras[2] = models[0]->getPlanet(1)->getCamera();  //unum cam
	cameras[3] = models[0]->getPlanet(2)->getCamera();  //duo cam
	cameras[4] = models[1]->getWarBird()->getCamera();  //add warbird
	
	//set initial camera
	camera = cameras[0];
	camera->setCameraOn(true);
	printf("%d Shapes created \n", numPlanets);

	//set warbirds warp reference positions
	warpPos.push_back(models[0]->getPlanet(1));
	warpPos.push_back(models[0]->getPlanet(2));
	models[1]->getWarBird()->setWarpPos(warpPos);

	//-----------------SKY BOX-----------------//

	// set up the indices buffer
	glGenBuffers(1, &textIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, textIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// set up the vertex attributes
	glGenVertexArrays(1, &textVao);
	glBindVertexArray(textVao);

	//  initialize a buffer object
	glGenBuffers(1, &textBuf);	
	glBindBuffer(GL_ARRAY_BUFFER, textBuf);
	glBufferData(GL_ARRAY_BUFFER, sizeof(textVertices) + sizeof(textCoords), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(textVertices), textVertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(textVertices), sizeof(textCoords), textCoords);

	// set up vertex arrays
	TexturePosition = glGetAttribLocation(shaderProgram, "vPosition");
	glVertexAttribPointer(TexturePosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(TexturePosition);

	vTextCoord = glGetAttribLocation(shaderProgram, "vTextCoord");
	glVertexAttribPointer(vTextCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(textVertices)));
	glEnableVertexAttribArray(vTextCoord);

	// Set the intial texture indicator to false
	// This variable indicates when the texture is being drawn
	IsTexture = glGetUniformLocation(shaderProgram, "IsTexture");
	glUniform1ui(IsTexture, false);

	// load texture
	texture = loadRawTexture(texture, textureFile, width, height);
	if (texture != 0) {
		texture = glGetUniformLocation(shaderProgram, "Texture");
		printf("texture file, read, texture %1d generated and bound  \n", Texture);
	}
	else { // texture file loaded
		std::cout << "texture not loaded: " << textureFile << std::endl;
	}

	// ruber render bool
	ruber = glGetUniformLocation(shaderProgram, "ruber");
	glUniform1ui(ruber, false);

	planet = glGetUniformLocation(shaderProgram, "planet");
	glUniform1ui(planet, false);
	//color
	color = glGetUniformLocation(shaderProgram, "planetColor");

	lastTime = glutGet(GLUT_ELAPSED_TIME);  // get elapsed system time


}

// set viewport and projectionMatrix on window resize events()automatically calls display
void reshape(int width, int height) {
	glViewport(0, 0, width, height);
	aspectRatio = (GLfloat)width / (GLfloat)height;

	projectionMatrix = glm::perspective(glm::radians(48.0f), aspectRatio, 1.0f, 80000.0f);
	printf("reshape: width = %4d height = %4d aspect = %5.2f \n", width, height, aspectRatio);
}

//return updates per second
int updatesPerSecond()
{
	return 1000 / timerDelay;
}

// update and display animation state in window title
void updateTitle() {
	sprintf(titleStr, "Warbird %d Unum %d Secundus %d U/S %d F/S %d %s   %s",
		models[1]->getWarBird()->missileCount,			//wb missile count
		models[3]->getMissileSite(0)->missileCount,	//unum missile count
		models[3]->getMissileSite(1)->missileCount,	//secundus missile count
		updatesPerSecond(), 
		framesPerSecond, 
		camera->getCameraName(),
		models[1]->getWarBird()->cadetStatus());		//cadets status
	glutSetWindowTitle(titleStr);
}

/* Renders all the models*/
void display(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// update model matrix, set MVP, draw
	viewMatrix = camera->getViewMatrix();

	vpMatrix = projectionMatrix * viewMatrix;
	//------------LIGHTING------------//
	Light point = ruberLight();
	glUniform3fv(glGetUniformLocation(shaderProgram, "RuberLight.position"), 1, glm::value_ptr(getPosition(projectionMatrix*viewMatrix * models[0]->getPlanet(0)->getModelMatrix())));
	glUniform3fv(glGetUniformLocation(shaderProgram, "RuberLight.direction"), 1, glm::value_ptr(point.direction));
	glUniform3fv(glGetUniformLocation(shaderProgram, "RuberLight.ambient"), 1, glm::value_ptr(point.ambient));
	glUniform3fv(glGetUniformLocation(shaderProgram, "RuberLight.diffuse"), 1, glm::value_ptr(point.diffuse));
	glUniform3fv(glGetUniformLocation(shaderProgram, "RuberLight.specular"), 1, glm::value_ptr(point.specular));
	glUniform1f(glGetUniformLocation(shaderProgram, "RuberLight.attenuation"), point.attenuation);
	glUniform1i(glGetUniformLocation(shaderProgram, "RuberLight.on"), ruberEnabled);

	Light ambient = ambientLight();
	glUniform3fv(glGetUniformLocation(shaderProgram, "AmbientLight.position"), 1, glm::value_ptr(getPosition(projectionMatrix*viewMatrix *models[0]->getPlanet(0)->getModelMatrix())));
	glUniform3fv(glGetUniformLocation(shaderProgram, "AmbientLight.direction"), 1, glm::value_ptr(ambient.direction));
	glUniform3fv(glGetUniformLocation(shaderProgram, "AmbientLight.ambient"), 1, glm::value_ptr(ambient.ambient));
	glUniform3fv(glGetUniformLocation(shaderProgram, "AmbientLight.diffuse"), 1, glm::value_ptr(ambient.diffuse));
	glUniform3fv(glGetUniformLocation(shaderProgram, "AmbientLight.specular"), 1, glm::value_ptr(ambient.specular));
	glUniform1f(glGetUniformLocation(shaderProgram, "AmbientLight.attenuation"), ambient.attenuation);
	glUniform1i(glGetUniformLocation(shaderProgram, "AmbientLight.on"), ambientEnabled);

	//render planets
	for (int j = 0; j < numPlanets; j++) {
		glUniform1ui(planet, true);

		if (models[0]->getPlanet(j)->planetName == "Ruber")
		{
			glUniform1ui(ruber, true);

		}
		else {
			glUniform1ui(ruber, false);
		}

		glUniform4fv(glGetUniformLocation(shaderProgram, "planetColor"), 1, glm::value_ptr(models[0]->getPlanet(j)->color));

		//glUniform4fv(color, 1, GL_FALSE, glm::value_ptr(models[0]->getPlanet(j)->color));

		modelMatrix = models[0]->getPlanet(j)->getModelMatrix();

		//pass mv uniform
		mvMatrix = viewMatrix * modelMatrix;
		glUniformMatrix4fv(MV, 1, GL_FALSE, glm::value_ptr(mvMatrix));

		//pass normal
		glUniformMatrix3fv(Normal, 1, GL_FALSE, glm::value_ptr(glm::mat3(mvMatrix)));

		//pass mvp uniform
		ModelViewProjectionMatrix = projectionMatrix * mvMatrix;
		glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(ModelViewProjectionMatrix));
		glBindVertexArray(VAO[0]);
		glDrawArrays(GL_TRIANGLES, 0, 264 * 3);

		//pass normal uniform
	}
	glUniform1ui(planet, false);

	//render warbird
	modelMatrix = models[1]->getWarBird()->getModelMatrix();
	ModelViewProjectionMatrix = vpMatrix * modelMatrix;
	glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(ModelViewProjectionMatrix));
	glBindVertexArray(VAO[1]);
	glDrawArrays(GL_TRIANGLES, 0, 132 * 3);
	
	//render wb missiles
	for (int j = 0; j < models[2]->getMissiles().size(); j++) {
		modelMatrix = models[2]->getMissile(j)->getModelMatrix();
		ModelViewProjectionMatrix = vpMatrix * modelMatrix;
		glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(ModelViewProjectionMatrix));
		glBindVertexArray(VAO[2]);
		glDrawArrays(GL_TRIANGLES, 0, 552 * 3);
	}

	//render missile site missiles
	for (int j = 0; j < models[3]->getMissiles().size(); j++) {
		modelMatrix = models[3]->getMissile(j)->getModelMatrix();
		ModelViewProjectionMatrix = vpMatrix * modelMatrix;
		glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(ModelViewProjectionMatrix));
		glBindVertexArray(VAO[4]);
		glDrawArrays(GL_TRIANGLES, 0, 552 * 3);
	}

	//render missile sites
	for (int i = 0; i < models[3]->getMissileSites().size(); i++) {
		modelMatrix = models[3]->getMissileSite(i)->getModelMatrix();
		ModelViewProjectionMatrix = vpMatrix * modelMatrix;
		glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(ModelViewProjectionMatrix));
		glBindVertexArray(VAO[3]);
		glDrawArrays(GL_TRIANGLES, 0, 88 * 3);
	}

	////////////////////////////////////////////////////////////////////////////////////////

	// Draw the Texture

	// indicate that texture is being drawn
	glUniform1ui(IsTexture, true);

	for (int i = 0; i < numberOfSquares; i++) {
		if (i > 1) {
			textRotation = PI / 2;
		}
		else {
			textRotation = 0.0f;
		}
		modelMatrix = glm::translate(translateSquare, skyTranslation[i])
						  * glm::rotate(squareRotation, textRotation, skyRotation[i]);

		ModelViewProjectionMatrix = vpMatrix * modelMatrix;
		glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(ModelViewProjectionMatrix));
		glBindVertexArray(textVao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, textIBO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
	}

	// indicate that texture is done being drawn
	glUniform1ui(IsTexture, false);

	glutSwapBuffers();
	frameCount++;
	// see if a second has passed to set estimated fps information
	currentTime = glutGet(GLUT_ELAPSED_TIME);  // get elapsed system time
	timeInterval = currentTime - lastTime;
	if (timeInterval >= 1000) {
		framesPerSecond = (int)(frameCount / (timeInterval / 1000.0f));
		lastTime = currentTime;
		frameCount = 0;
		updateTitle();
	}
}


// for use with Idle and intervalTimer functions 
void update(int i) {
	++updateCount;
	glutTimerFunc(timerDelay, update, 1);

	for (int i = 0; i < numPlanets; i++)
	{
		models[0]->getPlanet(i)->update();
	}
	models[1]->getWarBird()->update(); 
	models[3]->getMissileSite(0)->updateMissileSite();
	models[3]->getMissileSite(1)->updateMissileSite();
	collision->checkCollisions();

}

// Quit or set the view
void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 033: case 'q':  case 'Q': exit(EXIT_SUCCESS); break;
	case 'x': case 'X':  // top view
		if (cameraCount <= 0) { cameraCount = nCameras ; }//reset camera count to 
		camera->setCameraOn(false);
		camera = cameras[--cameraCount];
		camera->setCameraOn(true);
		break;
	case 'v': case 'V':  // top view
		if (cameraCount >= nCameras -1 ) { cameraCount = -1; }//reset camera count to 
		camera->setCameraOn(false);
		camera = cameras[++cameraCount];
		camera->setCameraOn(true);
		break;
	case 'g': case 'G':  // set gravity
		if (models[1]->getWarBird()->gravityOn()) 
		{
			models[1]->getWarBird()->setGravity(false);
		}
		else 
		{
			models[1]->getWarBird()->setGravity(true);
		}
		break;
	case 'f': case 'F':  // fire
		models[1]->getWarBird()->launchMissile();
		break;
	case 'w': case 'W':  // warp
		if(!models[1]->getWarBird()->destroyed)
			models[1]->getWarBird()->warp();
		break;
	case 't': case 'T':  // warp
		tqCount++;
		if (tqCount > 3) { tqCount = 0; }
		timerDelay = tq[tqCount];
		break;
	case 's': case 'S':  // warp
		speedCount++;
		if (speedCount > 2) { speedCount = 0; }
		models[1]->getWarBird()->speed = wbSpeed[speedCount];
		break;
	case 'a': case 'A':  // ambient
		if (!ambientEnabled) { ambientEnabled = true; }
		else				{ ambientEnabled = false; }
		break;
	case 'p': case 'P':  // point
		if (!ruberEnabled) { ruberEnabled = true; }
		else			   { ruberEnabled = false; }
		break;

	}
	updateTitle();
}

void specialKeys(int key, int x, int y) 
{	
	switch (key) {
	case GLUT_KEY_UP:
		if (glutGetModifiers() == GLUT_ACTIVE_CTRL) {//ship pitch down
				models[1]->getWarBird()->setPitch(1);
		} 
		else {	models[1]->getWarBird()->setMove(-1);}//ship foward
		break;
	case GLUT_KEY_DOWN: 
		if (glutGetModifiers() == GLUT_ACTIVE_CTRL) {//ship pitch up
				models[1]->getWarBird()->setPitch(-1); 
		} 
		else {	models[1]->getWarBird()->setMove(1);}//ship backward
		break;
	case GLUT_KEY_LEFT: 
		if (glutGetModifiers() == GLUT_ACTIVE_CTRL) {//ship rolls right
				models[1]->getWarBird()->setRoll(1); 
		}
		else {	models[1]->getWarBird()->setYaw(1);}//ship yaws left
		break;
	case GLUT_KEY_RIGHT: 
		if (glutGetModifiers() == GLUT_ACTIVE_CTRL) { //ship rolls left
				models[1]->getWarBird()->setRoll(-1); 
		}
		else {	models[1]->getWarBird()->setYaw(-1);}//ship yaws right
		break;
	}
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
# ifdef __MAC__
	// Can't change the version in the GLUT_3_2_CORE_PROFILE
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_3_2_CORE_PROFILE);
# endif
# ifndef __MAC__
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
# endif
	glutInitWindowSize(800, 600);
	// set OpenGL and GLSL versions to 3.3 for Comp 465/L, comment to see highest versions
# ifndef __MAC__
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
# endif
	glutCreateWindow("465 manyCubes Example {f, t, r} : front view");
	// initialize and verify glew
	glewExperimental = GL_TRUE;  // needed my home system 
	GLenum err = glewInit();
	if (GLEW_OK != err)
		printf("GLEW Error: %s \n", glewGetErrorString(err));
	else {
		printf("Using GLEW %s \n", glewGetString(GLEW_VERSION));
		printf("OpenGL %s, GLSL %s\n",
			glGetString(GL_VERSION),
			glGetString(GL_SHADING_LANGUAGE_VERSION));
	}
	// initialize scene
	init();
	// set glut callback functions
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialKeys);
	glutTimerFunc(timerDelay, update, 1);
	glutIdleFunc(display);
	glutMainLoop();
	printf("done\n");
	return 0;
}