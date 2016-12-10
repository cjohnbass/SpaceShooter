/*
TriModel.hpp
Class that loads the tri models into the buffers
and binds an vao to it. TriModel class represents
either a list of Planets, a WarBird, or a list of
missles.
Comp465 Computer Graphics System and Design
Project Phase w.
Christopher Bass
10/09/2016
*/
#pragma once
#include "Planet.hpp"
#include "Missile.hpp"
#include "WarBird.hpp"
#include "MissileSite.hpp"
class TriModel
{
private:
	//tri models file name
	const char* modelFileName;
	//number of vertices in tri model
	int numVertices;
	//max radius of tri model
	float  boundingRadius;
	//shader position 
	GLuint vPosition = 0;
	//shader color 
	GLuint vColor = 0;
	//shader normal 
	GLuint vNormal = 0;
	//if model is planet
	std::vector<Planet*> planets;
	//if model is missle
	std::vector<Missile*> missiles;
	//if model is warbird
	WarBird * warBird;
	//missile sites
	std::vector<MissileSite*> sites;


public:
	TriModel(char* fileName, int numVertices, GLuint vao, GLuint buffer, GLuint shaderProgram)
	{ 
		modelFileName = fileName; 
		this->numVertices = numVertices;
		boundingRadius = loadModelBuffer(fileName, numVertices, vao, buffer,
			shaderProgram, vPosition, vColor, vNormal,
			"vPosition", "vColor", "vNormal");
	}

	//------------PLANET METHODS------------//
	/*adds planet to vectors*/
	void TriModel::addPlanet(Planet* planet)
	{
		planets.push_back(planet);
	}
	/*adds moons(Planet) to a specific planet within the planets
	vector.*/
	void TriModel::addMoon(int planetIndex, int moonIndex)
	{
		planets.at(planetIndex)->addMoon(planets.at(moonIndex));
	}
	/*returns a planet from the planets vector*/
	Planet * TriModel::getPlanet(int index)
	{
		return planets.at(index);
	}

	/*returns a missile site object*/
	std::vector<Planet*> TriModel::getPlanets()
	{
		return planets;
	}
	//------------WARBIRD METHODS------------//
	/*sets the warbird object*/
	void TriModel::setWarBird(WarBird * wB)
	{
		this->warBird = wB;
	}
	/*returns a warbird object*/
	WarBird * TriModel::getWarBird()
	{
		return warBird;
	}

	//------------MISSILESITE METHODS------------//
	/*sets the warbird object*/
	void TriModel::addMissileSite(MissileSite* site)
	{
		sites.push_back(site);
	}
	/*returns a missile site object*/
	MissileSite* TriModel::getMissileSite(int index)
	{
		return sites.at(index);
	}
	/*returns a missile site object*/
	std::vector<MissileSite*> TriModel::getMissileSites()
	{
		return sites;
	}

	//------------MISSLE METHODS------------//

	/*parameter takes a missle and warbird object. The missle
	is added to that specific warbird object*/
	void TriModel::addWBMissile(Missile* miss,WarBird* wB)
	{
		missiles.push_back(miss);
		wB->addMissle(miss);
	}

	void TriModel::addMSMissile(Missile* miss, MissileSite* mS)
	{
		missiles.push_back(miss);
		mS->addMissile(miss);
	}
	
	/*returns missle from missile vector*/
	Missile * TriModel::getMissile(int index)
	{
		return missiles.at(index);
	}

	/*returns missles from missile vector*/
	std::vector<Missile*> TriModel::getMissiles()
	{
		return missiles;
	}
	//------------ALL MODELS METHODS------------//

	/*returns tri file name*/
	const char* TriModel::getFileName()
	{
		return modelFileName;
	}

	/*returns bounding radius of tri model*/
	float TriModel::getBoundingRadius()
	{
		return boundingRadius;
	}

};