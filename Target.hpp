/*
Target.hpp
Comp465 Computer Graphics System and Design
Project Phase 2.
Christopher Bass
11/20/2016
*/
#pragma once


class Target
{
public:
	glm::mat4* targetMatrix;
	bool* destroyed;
public:

	Target(glm::mat4* targetMat, bool* dest)
	{
		targetMatrix = targetMat;
		destroyed = dest;
	}

};

