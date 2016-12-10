/*
MissileSite.hpp
Class that defines the missile sites
of the war.
Comp465 Computer Graphics System and Design
Project Phase 2.
Christopher Bass
11/20/2016
*/
#pragma once
# include "Missile.hpp"
# include <vector>


class MissileSite : public Shape
{
public:
	/*sites missile*/
	Missile *missile;
	/*planet site is located*/
	glm::mat4 *planetPosition;
	/*target*/
	glm::mat4 *warBird;
	/*missile count*/
	int missileCount;
	/*how much to translate site up*/
	float translation;
public:
	MissileSite(float siteRadius, float modelRadius, int count, float trans, glm::mat4* planetPos, glm::mat4* wbPos)
		: Shape(siteRadius, modelRadius)
	{
		missileCount = count;
		translation = trans;
		planetPosition = planetPos;
		warBird = wbPos;
	}
	/*updates missile sites position and its missiles */
	void MissileSite::updateMissileSite()
	{
		setPosition(glm::vec3(getPosition(*planetPosition).x, translation, getPosition(*planetPosition).z));
		if (missile != NULL && !destroyed) {
			if (!missile->hasFired())
				launchMissile();		
			updateMissile();			
		}
	}

	void MissileSite::updateMissile()
	{
		if (!missile->isAlive() && missileCount > 0)
		{
			missile->setMissileProperties();
		}
		else {
			missile->update();
		}
	}

	void MissileSite::launchMissile()
	{
		if (!missile->allTargetsDestroyed()) {
			if (abs(distance(getPosition(*warBird), getPosition(orientationMatrix))) < 5000)
			{
				missile->fireMissle();
				missile->orientTarget(warBird);
				missile->orientationMatrix = missile->orientationMatrix * missile->rotationMatrix;
				missile->fireStraight();
				--missileCount;
			}
		}
	}

	void MissileSite::addMissile(Missile * miss)
	{
		missile = miss;
	}
};
