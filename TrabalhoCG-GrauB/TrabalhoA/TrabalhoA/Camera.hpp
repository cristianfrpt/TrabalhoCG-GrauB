#ifndef CAMERA_H
#define CAMERA_H

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {
public:
	Camera(glm::vec3 startPosition, glm::vec3 startFront, glm::vec3 startUp, float startFov, float startZNear, float startZFar, glm::mat4 startView, glm::mat4 startProjection, float cameraSpeed) {
		position = startPosition;
		front = startFront;
		up = startUp;
		fov = startFov;
		zNear = startZNear;
		zFar = startZFar;
		view = startView;
		projection = startProjection;
		moveSpeed = cameraSpeed;
		mouseX = 0;
		mouseY = 0;
		pitch = 0.0;
		yaw = -90.0;
		firstMouse = true;
	}

	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;

	float fov;
	float zNear;
	float zFar;
	float moveSpeed;
	float mouseX;
	float mouseY;
	float pitch;
	float yaw;

	bool firstMouse;

	glm::mat4 view;
	glm::mat4 projection;

	void moveFoward();
	void moveBack();
	void moveRight();
	void moveLeft();
	void moveUp();
	void moveDown();
	void moveMouse(double newX, double newY);
	void updateView();
	

	glm::vec3 getPosition();
	glm::vec3 getFront();
	glm::vec3 getUp();
	glm::mat4 getView();
	glm::mat4 getProjection();

	void setPosition(glm::vec3 newPosition);
	void setFront(glm::vec3 newFront);
	void setUp(glm::vec3 newUp);
	void setView(glm::mat4 newView);
	void setProjection(glm::mat4 newProjection);

};


#endif