#ifndef OBJECTREADER_H
#define OBJECTREADER_H

#include <iostream>
#include <string>
#include <assert.h>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "SimpleObject.hpp"
#include "Mesh.hpp"

struct Vertex
{
	glm::vec3 position;
	glm::vec3 color;
	//glm::vec3 normal;
};

class ObjectReader {
public:
	SimpleObject* loadSimpleOBJ(string filepath, Shader* shader, glm::vec3 color, glm::vec3 position, glm::vec3 scale);
	SimpleObject* createSquare(Shader* shader);
};


#endif
