//shader.cpp
#include "shader.h"
#include "transformations.h"

Camera::ViewMatrix() {
	return rbLib::LookAt(position, target, ew::Vec3(0, 1, 0));
}

Camera::ProjectionMatrix() {
	if (orthographic) {
		return rbLib::Orthographic(orthoSize, aspectRatio, nearPlane, farPlane);
	}
	else {
		return rbLib::Perspective(fov, aspectRatio, nearPlane, farPlane);
	}
}