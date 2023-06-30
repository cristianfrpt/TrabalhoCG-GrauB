#include "SimpleObject.hpp"


void SimpleObject::update()
{
	glm::mat4 model = glm::mat4(1);
	model = glm::translate(model, position);
	model = glm::rotate(model, glm::radians(angle), axis);
	model = glm::scale(model, scale);
	shader->setMat4("model", glm::value_ptr(model));
}

void SimpleObject::draw() {
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, vertices);
	glBindVertexArray(0);
}


Shader* SimpleObject::getShader() {
	return shader;
}
glm::mat4 SimpleObject::getTransform() {
	return transform;
}
glm::vec3 SimpleObject::getPosition() {
	return position;
}
glm::vec3 SimpleObject::getScale() {
	return scale;
}
glm::vec3 SimpleObject::getAxis() {
	return axis;
}
float SimpleObject::getAngle() {
	return angle;
}

void SimpleObject::setShader(Shader* newShader) {
	shader = newShader;
}
void SimpleObject::setTransform(glm::mat4 newTransform) {
	transform = newTransform;
}
void SimpleObject::setPosition(glm::vec3 newPosition) {
	position = newPosition;
}
void SimpleObject::setScale(glm::vec3 newScale) {
	scale = newScale;
}
void SimpleObject::setAxis(glm::vec3 newAxis) {
	axis = newAxis;
}
void SimpleObject::setAngle(float newAngle) {
	angle = newAngle;
}
