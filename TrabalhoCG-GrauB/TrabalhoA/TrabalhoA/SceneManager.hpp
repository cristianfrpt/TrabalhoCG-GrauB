#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <iostream>

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.hpp"
#include "ObjectReader.hpp"
#include "SimpleObject.hpp"
#include "Object.hpp"
#include "Shader.hpp"
#include "Bezier.hpp"

using namespace std;

#define PIKACHU_ID 99
#define FLOOR_ID 0
#define LEFT_WALL_ID 1
#define RIGHT_WALL_ID 2
#define BACK_WALL_ID 3
#define FRONT_WALL_ID 4
#define CEILING_ID 5
#define WINDOW_ID 6
#define SKYBOX_ID 7
#define SUN_ID 8
#define BED_ID 9
#define WARDROBE_ID 10
#define CHAIR_ID 11


class SceneManager {
public:
	static const GLuint WIDTH = 1680;
	static const GLuint HEIGHT = 720;

	GLFWwindow* window;
	ObjectReader* reader;
	Object* objects;
	Bezier bezier;

	int curvePoints;
	int currentCurvePoint;
	int controlCurveAceleration=0;
	glm::vec3 currentSunPos;
	glm::vec3 startPosition;
	glm::vec3 startFront;
	glm::vec3 startUp;
	glm::mat4 startView;
	std::vector<glm::vec3> circlePoints;

	float fov;
	float zNear;
	float zFar;
	float cameraSpeed;

	void initialize();
	int run();
	void loadConfig(string filepath);
	void updateCamera();
	void updateObjects();
	void updateSun();
	void doTransformation();
	void initializeShader(Shader* shader, int width, int height);
	void initializeObjects();
	void initializeFloor();
	void initializeLeftWall();
	void initializeWindow();
	void initializeRightWall();
	void initializeBackWall();
	void initializeFrontWall();
	void initializePikachu();
	void initializeCeiling();
	void initializeBed();
	void initializeSkybox();
	void initializeSun();
	void initializeWardrobe();
	void initializeChair();
	glm::vec3 readVec3(string s);
	glm::mat4 readMat4View(string s);
	std::vector<glm::vec3> readCircle(string s);

	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	static void mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	
};
#endif