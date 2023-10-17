//camera.h
#pragma once
#include "../ew/ewMath/mat4.h"
#include "../ew/ewMath/vec3.h"
#include "../ew/ewMath/ewMath.h"

namespace rbLib {
	struct Camera {
		Camera();
		ew::Vec3 position; // Camera body position
		ew::Vec3 target; // Position to look at
		float fov; // Vertical field of view in degrees
		float aspectRatio; // Screen width / Screen height
		float nearPlane; // near plane distance (+Z)
		float farPlane; // far plane distance (+Z)
		bool orthographic; // Perspective or orthographic?
		float orthoSize; // Height of orthographic frustum
		ew::Mat4 ViewMatrix(); // World->View
		ew::Mat4 ProjectionMatrix(); // View->Clip
	};
}