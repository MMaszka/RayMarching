#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
	float sensivity = 0.005f;
	glm::vec2 Offset{};
	glm::vec3 velocity{};
public:
	double xpos{}, ypos{};
	glm::vec2 DeltaPos{};
	glm::vec3* CamDir{},* CamPos{}, * CamUp{}, * CamRight{};
	void processInput(GLFWwindow* window);
	void processMouseMovement(GLFWwindow* window);
	glm::vec4 quatMultip(glm::vec4 q1, glm::vec4 q2);
	glm::mat3 quaternionRotationMatrix(glm::vec4 quaternion);
};
