#include "Camera.hpp"


#include <cstdlib> 

void Camera::moveMouse(double newX, double newY) {
	if (firstMouse) {
		mouseX = newX;
		mouseY = newY;
		firstMouse = false;
	}

	float offsetx = newX - mouseX;
	float offsety = mouseY - newY;
	mouseX = newX;
	mouseY = newY;

	offsetx *= moveSpeed;
	offsety *= moveSpeed;

	pitch += offsety;
	yaw += offsetx;

	glm::vec3 newFront;
	newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	newFront.y = sin(glm::radians(pitch));
	newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(newFront);
}

void Camera::updateView() {
	view = glm::lookAt(position, position + front, up);
}

void Camera::moveFoward() {
	position += front * moveSpeed;
}

void Camera::moveBack() {
	position -= front * moveSpeed;
}

void Camera::moveRight() {
	position += glm::normalize(glm::cross(front, up)) * moveSpeed;
}

void Camera::moveLeft() {
	position -= glm::normalize(glm::cross(front, up)) * moveSpeed;
}

void Camera::moveUp() {
	position += up * moveSpeed;
}

void Camera::moveDown() {
	position -= up * moveSpeed;
}

glm::vec3 Camera::getPosition() {
	return position;
}

glm::vec3 Camera::getFront() {
	return front;
}

glm::vec3 Camera::getUp() {
	return up;
}

glm::mat4 Camera::getView() {
	return view;
}
glm::mat4 Camera::getProjection() {
	return projection;
}

void Camera::setPosition(glm::vec3 newPosition) {
	position = newPosition;
}

void Camera::setFront(glm::vec3 newFront) {
	front = newFront;
}

void Camera::setUp(glm::vec3 newUp) {
	up = newUp;
}

void Camera::setView(glm::mat4 newView) {
	view = newView;
}
void Camera::setProjection(glm::mat4 newProjection) {
	projection = newProjection;
}