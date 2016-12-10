/*
Camera.hpp
Class that defines the cameras eye, at, and up vectors.
Comp465 Computer Graphics System and Design
Project Phase 1.
Christopher Bass
10/09/2016
*/
#pragma once
# include <string>

class Camera
{
private:
	const char* cameraName;
	glm::vec3 eye;
	glm::vec3 at;
	glm::vec3 up;
	bool dynamicCamera;
	bool cameraOn;
public:
	Camera() {}
	Camera(const char* camName) { cameraName = camName; }
	Camera(const char* camName, glm::vec3 eye, glm::vec3 at, glm::vec3 up)
	{
		cameraName = camName;
		this->eye = eye;
		this->at = at;
		this->up = up;	
	}

	void Camera::setEye(glm::vec3 eye) 
	{
		this->eye = eye;
	}

	void Camera::setAt(glm::vec3 at)
	{
		this->at = at;
	}

	void Camera::setUp(glm::vec3 up)
	{
		this->up = up;
	}

	bool Camera::setIsDynamic()
	{
		return dynamicCamera;
	}

	const char* Camera::getCameraName()
	{
		return cameraName;
	}

	void Camera::isDynamic(bool dynamicCam)
	{
		this->dynamicCamera = dynamicCam;
	}

	glm::mat4 Camera::getViewMatrix()
	{
		return glm::lookAt(eye, at, up);
	}

	bool isCameraOn()
	{
		return cameraOn;
	}

	void setCameraOn(bool camOn)
	{
		cameraOn = camOn;
	}
	void updateCamera()
	{}
};

