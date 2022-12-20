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
			.applicationVersion = { 1, 0, 0 }
	};
	GLFW3WindowPtr window = std::make_unique<GlfW3>(info.applicationName, 1280, 720);
	VulkanRenderer engine(std::move(info), *window.get());

	float fractionChangeX = 0.0f;
	float fractionChangeY = 0.0f;
	float aspect = float(window->GetWidth() / window->GetHeight());
	double mouseX, mouseY;
	bool leftMouseButtonPressed = false;
	bool rightMouseButtonPressed = false;
	bool middleMouseButtonPressed = false;
	double cx = 0;
	double cy = 0;
	CameraViewer cameraViewer({ 5.f, 5.f, 5.f }, { 0.f, 0.f, 0.f }, { 0.0f, 1.0f, 0.0f }, 45.f, aspect);

	window->RegisterCursorPosCallback([&](AWindow<GLFWwindow>& window, double x, double y)
	{
		cx = x;
		cy = y;
	});
	window->RegisterMouseButtonCallback([&](AWindow<GLFWwindow>& window, int button, int action, int mods)
	{
		leftMouseButtonPressed = action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_LEFT;
		rightMouseButtonPressed = action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_RIGHT;
		middleMouseButtonPressed = action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_MIDDLE;
	});

	Camera camera
			{
					.view = glm::lookAt(glm::vec3(5.f,5.f,5.f), glm::vec3(0.0f, 0.0f, 0.0f), glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f))),
//					.view = glm::translate(glm::mat4(1.f), { 5, 5, 5 }),
					.proj = glm::perspective(glm::radians(90.f), aspect, 0.001f, 2000.0f),
					.viewproj = camera.proj * camera.view * glm::mat4(1.f)
			};
	while (!window->ShouldClose())
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
		engine.DrawObject("./assets/monkey_flat.obj", {}, 0, 0, 0, 0, 0, 0, 1, 1, 1);
		engine.Draw(camera);
	}
}
