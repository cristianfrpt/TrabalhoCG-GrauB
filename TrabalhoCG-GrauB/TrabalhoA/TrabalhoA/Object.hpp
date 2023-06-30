#pragma once

#include <vector>
#include <stb_image.h>

#include "Mesh.hpp"

using namespace std;

class Object
{
public:
	Object() {}
	void initialize(string filePath, string* texPaths, Shader* shader, glm::vec3 position = glm::vec3(0.0, 0.0, 0.0), glm::vec3 scale = glm::vec3(1.0, 1.0, 1.0), float angle = 0.0, glm::vec3 axis = glm::vec3(0.0, 0.0, 1.0));
	void update();
	void draw();

	Shader* getShader();
	glm::vec3 getPosition();
	glm::vec3 getScale();
	glm::vec3 getAxis();
	float getAngle();


	void setShader(Shader*);
	void setPosition(glm::vec3);
	void setScale(glm::vec3);
	void setAxis(glm::vec3);
	void setAngle(float);

protected:
	void loadObj(string filePath);
	void loadQuadObj(string filePath);
	GLuint generateVAO(vector<GLfloat> vertbuffer, int& nVerts);
	GLuint loadTexture(string filePath);
	vector <Mesh> grupos;

	//Informações sobre as transformações a serem aplicadas no objeto
	glm::vec3 position;
	glm::vec3 scale;
	float angle;
	glm::vec3 axis;

	Shader* shader;
	string* texPaths;


};

