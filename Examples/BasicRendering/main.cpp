//
// Created by arthur on 12/07/2022.
//
#include <chrono>
#include <Concerto/Core/Math/Vector.hpp>
#include <Concerto/Core/Math/Algorithm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "VulkanRenderer.hpp"
#include "Camera.hpp"
#include "Input.hpp"
#include <Primitives.hpp>

using namespace Concerto;
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
	Input input;
	GLFW3WindowPtr window = std::make_unique<GlfW3>(info.applicationName, 1280, 720);
	Swapchain swapchain();

	
	VulkanRenderer engine(std::move(info), *window.get());
	Concerto::Graphics::ImGUI* imGui = engine.GetImGUIContext();
	assert(imGui != nullptr);
	ImGui::SetCurrentContext(imGui->GetContext());
	float aspect = float(window->GetWidth()) / float(window->GetHeight());
	float deltaTime = 0.f;
	Scene sceneParameters{};
	sceneParameters.gpuSceneData.sunlightDirection = { 3.1f, 1.f, -1.f, 0 };
	sceneParameters.gpuSceneData.ambientColor = { 0.f, 0.f, 0.f, 1.f };
	sceneParameters.gpuSceneData.sunlightColor = { 255.f, 109.f, 39.f, 1.f };
	sceneParameters.clearColor = { 0.1f, 0.1f, 0.1f, 1.f };
	engine.UpdateSceneParameters(sceneParameters);

	Camera camera(90.f, 0.1f, 1000000.f, aspect);

	window->RegisterCursorPosCallback([&](AWindow<GLFWwindow>& window, double x, double y)
	{
	  imGui->UpdateMousePosition(x, y);
	  Concerto::MouseEvent mouseEvent{};
	  static double deltaX = 0, deltaY = 0;
	  deltaX = x - deltaX;
	  deltaY = y - deltaY;
	  mouseEvent.type = Concerto::MouseEvent::Moved;
	  mouseEvent.mouseMove.x = x;
	  mouseEvent.mouseMove.y = y;
	  mouseEvent.mouseMove.deltaX = deltaX;
	  mouseEvent.mouseMove.deltaY = deltaY;
	  input.TriggerMouseEvent(mouseEvent, deltaTime);
	  deltaX = x;
	  deltaY = y;
	});

	window->RegisterMouseButtonCallback([&](AWindow<GLFWwindow>& window, int button, int action, int mods)
	{
	  imGui->UpdateMouseButton(button, action == GLFW_PRESS);
	  Concerto::MouseEvent mouseEvent{};
	  mouseEvent.type = Concerto::MouseEvent::Button;
	  mouseEvent.button.button = Concerto::MouseButton::Button(button);
	  mouseEvent.button.triggerType = Concerto::TriggerType(action);
	  input.TriggerMouseEvent(mouseEvent, deltaTime);
	});

	window->RegisterKeyCallback([&](AWindow<GLFWwindow>& window, Key key, int scancode, int action, int mods)
	{
	  Concerto::KeyEvent keyEvent{};
	  keyEvent.key = Concerto::Key(key);
	  keyEvent.triggerType = Concerto::TriggerType(action);
	  input.TriggerKeyEvent(keyEvent, deltaTime);
	});

	window->RegisterResizeCallback([&](AWindow<GLFWwindow>& window)
	{
	  aspect = float(window.GetWidth()) / float(window.GetHeight());
	  camera.projectionMatrix = glm::perspective(45.f, aspect, 0.1f, 1000.f);
	  camera.viewProjectionMatrix = camera.projectionMatrix * camera.viewMatrix * glm::mat4(1.f);
	});

	bool cursorDisabled = false;
	window->SetCursorDisabled(cursorDisabled);
	float speed = 150.f;
	bool imguiFocused = false;
	Input::Instance().Register("MouseMoved", MouseEvent::Type::Moved, [&](const MouseEvent& e, float deltaTime)
	{
	  if (imguiFocused)
		  return;
	  camera.Rotate(e.mouseMove.deltaX, -e.mouseMove.deltaY);
	});

	Input::Instance().Register("Forward", Key::Z, TriggerType::Held, [&](float deltaTime)
	{
	  camera.Move(FreeFlyCameraMovement::Forward, deltaTime * speed);
	});

	Input::Instance().Register("Backward", Key::S, TriggerType::Held, [&](float deltaTime)
	{
	  camera.Move(FreeFlyCameraMovement::Backward, deltaTime * speed);
	});

	Input::Instance().Register("Left", Key::Q, TriggerType::Held, [&](float deltaTime)
	{
	  camera.Move(FreeFlyCameraMovement::Left, deltaTime * speed);
	});

	Input::Instance().Register("_right", Key::D, TriggerType::Held, [&](float deltaTime)
	{
	  camera.Move(FreeFlyCameraMovement::Right, deltaTime * speed);
	});

	Input::Instance().Register("MouseFocused", Key::LeftAlt, TriggerType::Pressed, [&](float deltaTime)
	{
	  cursorDisabled = !cursorDisabled;
	  window->SetCursorDisabled(cursorDisabled);
	});

	/*MeshPtr sponzaMesh = std::make_shared<Mesh>();
	sponzaMesh->LoadFromFile("./assets/sponza/sponza.obj");*/
	Math::Vector3f position(0.f, 0.f, 0.f);
	Math::Quaternionf rotation(Math::EulerAnglesf(0.f, 0.f, 0.f));
	Math::Vector3f scale(1.f, 1.f, 1.f);

	MeshPtr cube = std::make_shared<Mesh>(Primitive::MakeCube(50.f));

	std::chrono::steady_clock::time_point lastFrameTime = std::chrono::steady_clock::now();
	while (!window->ShouldClose())
	{
		window->PopEvent();
		imguiFocused = ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow);
		auto beginTime = std::chrono::high_resolution_clock::now();
		deltaTime = std::chrono::duration<float>(beginTime - lastFrameTime).count();
		lastFrameTime = beginTime;
		int fps = 1.f / deltaTime;
		imGui->NewFrame();
		ImGui::Begin("Timings");
		ImGui::Text("%d fps", fps);
		ImGui::Text("%f ms", deltaTime * 1000.f);
		ImGui::Text("Transfers %f ms", engine._transfersTime * 1000.f);
		ImGui::Text("Draw %f ms", engine._drawTime * 1000.f);
		ImGui::Text("Load meshes %f ms", engine._drawObjectsTime * 1000.f);
		ImGui::End();

		ImGui::Begin("Camera");
		ImGui::Text("Position : %f %f %f",
			camera.GetPosition().X(),
			camera.GetPosition().Y(),
			camera.GetPosition().Z());
		ImGui::Text("Rotation : %f %f %f",
			camera.GetRotation().Pitch(),
			camera.GetRotation().Yaw(),
			camera.GetRotation().Roll());
		ImGui::DragFloat("Speed", &speed, 1.f, 0.f, 100000.f);
		ImGui::End();

		if (ImGui::ColorEdit3("Select clear color", &sceneParameters.clearColor[0]))
			engine.UpdateSceneParameters(sceneParameters);
		if (ImGui::ColorEdit3("Select ambiant color", &sceneParameters.gpuSceneData.ambientColor[0]))
			engine.UpdateSceneParameters(sceneParameters);
		if (ImGui::ColorEdit3("Select sun color", &sceneParameters.gpuSceneData.sunlightColor[0]))
			engine.UpdateSceneParameters(sceneParameters);

		engine.DrawObject(cube, position, rotation, scale);
		engine.Draw(camera);
	}
}
