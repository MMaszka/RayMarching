#pragma once
#include "movement.h"


void Camera::processMouseMovement(GLFWwindow* window) {
	DeltaPos.x = float(xpos);
	DeltaPos.y = float(ypos);
	glfwGetCursorPos(window, &xpos, &ypos);

	// calculate mouse position change
	DeltaPos.x = float(DeltaPos.x - xpos) * sensivity;
	DeltaPos.y = float(ypos - DeltaPos.y) * sensivity;
}

void Camera::processInput(GLFWwindow* window) {
	// W A S D
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) { 
		velocity += *CamDir / 200.0f;
	};
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		velocity += *CamRight / 200.0f;
	};
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		velocity -= *CamDir / 200.0f;
	};
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		velocity -= *CamRight / 200.0f;
	};
	// SPACE CTRL
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		velocity += *CamUp / 200.0f;
	};
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
		velocity -= *CamUp / 200.0f;
	};

	// move camera, update speed 
	*CamPos += velocity;
	velocity /= 1.02f;

}

glm::vec4 Camera::quatMultip(glm::vec4 q1, glm::vec4 q2) { // multiplies two quaternions
	glm::vec4 NewQuat{};

	NewQuat.x = q1.x * q2.x  -  q1.y * q2.y  -  q1.z * q2.z  -  q1.w * q2.w;
	NewQuat.y = q1.x * q2.y  +  q1.y * q2.x  +  q1.z * q2.w  -  q1.w * q2.z;
	NewQuat.z = q1.x * q2.z  -  q1.y * q2.w  +  q1.z * q2.x  +  q1.w * q2.y;
	NewQuat.w = q1.x * q2.w  +  q1.y * q2.z  -  q1.z * q2.y  +  q1.w * q2.x;

	return NewQuat;
}
glm::mat3 Camera::quaternionRotationMatrix(glm::vec4 Quat) { // creates rotation matrix using quaternions 
	glm::mat3 matrix = {
		{1- 2 * (Quat.z * Quat.z + Quat.w * Quat.w),        2 * (Quat.y * Quat.z - Quat.w * Quat.x),		2 * (Quat.y * Quat.w + Quat.z * Quat.x)},
		{   2 * (Quat.y * Quat.z + Quat.w * Quat.x),    1 - 2 * (Quat.y * Quat.y + Quat.w * Quat.w),		2 * (Quat.z * Quat.w - Quat.y * Quat.x)},
		{   2 * (Quat.y * Quat.w - Quat.z * Quat.x),        2 * (Quat.z * Quat.w + Quat.y * Quat.x),	1 - 2 * (Quat.y * Quat.y + Quat.z * Quat.z)}
	};
	return matrix;
}