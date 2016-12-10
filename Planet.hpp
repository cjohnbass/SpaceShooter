/*
Planet.hpp
Class that defines the Planets attributes and
updates the planets position within space.
Comp465 Computer Graphics System and Design
Project Phase 1.
Christopher Bass
10/09/2016
*/
#pragma once
#include "Shape.hpp"
#include "Camera.hpp"
# include <string>
# include <vector>
class Planet : public Shape
{
public:
	/*name of planet*/
	const char* planetName;
	/*time it takes to complete one orbit*/
	float radians;
	/*planets current position*/
	glm::vec3 parentPlanetPos;
	/*list of moons*/
	std::vector<Planet*> moons;
	/*does the planet orbit*/
	bool orbital;
	/*parents rotation matrix*/
	glm::mat4 parentsRotationMatrix;
	/*planets camera*/
	Camera* camera;
	glm::vec4 color;
public:
	Planet(const char* planetName, float planetsRadius, float modelRadius, float translation, float rotation, bool orbital, glm::vec4 color): Shape(planetsRadius, modelRadius)
	{	
		//setting names
		camera = new Camera(planetName);
		this->planetName = planetName;

		//set rotation rotation axis and amount of radians to rotate
		rotationAxis = glm::vec3(0, 1, 0);			
		radians = rotation;

		// set translation matrix (only translating x coordinate)
		translationMatrix = glm::translate(glm::mat4(),	glm::vec3(translation, 0, 0));

		parentPlanetPos = glm::vec3(0.0);
		this->orbital = orbital;
		this->color = color;
	}

	/*adds a moon to the planet*/
	void Planet::addMoon(Planet* moon)
	{
		moons.push_back(moon);
	}
	
	/*returns the modelmatrix of the planet*/
	void Planet::setModelMatrix()
	{
		if (orbital)
		{
			//translate to planets position and rotate by parents rotation matrix
			glm::mat4 parentsRotation = parentsRotationMatrix * translationMatrix;  
		    
			//matrix that subtracts the distance from parent planet(Duo) to ruber from this planet
			glm::mat4 translateBack = glm::translate(glm::mat4(1.0f), 
				glm::vec3(-parentPlanetPos.x, 0, -parentPlanetPos.z));

			//matrix that adds back the distance subtracted after rotation is performed
			glm::mat4 addBackMat = glm::translate(glm::mat4(1.0f),
				glm::vec3(parentPlanetPos.x, 0, parentPlanetPos.z));

			//perform transformation
			orientationMatrix = addBackMat * rotationMatrix * translateBack *  parentsRotation * scaleMatrix;
			//orientationMatrix = addBackMat * rotationMatrix * translationMatrix * scaleMatrix;
		}
		else
		{
			orientationMatrix = rotationMatrix * translationMatrix * scaleMatrix;
		}
	}
	/*returns the modelmatrix of the planet*/
	glm::mat4 Planet::getModelMatrix()
	{
		return orientationMatrix;
	}

	/*gets translation position planet position*/
	glm::vec3 Planet::getPlanetPosition()
	{
		return getPosition(orientationMatrix);
	}

	/*update planets position*/
	void Planet::update()
	{

		rotationMatrix = glm::rotate(rotationMatrix, radians, rotationAxis);
		setModelMatrix();
		
		//update moon's parent planet's position
		if (planetName != "Ruber")
			for(int i = 0; i < moons.size(); i++)
			{
				moons[i]->setParentPlanetPos(getPosition(orientationMatrix));
				moons[i]->parentsRotationMatrix = rotationMatrix;		
			}
		//update camera
		updateCamera();
	}

	void setParentPlanetPos(glm::vec3 newPos)
	{
		parentPlanetPos = newPos;
	}
	Planet & getPlanet(int id, std::vector<Planet> & objects)
	{
		return objects[id];
	}

	void Planet::updateCamera() 
	{
			camera->setEye(getPosition(rotationMatrix * translationMatrix * glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, -6000))));  
			camera->setAt(glm::vec3(getPosition(orientationMatrix)));
			camera->setUp(glm::vec3(0, 1, 0));   // camera's up is looking towards -Z vector
	}

	Camera * Planet::getCamera() 
	{
		return camera;
	}
};
