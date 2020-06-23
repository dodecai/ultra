#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Ultra {

OrthographicCamera::OrthographicCamera(float left, float right, float buttom, float top):
	ProjectionMatrix { glm::ortho(left, right, buttom, top, -1.0f, 1.0f) },
	ViewMatrix { 1.0f } {
	ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
}

void OrthographicCamera::SetProjection(float left, float right, float buttom, float top) {
	ProjectionMatrix = { glm::ortho(left, right, buttom, top, -1.0f, 1.0f) };
	ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
}

void OrthographicCamera::CalculateViewMatrix() {
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), Position) *
		glm::rotate(glm::mat4(1.0f), glm::radians(Rotation), glm::vec3(0, 0, 1));
	ViewMatrix = glm::inverse(transform);
	ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
}

}
