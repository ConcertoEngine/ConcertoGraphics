//
// Created by arthur on 21/03/2023.
//

#ifndef BASICRENDERING_INCLUDE_CONCERTO_GRAPHICS_CAMERA_HPP_
#define BASICRENDERING_INCLUDE_CONCERTO_GRAPHICS_CAMERA_HPP_

#include <Concerto/Core/Math/Quaternion.hpp>
#include <Concerto/Core/Math/Vector.hpp>
#include <Concerto/Core/Types.hpp>
#include <glm/mat4x4.hpp>

#include "Concerto/Graphics/GPUData.hpp"

namespace Concerto::Graphics
{
	enum FreeFlyCameraMovement
	{
		Forward,
		Backward,
		Left,
		Right
	};

// Default camera values
	inline constexpr float YAW = -90.0f;
	inline constexpr float PITCH = 0.0f;
	inline constexpr float SPEED = 2.5f;
	inline constexpr float SENSITIVITY = 0.1f;
	inline constexpr float ZOOM = 45.0f;


	class CONCERTO_PUBLIC_API Camera : public GPUCamera
	{
	 public:

		Camera(float fov, float near, float far, float aspectRatio);
		Camera(const Camera&) = default;
		Camera(Camera&&) = default;
		Camera& operator=(const Camera&) = default;
		Camera& operator=(Camera&&) = default;

		[[nodiscard]] const Math::EulerAnglesf& GetRotation() const;
		[[nodiscard]] const Math::Vector4f& GetClearColor() const;
		[[nodiscard]] Math::Vector3f GetPosition() const;
		[[nodiscard]] float GetFov() const;
		[[nodiscard]] float GetNear() const;
		[[nodiscard]] float GetFar() const;
		[[nodiscard]] float GetAspectRatio() const;

		void SetClearColor(const Math::Vector4f& clearColor);
		void SetPosition(const Math::Vector3f& position);
		void SetFov(float fov);
		void SetNear(float near);
		void SetFar(float far);
		void SetAspectRatio(float aspectRatio);

		// processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
		void Move(FreeFlyCameraMovement direction, float deltaTime);

		// processes input received from a mouse input system. Expects the offset value in both the x and y direction.
		void Rotate(float xoffset, float yoffset, bool constrainPitch = true);

		// processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
		void ProcessMouseScroll(float yoffset);

	 private:
		// calculates the front vector from the Camera's (updated) Euler Angles
		void updateCameraVectors();
		void UpdateViewMatrix();
		void UpdateProjectionMatrix();
		void UpdateViewProjectionMatrix();
		glm::vec3 _position;
		glm::vec3 _front;
		glm::vec3 _up;
		glm::vec3 _right;
		glm::vec3 _worldUp;
		Math::EulerAnglesf _eulerAngles;
		float _movementSpeed;
		float _mouseSensitivity;
		float _zoom;
		float _fov;
		float _near;
		float _far;
		float _aspectRatio;
		Math::Vector4f _clearColor;
	};
}

#endif //BASICRENDERING_INCLUDE_CONCERTO_GRAPHICS_CAMERA_HPP_
