/*
Planet.hpp
Class that defines the Planets attributes and
updates the planets position within space.
Comp465 Computer Graphics System and Design
Project Phase 1.
Christopher Bass
10/09/2016
*/
# pragma once
# include "WarBird.hpp"
# include "MissileSite.hpp"
# include <vector>
class Collision
{
public:
	/*warbird*/ 
	WarBird* warBird;
	/*missile sites*/
	std::vector<MissileSite*> missileSites;
	/*planet positions*/
	std::vector<Planet*> planets;
public:
	Collision(WarBird* wB, std::vector<MissileSite*> sites, std::vector<Planet*> p)
	{
		warBird = wB;
		missileSites = sites;
		planets = p;
	}

	void Collision::checkCollisions()
	{
		bool collided;

		//check if there is collision between warbird and planet
		for (int i = 0; i < planets.size(); i++)
		{	
			collided = collision(getPosition(warBird->orientationMatrix),			//warbird pos
								 getPosition(planets.at(i)->orientationMatrix),		//planet pos
								 warBird->radius+10, planets.at(i)->radius);		//warbird and planet radius
			if (collided)
			{
				warBird->destroy();
			}
		}

		//wb missile collided with missile sites
		for (int i = 0; i < missileSites.size(); i++) 
		{
			//checking if there is collision between missile and missile site
			collided = collision(getPosition(warBird->missile->orientationMatrix),			//missile pos
								 getPosition(missileSites.at(i)->orientationMatrix),		//missile site pos
								 warBird->missile->radius+15, missileSites.at(i)->radius);	//wb missile and missile site radius
			if (collided)
			{
				missileSites.at(i)->destroy();
				missileSites.at(i)->missile->destroy();
				warBird->missile->destroy();
				warBird->missile->hitTarget = true;
			}
		}	
		
		//checks missile sites missile collided with warbird
		for (int i = 0; i < missileSites.size(); i++)
		{
			if (missileSites.at(i)->missile != NULL) {
				//checking if there is collision between missile and missile site
				collided = collision(getPosition(warBird->orientationMatrix),				//wb missile pos
					getPosition(missileSites.at(i)->missile->orientationMatrix),			//missile site's missile pos
					warBird->missile->radius + 15, missileSites.at(i)->radius);				//wb and missile site's missile radius
				if (collided)
				{
					warBird->destroy();
					missileSites.at(i)->missile->destroy();
					missileSites.at(i)->missile->hitTarget = true;
				}
			}
		}
	}

	bool Collision::collision(glm::vec3 pos1, glm::vec3 pos2,float radius1, float radius2)
	{
		float length = distance(pos1, pos2);

		if (length < (radius1 + radius2))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
};
