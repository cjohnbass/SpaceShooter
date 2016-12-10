/*
WarBird.hpp
Class that defines the WarBird attributes and
updates the WarBird position within space.
Comp465 Computer Graphics System and Design
Project Phase 1.
Christopher Bass
10/09/2016
*/
#pragma once
# include "Shape.hpp"
# include "Camera.hpp"
# include "Planet.hpp"
# include "Missile.hpp"
# include <string>
# include <vector>
using namespace std;

class WarBird : public Shape
{
public:
	/*name of WarBird*/
	const char* name;
	/*radius of WarBird*/
	float radius;
	/*gravity vector*/
	float gravityVector;
	/*warbird camera*/
	Camera* camera;
	/*list of missiles*/
	int missileCount;
	/*warbird current missile*/
	Missile* missile;
	/*how fast wb moves*/
	int speed;
	/*wb distance and rotational properties*/
	int step=0, pitch=0, yaw=0,roll=0;
	/*gravity on or off*/
	bool gravity;
	/*determines where to warp*/
	int warpCount;
	/*where warbird can warp*/
	std::vector<Planet*> warpPositions;
public:
	WarBird(const char* n, float warBirdRadius, float modelRadius, glm::vec3 translation, int p) : Shape(warBirdRadius, modelRadius)
	{
		camera = new Camera(name);
		name = n;
		gravity = false;

		// set translation matrix (only translating x coordinate)
		translationMatrix = glm::translate(glm::mat4(),	translation);
		speed = p;
		
		rotationAxis=glm::vec3(0, 1, 0);
		//set initial translation
		orientationMatrix = translationMatrix * rotationMatrix * scaleMatrix;
		missileCount = 9;
		warpCount = 2;
	}

	/*returns the modelmatrix of the WarBird*/
	glm::mat4 WarBird::getModelMatrix()
	{
		return orientationMatrix;
	}

	/*gets translation position planet position*/
	glm::vec3 WarBird::warBirdPosition()
	{
		return getPosition(orientationMatrix);
	}

	void WarBird::addMissle(Missile* miss)
	{
		missile = miss;
	}

	/*--------------------------update warbird--------------------------*/

	//update WarBird position
	void WarBird::update()
	{
		if (gravity == true) { applyGravity(); }
		
		//distance the warbird will travel
		glm::vec3 distanceTraveled = glm::vec3(0, 0, step * speed);

		//determine the rotation axis
		rotationAxis = glm::vec3(pitch, yaw, roll);

		//set rotationMatrix to identity
		rotationMatrix = identity;

		//rotate warbird
		if ((pitch != 0) || (yaw != 0) || (roll != 0))
		{
			rotationMatrix = glm::rotate(identity, .02f, rotationAxis);
		}

		// Get the tranlation matrix
		translationMatrix = glm::translate(identity, distanceTraveled);

		// Update the location of the object
		orientationMatrix = orientationMatrix * translationMatrix * rotationMatrix;
		// Reset the values 
		step = pitch = yaw = roll = 0;

		//update camera
		updateCamera();
		updateMissile();
	}

	/*--------------------------update missile--------------------------*/

	/*launches missile and decrement count*/
	void WarBird::launchMissile()
	{
		missile->fired = true;
		missileCount--;
	}

	/*updates or resets missile properties*/
	void WarBird::updateMissile()
	{
		if (!missile->isAlive() && missileCount > 0)
		{
			missile->setMissileProperties();			//reseting missile properties, old missile died
		}
		else {
			missile->update();
		}
	}

	/*--------------------------warbird movement properties--------------------------*/

	void WarBird::setMove(int s)
	{
		step = s;
	}

	void WarBird::setPitch(int p)
	{
		pitch = p;
	}

	void WarBird::setYaw(int y)
	{
		yaw = y;
	}

	void WarBird::setRoll(int r)
	{
		roll = r;
	}

	void WarBird::setGravity(bool g)
	{
		gravity = g;
	}

	bool WarBird::gravityOn()
	{
		return gravity;
	}

	/*--------------------------warbirds special properties--------------------------*/

	/*applies gravity to the warbird*/
	void WarBird::applyGravity()
	{	
		glm::vec3 warBirdPos = getPosition(orientationMatrix);
		glm::vec3 gravityVec;

		//get distance from origin
		float length = distance(warBirdPos, glm::vec3(0, 0, 0));

		//point vector to ruber
		warBirdPos = -1.0f * warBirdPos;

		//calculate gravity relative to position
		if (length != 0)
		{
			gravityVec = glm::normalize(warBirdPos) * (90000000 / (length * length));
		}
		else 
		{
			gravityVec = glm::vec3(0, 0, 0);
		}

		//set warbirds new position
		setPosition(getPosition(orientationMatrix) + gravityVec);
	}

	/*warp the warbird*/
	void WarBird::warp() 
	{
		Planet * planetPosition;
		
		//determine warp pos
		if (warpCount == 2)
		{
			planetPosition = warpPositions.at(0);
			warpCount = 1;
		}
		else
		{
			planetPosition = warpPositions.at(1);
			warpCount = 2;
		}

		//get and set translation matrix
		glm::mat4 translateToPosition = glm::translate(glm::mat4(), getPosition(planetPosition->rotationMatrix *
			glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, -8000.0f)) *
			planetPosition->translationMatrix));

		//set orientation to translated position
		orientationMatrix = translateToPosition;

		//once at position orient to target
		orientTarget(&planetPosition->orientationMatrix);

		orientationMatrix = translateToPosition * rotationMatrix * scaleMatrix;
	}

	/*--------------------------warbirds camera--------------------------*/

	void WarBird::updateCamera()
	{
		camera->setEye(getPosition(orientationMatrix * glm::translate(glm::mat4(), glm::vec3(0.0f, 300.0f, 1000.0f))));   // eye is 4000 up from planet
		camera->setAt(getPosition(orientationMatrix * glm::translate(glm::mat4(), glm::vec3(0.0f, 300.0f, 0.0f))));   // looking at planet  
		camera->setUp(getUp(orientationMatrix));   // camera's up is looking towards -Z vector
	}

	Camera * WarBird::getCamera()
	{
		return camera;
	}

	void WarBird::setWarpPos(std::vector<Planet*> warpPos)
	{
		warpPositions = warpPos;
	}
	//cadets status
	const char* cadetStatus() {
		char status[30];
		if (missile->allTargetsDestroyed())
		{
			return "CADET PASSES FLIGHT TRAINING";
		}
		else if (destroyed)
		{
			return"CADET RESIGNS FROM WAR COLLEGE";
		}
		else {
			return " ";
		}
	}
};