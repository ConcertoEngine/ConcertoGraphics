//
// Created by arthur on 12/07/2022.
//
#define VMA_IMPLEMENTATION
#define VMA_VULKAN_VERSION 1000000
#define VKB_DEBUG
#define GLM_FORCE_LEFT_HANDED
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <iostream>
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "VulkanRenderer.hpp"
#include "CameraViewer.hpp"

using namespace Concerto::Graphics;


int main()
{
	RendererInfo info = {
			.applicationName = "Concerto Graphics",
			.applicationVersion = { 1, 0, 0 },
			.useImGUI = true,
			.width = 1280,
			.height = 720
	};
	GLFW3WindowPtr window = std::make_unique<GlfW3>(info.applicationName, 1280, 720);
	VulkanRenderer engine(std::move(info), *window.get());
	Concerto::Graphics::ImGUI* imGui = engine.GetImGUIContext();
	assert(imGui != nullptr);
	ImGuiIO& io = ImGui::GetIO();

	float fractionChangeX;
	float fractionChangeY;
	float aspect = float(window->GetWidth() / window->GetHeight());
	double mouseX, mouseY;
	bool leftMouseButtonPressed = false;
	bool rightMouseButtonPressed = false;
	bool middleMouseButtonPressed = false;
	double cx = 0;
	double cy = 0;
	CameraViewer cameraViewer({ 5.f, 5.f, 5.f }, { 0.f, 0.f, 0.f }, { 0.0f, 1.0f, 0.0f }, 45.f, aspect);
	Scene sceneParameters{};
	sceneParameters.gpuSceneData.ambientColor = { 0.1f, 0.1f, 0.1f, 1.f },
			sceneParameters.clearColor = { 0.1f, 0.1f, 0.1f, 1.f };
	engine.UpdateSceneParameters(sceneParameters);
	window->RegisterCursorPosCallback([&](AWindow<GLFWwindow>& window, double x, double y)
	{
		io.AddMousePosEvent(x, y);
		cx = x;
		cy = y;
	});
	window->RegisterMouseButtonCallback([&](AWindow<GLFWwindow>& window, int button, int action, int mods)
	{
		io.AddMouseButtonEvent(button, action == GLFW_PRESS);
		leftMouseButtonPressed = action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_LEFT;
		rightMouseButtonPressed = action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_RIGHT;
		middleMouseButtonPressed = action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_MIDDLE;
	});


	Camera camera
			{
					.view = glm::lookAt(glm::vec3(5.f, 5.f, 5.f), glm::vec3(0.0f, 0.0f, 0.0f),
							glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f))),
					.proj = glm::perspective(glm::radians(90.f), aspect, 0.001f, 2000.0f),
					.viewproj = camera.proj * camera.view * glm::mat4(1.f)
			};
	window->RegisterResizeCallback([&](AWindow<GLFWwindow>& window)
	{
		aspect = float(window.GetWidth() / window.GetHeight());
		camera.proj = glm::perspective(glm::radians(90.f), aspect, 0.001f, 2000.0f);
		camera.viewproj = camera.proj * camera.view * glm::mat4(1.f);
//		engine.Resize(window.GetWidth(), window.GetHeight());
	});
	while (!window->ShouldClose())
	{
		window->PopEvent();
		if (!ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow))
		{
			if (leftMouseButtonPressed)
			{
				fractionChangeX = static_cast<float>(cx - mouseX) / static_cast<float>(window->GetWidth());
				fractionChangeY = static_cast<float>(mouseY - cy) / static_cast<float>(window->GetHeight());
				cameraViewer.Rotate(fractionChangeX, fractionChangeY);
			}
			if (rightMouseButtonPressed)
			{
				fractionChangeX = static_cast<float>(cx - mouseX) / static_cast<float>(window->GetWidth());
				fractionChangeY = static_cast<float>(mouseY - cy) / static_cast<float>(window->GetHeight());
				cameraViewer.Translate(fractionChangeX, fractionChangeY);
			}
			if (middleMouseButtonPressed)
			{
				fractionChangeY = static_cast<float>(mouseY - cy) / static_cast<float>(window->GetHeight());
				cameraViewer.Zoom(fractionChangeY);
			}
			cameraViewer.UpdateCameraView(camera);
			mouseX = cx;
			mouseY = cy;
		}
		imGui->NewFrame();
		if (ImGui::ColorEdit3("Select clear color", &sceneParameters.clearColor[0]))
		{
			engine.UpdateSceneParameters(sceneParameters);
		}
		if (ImGui::ColorEdit3("Select ambiant color", &sceneParameters.gpuSceneData.ambientColor[0]))
		{
			engine.UpdateSceneParameters(sceneParameters);
		}
		engine.DrawObject("./assets/sponza/sponza.obj", {}, 0, 0, 0, 0, 0, 0, 0.1f, 0.1f, 0.1f);
//		engine.DrawObject("./assets/lost_empire.obj", "./assets/lost_empire-RGBA.png", 10, 0, 0, 0, 0, 0, 1, 1, 1);
		engine.Draw(camera);
	}
}
