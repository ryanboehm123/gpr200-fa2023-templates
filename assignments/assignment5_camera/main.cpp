#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <ew/shader.h>
#include <ew/procGen.h>
#include <ew/transform.h>

#include <rb/camera.h>
#include <rb/transformations.h>

void framebufferSizeCallback(GLFWwindow* window, int width, int height);

void moveCamera(GLFWwindow* window, rbLib:: Camera* camera, rbLib::CameraControls* controls, float deltaTime);

void reset(rbLib::CameraControls* controls, rbLib::Camera* camera);

//Projection will account for aspect ratio!
const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

const int NUM_CUBES = 4;
ew::Transform cubeTransforms[NUM_CUBES];
rbLib::Camera camera;

int main() {
	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Camera", NULL, NULL);
	if (window == NULL) {
		printf("GLFW failed to create window");
		return 1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	if (!gladLoadGL(glfwGetProcAddress)) {
		printf("GLAD Failed to load GL headers");
		return 1;
	}

	//Initialize ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	//Enable back face culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//Depth testing - required for depth sorting!
	glEnable(GL_DEPTH_TEST);

	ew::Shader shader("assets/vertexShader.vert", "assets/fragmentShader.frag");
	
	//Cube mesh
	ew::Mesh cubeMesh(ew::createCube(0.5f));

	//Cube positions
	for (size_t i = 0; i < NUM_CUBES; i++)
	{
		cubeTransforms[i].position.x = i % (NUM_CUBES / 2) - 0.5;
		cubeTransforms[i].position.y = i / (NUM_CUBES / 2) - 0.5;
	}

	rbLib::Camera camera;
	rbLib::CameraControls cameraControls;

	float prevTime = 0; //Timestamp of previous frame

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		//Calculate deltaTime
		float time = (float)glfwGetTime(); //Timestamp of current frame
		float deltaTime = time - prevTime;
		prevTime = time;

		moveCamera(window, &camera, &cameraControls , deltaTime);
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		//Clear both color buffer AND depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Set uniforms
		shader.use();
		shader.setMat4("_View", camera.ViewMatrix());
		shader.setMat4("_Projection", camera.ProjectionMatrix());

		//TODO: Set model matrix uniform
		for (size_t i = 0; i < NUM_CUBES; i++)
		{
			//Construct model matrix
			shader.setMat4("_Model", cubeTransforms[i].getModelMatrix());
			cubeMesh.draw();
		}

		//Render UI
		{
			ImGui_ImplGlfw_NewFrame();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("Settings");
			ImGui::Text("Camera");
			ImGui::DragFloat3("Position", &camera.position.x, 0.05f);
			ImGui::DragFloat3("Target", &camera.target.x, 0.05f);
			ImGui::Checkbox("Orthographic", &camera.orthographic);
			ImGui::DragFloat("Ortho Height", &camera.orthoSize);
			ImGui::SliderFloat("FOV", &camera.fov, 0.00f, 180.00f);
			ImGui::DragFloat("Near Plane", &camera.nearPlane);
			ImGui::DragFloat("Far Plane", &camera.farPlane);
			ImGui::Text("Camera Controller");
			ImGui::Text("Yaw: %f", cameraControls.yaw);
			ImGui::Text("Pitch: %f", cameraControls.pitch);
			ImGui::DragFloat("Move Speed", &cameraControls.moveSpeed);
			if (ImGui::Button("Reset")) {
				reset(&cameraControls, &camera);
			}
			ImGui::End();
			
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void moveCamera(GLFWwindow* window, rbLib::Camera* camera, rbLib::CameraControls* controls, float deltaTime) {
	//If right mouse is not held, release cursor and return early
	if (!glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2)) {
		//Release cursor
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		controls->firstMouse = true;
		return;
	}

	//GLFW_CURSOR_DISABLED hides the cursor, but the position will still be changed as we move our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//Get screen mouse position this frame
	double mouseX, mouseY;
	glfwGetCursorPos(window, &mouseX, &mouseY);

	//If we just started right clicking, set prevMouse values to current position
	//This prevents a bug where the camera moves as soon as we click
	if (controls->firstMouse) {
		controls->firstMouse = false;
		controls->prevMouseX = mouseX;
		controls->prevMouseY = mouseY;
	}

	//Get mouse position delta for this frame
	float deltaMouseX = (mouseX - controls->prevMouseX) * controls->mouseSensitivity;
	float deltaMouseY = (mouseY - controls->prevMouseY) * controls->mouseSensitivity;

	//Add to yaw and pitch
	controls->yaw += deltaMouseX;
	controls->pitch -= deltaMouseY;

	//Clamp pitch between -89 and 89 degrees
	if (controls->pitch > 89) {
		controls->pitch = 89;
	}
	else if (controls->pitch < -89) {
		controls->pitch = -89;
	}

	//Remember previous mouse position
	controls->prevMouseX = mouseX;
	controls->prevMouseY = mouseY;

	//Construct forward vector using yaw and pitch. Don't forgot to convert to radians
	ew::Vec3 forward = { sin(ew::Radians(controls->yaw)) * cos(ew::Radians(controls->pitch)), sin(ew::Radians(controls->pitch)),
		-cos(ew::Radians(controls->yaw)) * cos(ew::Radians(controls->pitch)) };

	//By setting target to a point in front of the camera along its forward direction, our LookAt will be updated accordingly when rendering
	camera->target = camera->position + forward;

	ew::Vec3 right = ew::Normalize(ew::Cross(forward, { 0,1,0 }));
	ew::Vec3 up = ew::Normalize(ew::Cross(right, forward));

	//TODO: Keyboard controls for moving along forward, back, right, left, up, and down. See Requirements for key mapping.
	//EXAMPLE: Moving along forward axis if W is held.
	//Note that this is framerate dependent, and will be very fast until you scale by deltaTime. See the next section.
	if (glfwGetKey(window, GLFW_KEY_W)) {
		camera->position += forward * controls->moveSpeed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_A)) {
		camera->position += -right * controls->moveSpeed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_S)) {
		camera->position += -forward * controls->moveSpeed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_D)) {
		camera->position += right * controls->moveSpeed * deltaTime;
	}

	//Setting camera.target should be done after changing position. Otherwise, it will use camera.position from the previous frame and lag behind
	camera->target = camera->position + forward;
}

void reset(rbLib::CameraControls* controls, rbLib::Camera* camera) {
	controls->yaw = 0;
	controls->pitch = 0;
	controls->mouseSensitivity = 0.1f;
	controls->firstMouse = true;
	controls->moveSpeed = 5.0f;

	camera->position = { 0, 0, 5 };
	camera->target = { 0, 0, 0 };
	camera->fov = 60;
	camera->aspectRatio = 1.5;
	camera->nearPlane = 0.1;
	camera->farPlane = 100;
	camera->orthographic = false;
	camera->orthoSize = 6;
}