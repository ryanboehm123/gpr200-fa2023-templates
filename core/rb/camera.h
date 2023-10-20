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

	struct CameraControls {
		double prevMouseX, prevMouseY; // Mouse position from previous frame
		float yaw = 0, pitch = 0; // How fast to turn with mouse
		float mouseSensitivity = 0.1f; // How fast to turn with mouse
		bool firstMouse = true; // Flag to store initial mouse position
		float moveSpeed = 5.0f; // How fast to move with arrow keys (M/S)
	};
}