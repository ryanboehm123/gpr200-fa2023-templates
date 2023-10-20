//shader.cpp
#include "camera.h"
#include "transformations.h"

namespace rbLib {
	Camera::Camera() {
		position = { 0, 0, 5 };
		target = { 0, 0, 0 };
		fov = 60;
		aspectRatio = 1.5f;
		nearPlane = 0.1f;
		farPlane = 100;
		orthographic = false;
		orthoSize = 6;
	}

	ew::Mat4 Camera::ViewMatrix() {
		return rbLib::LookAt(position, target, ew::Vec3(0, 1, 0));
	}

	ew::Mat4 Camera::ProjectionMatrix() {
		if (orthographic) {
			return rbLib::Orthographic(orthoSize, aspectRatio, nearPlane, farPlane);
		}
		else {
			return rbLib::Perspective(fov, aspectRatio, nearPlane, farPlane);
		}
	}
}