#pragma once
# define __Windows__

struct Light {
	glm::vec3 position;
	glm::vec3 direction;  // (0, 0, 0) means point light; otherwise means directional

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	// Specular sharpness/power is fixed in the shader

	float attenuation;
	bool on;
};

