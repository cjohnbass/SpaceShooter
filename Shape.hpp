#pragma once

# define __Windows__
# ifndef __INCLUDES465__
# include "../includes465/include465.hpp"
# define __INCLUDES465__
# endif
#include <iostream>

using namespace std;
class Shape
{
public:
	int id;
	glm::mat4 rotationMatrix;
	glm::mat4 scaleMatrix;
	glm::mat4 translationMatrix;
	glm::mat4 orientationMatrix;
	glm::mat4 identity;
	glm::vec3 rotationAxis;
	bool destroyed;
	float radius;

public:

	Shape()
	{
		//initialize scale, rotation, translation, and orientation to identity matrix
		glm::mat4 identity = glm::mat4(1.0);
		rotationMatrix = identity;
		scaleMatrix = identity;
		translationMatrix = identity;
		orientationMatrix = identity;
		destroyed = false;
	}

	Shape(float r, float boundingRadius)
	{
		//initialize scale, rotation, translation, and orientation to identity matrix
		radius = r;
		scale(radius, boundingRadius);
	}

	void scale(float radius, float boundingRadius)
	{
		glm::vec3 scale = glm::vec3(radius * 1.0f / boundingRadius);
		this->scaleMatrix = glm::scale(glm::mat4(1.0), glm::vec3(boundingRadius));
		this->scaleMatrix = glm::scale(scaleMatrix, scale);
	}

	void setO(glm::mat4 mat)
	{
		orientationMatrix = mat;
	}

	void setPosition(glm::vec3 vec)
	{
		orientationMatrix[3][0] = vec[0];
		orientationMatrix[3][1] = vec[1];
		orientationMatrix[3][2] = vec[2];
	}

	void setOut(glm::vec3 vec)
	{
		orientationMatrix[2][0] = vec[0];
		orientationMatrix[2][1] = vec[1];
		orientationMatrix[2][2] = vec[2];
	}

	void setRight(glm::vec3 vec)
	{
		orientationMatrix[0][0] = vec[0];
		orientationMatrix[0][1] = vec[1];
		orientationMatrix[0][2] = vec[2];
	}

	void setUp(glm::vec3 vec)
	{
		orientationMatrix[1][0] = vec[0];
		orientationMatrix[1][1] = vec[1];
		orientationMatrix[1][2] = vec[2];
	}

	glm::mat4 getModelMatrix() { return orientationMatrix; }

	void destroy() {
		orientationMatrix = glm::mat4(0);
		destroyed = true;
	}

	void orientTarget(glm::mat4* target)
	{
		// Get the Missiles at Vector and the Vector from the missile to the target
		glm::vec3 targetVector = getPosition(*target) - getPosition(orientationMatrix);
		glm::vec3 missileVector = getIn(orientationMatrix);

		// Normalize the vectors
		targetVector = glm::normalize(targetVector);
		missileVector = glm::normalize(missileVector);

		// do not do any rotations if two vectors are colinear
		if (!(colinear(missileVector, targetVector, 0.1))) {

			//rotation axis the missile will be rotating about
			rotationAxis = glm::cross(missileVector, targetVector);
			float rotationAxisDirection = rotationAxis.x + rotationAxis.y + rotationAxis.z;
			float rotationAmount;

			// determine direction of Rotation.
			rotationAmount = (2 * PI) + glm::acos(glm::dot(missileVector, targetVector));

			//determine rotation matrix
			rotationMatrix = glm::rotate(identity, rotationAmount, rotationAxis);
		}	
	}
	void update() {}
  };
