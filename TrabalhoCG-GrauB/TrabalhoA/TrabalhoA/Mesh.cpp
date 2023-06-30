#include "Mesh.hpp"

void Mesh::initialize(GLuint VAO, int nVertices, Shader* shader, GLuint texID)
{
	this->VAO = VAO;
	this->nVertices = nVertices;
	this->shader = shader;
	this->texID = texID;
}

void Mesh::update()
{

}

void Mesh::draw()
{
	shader->setFloat("ka", ka);
	shader->setFloat("kd", kd);
	shader->setFloat("ks", ks);


	glBindTexture(GL_TEXTURE_2D, texID);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, nVertices);
	glBindVertexArray(0);
}
