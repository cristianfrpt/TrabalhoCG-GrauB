#ifndef SIMPLEOBJECT_H
#define SIMPLEOBJECT_H

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.hpp"


class SimpleObject {
public:
	SimpleObject(Shader* shader, int vertices, GLuint VAO, glm::vec3 position, glm::vec3 scale) {
		this->shader = shader;
		this->VAO = VAO;
		this->vertices = vertices;
		this->position = position;
		this->scale = scale;
		this->axis = glm::vec3(0.0,0.0,1.0);
		this->angle = 0.0;

	}
		
	Shader* shader;

	glm::mat4 transform;
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 axis;
	

	float angle;
	int vertices;
	GLuint VAO;

	void update();
	void draw();

	Shader* getShader();
	glm::mat4 getTransform();
	glm::vec3 getPosition();
	glm::vec3 getScale();
	glm::vec3 getAxis();
	float getAngle();


	void setShader(Shader*);
	void setTransform(glm::mat4);
	void setPosition(glm::vec3);
	void setScale(glm::vec3);
	void setAxis(glm::vec3);
	void setAngle(float);

};

#endif
