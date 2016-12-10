/*
Missle.hpp
Class that defines the missles attributes and
updates the missles position within space.
Comp465 Computer Graphics System and Design
Project Phase 2.
Christopher Bass
11/20/2016
*/
#pragma once
#include "Shape.hpp"
#include "Target.hpp"

# include <string>
# include <vector>

class Missile : public Shape
{
public:
	/*carriers orientation to get missiles pos*/
	glm::mat4* carrierOrientationMatrix;
	/*targetMatrix*/
	glm::mat4 targetMatrix;
	/*missiles state*/
	bool alive, smart, fired, hasTarget, hitTarget;
	/*number of missle's updates*/
	int numUpdates, maxUpdates;
	/*target's location*/
	std::vector<Target*> targetsPosition;
	/*multiple targets location*/
	glm::mat4 * target;
	/*shooters location*/
	glm::vec3  shooterPosition;
	/*missles distance Matrix*/
	glm::mat4 stepMatrix;
	/*missles step*/
	float step = 20;
	/*how much to trans up*/
	float translateUp;

public:
	Missile(float missleRadius, float modelRadius, float trans, glm::mat4 * carrierOrientMatrix, std::vector<Target*> tP) : Shape(missleRadius, modelRadius)
	{
		carrierOrientationMatrix = carrierOrientMatrix;			//missiles carrier matrices to follow									
		translateUp = trans;									//how much to trans up on top of planet
		setStepMat(step);										//how fast miss moves
		setMissileProperties();
		targetsPosition = tP;
	}

	void Missile::setMissileProperties()
	{
		alive = true;
		smart = false;		//not active
		fired = false;		//not fired yet
		hitTarget = false;
		hasTarget = false;	//doesnt have a target
		destroyed = false;
		numUpdates = 0;		//no updates 
		maxUpdates = 2000;	//missles has max 2000 updates
		orientationMatrix = scaleMatrix;
		traceCarrier();
	}

	glm::vec3 Missile::missilePosition()
	{
		return getPosition(orientationMatrix);
	}

	/*--------------------------update missile--------------------------*/

	/*determins what missile has done and what missile should do*/
	void Missile::update()
	{
		if (hasFired())
		{
			incrementMissileCount();				//update missile lifespan count															
			if (isSmart() && !hasTarget)			//missile is above 200 updates and has a target
				findTarget();						//compare which target is closest
			if (hasTarget)							//missile has found a target			
				if (!hitTarget && !dieNaturally())	//missile has not hit target or is not above 2000 updates
					orientToTarget();				//start moving toward target
				else
					detonateMissile();
			else
				fireStraight();
		}
		else
			traceCarrier();							//if havent fired stay in missile silo
	}

	/*finds closest target*/
	void Missile::findTarget()
	{
		float minLength, tempLength;
		glm::vec3 targetPosition;
		//compare other target positions against min length
		//and set new target if a length less than min is found
		minLength = 5000;

		for (int i = 0; i < targetsPosition.size(); i++)
		{
			if (*targetsPosition.at(i)->destroyed != true)
			{
				tempLength = abs(distance(getPosition(orientationMatrix), getPosition(*targetsPosition.at(i)->targetMatrix)));
				if (tempLength < minLength && tempLength < 5000)
				{
					minLength = tempLength;
					target = targetsPosition.at(i)->targetMatrix;
					targetPosition = getPosition(*targetsPosition.at(i)->targetMatrix);
					hasTarget = true;
				}

			}
		}
	}

	/*--------------------------missile movement properties--------------------------*/

	/*if missile has not been fired missiles track its carriers position*/
	void Missile::traceCarrier()
	{
		setPosition(getPosition(*carrierOrientationMatrix * glm::translate(glm::mat4(), glm::vec3(0, translateUp, 0)))); //set pos plus some delta
		setRight(glm::normalize(getRight(*carrierOrientationMatrix)));	//set right vec to normalized silo's right vec
		setUp(glm::normalize(getUp(*carrierOrientationMatrix)));		//set up vec to normalized silo's up vec
		setOut(glm::normalize(getOut(*carrierOrientationMatrix)));		//set out vec to normalized silo's out vec
		orientationMatrix = orientationMatrix * scaleMatrix;			//then scale back out
	}

	/*fires missile straight*/
	void Missile::fireStraight()
	{
		orientationMatrix = orientationMatrix * stepMatrix;
	}
	void Missile::orientToTarget()
	{
		translationMatrix = identity;
		orientTarget(target);
		orientationMatrix = orientationMatrix * stepMatrix * rotationMatrix;
	}
	/*--------------------------missile properties--------------------------*/

	/*is missle active*/
	bool Missile::isAlive()
	{
		if (numUpdates >= 2000 || destroyed)
		{
			alive = false;
		}
		else
		{
			alive = true;
		}

		//also must perform collision detection
		return alive;
	}
	
	/*is missle active*/
	bool Missile::isSmart()
	{
		if(hasFired())
		{
			if (numUpdates >= 200 && numUpdates < 2000) 
			{
				smart = true;
			}
			else 
			{
				smart = false;
			}
		}
		return smart;
	}

	/*did missile die from passing max updats*/
	bool Missile::dieNaturally()
	{
		if (numUpdates >= 2000) { return true; }
		else return false;
	}
	
	/*returns whether missle has fired or not*/
	bool Missile::hasFired()
	{
		return fired;
	}
	
	/*returns true if missle locked on target*/
	bool Missile::foundTarget()
	{
		return hasTarget;
	}
	
	/*activate missle*/
	void Missile::activateMissle()
	{
		fired = true;
	}
	
	/*fired missle*/
	void Missile::fireMissle()
	{
		fired = true;
	}
	
	/*detonate missle*/
	void Missile::detonateMissile()
	{
		alive = false;
		orientationMatrix = glm::mat4(0);
		numUpdates = 2000;
	}
	/*set missles number of updates*/
	void Missile::incrementMissileCount()
	{
		numUpdates = numUpdates + 1;						
	}

	/*pace of missile*/
	void setStepMat(float step)
	{
		// The distance the missile will travel
		glm::vec3 distance = glm::vec3(0, 0, -step);
		stepMatrix = glm::translate(glm::mat4(1), distance);
	}

	/*check if targets are destroyed*/
	bool Missile::allTargetsDestroyed()
	{	
		//return false if a target is not destroyed
		for (int i = 0; i < targetsPosition.size(); i++) 
		{
			if (*targetsPosition.at(i)->destroyed == false)
				return false;
		}
		return true;
	}


};