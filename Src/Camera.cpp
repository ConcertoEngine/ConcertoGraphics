//
// Created by arthur on 21/03/2023.
//

#include <glm/gtc/quaternion.hpp>

#include "Concerto/Graphics/Camera.hpp"

namespace Concerto::Graphics
{
	Camera::Camera(float fov, float near, float far, float aspectRatio) :
		_fov(fov),
		_near(near),
		_far(far),
		_aspectRatio(aspectRatio),
		_clearColor(Math::Vector4f(0.0f, 0.0f, 0.0f, 1.0f)),
		_position(glm::vec3(0.0f, 0.0f, 0.0f)),
		_zoom(ZOOM),
		_up(0.0f, 1.0f, 0.0f),
		_worldUp(_up),
		_movementSpeed(SPEED),
		_mouseSensitivity(SENSITIVITY),
		_eulerAngles(Math::EulerAnglesf(PITCH, YAW, 0.0f))
	{
		UpdateProjectionMatrix();
		UpdateViewMatrix();
		updateCameraVectors();
	}

	void Camera::Move(FreeFlyCameraMovement direction, float deltaTime)
	{
		float velocity = _movementSpeed * deltaTime;
		if (direction == Forward)
			_position += _front * velocity;
		if (direction == Backward)
			_position -= _front * velocity;
		if (direction == Left)
			_position -= _right * velocity;
		if (direction == Right)
			_position += _right * velocity;
		updateCameraVectors();
	}

	void Camera::Rotate(float xoffset, float yoffset, bool constrainPitch)
	{
		xoffset *= _mouseSensitivity;
		yoffset *= _mouseSensitivity;

		_eulerAngles.Yaw() += xoffset;
		_eulerAngles.Pitch() += yoffset;

		// make sure that when pitch is out of bounds, screen doesn't get flipped
//		if (constrainPitch)
//		{
//			if (Pitch > 89.0f)
//				Pitch = 89.0f;
//			if (Pitch < -89.0f)
//				Pitch = -89.0f;
//		}

		updateCameraVectors();
	}

	void Camera::ProcessMouseScroll(float yoffset)
	{
		_zoom -= (float)yoffset;
		if (_zoom < 1.0f)
			_zoom = 1.0f;
		if (_zoom > 45.0f)
			_zoom = 45.0f;
	}

	void Camera::updateCameraVectors()
	{
		glm::vec3 front;
		float yaw = _eulerAngles.Yaw();
		float pitch = _eulerAngles.Pitch();
		front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.y = sin(glm::radians(pitch));
		front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		_front = glm::normalize(front);
		_right = glm::normalize(glm::cross(_front, _worldUp));
		_up = glm::normalize(glm::cross(_right, _front));
		UpdateViewMatrix();
		UpdateViewProjectionMatrix();
	}

	const Math::EulerAnglesf& Camera::GetRotation() const
	{
		return _eulerAngles;
	}

	const Math::Vector4f& Camera::GetClearColor() const
	{
		return _clearColor;
	}

	Math::Vector3f Camera::GetPosition() const
	{
		return Math::Vector3f(_position.x, _position.y, _position.z);
	}

	float Camera::GetFov() const
	{
		return _fov;
	}

	float Camera::GetNear() const
	{
		return _near;
	}

	float Camera::GetFar() const
	{
		return _far;
	}

	float Camera::GetAspectRatio() const
	{
		return _aspectRatio;
	}

	void Camera::SetClearColor(const Math::Vector4f& clearColor)
	{
		_clearColor = clearColor;
	}

	void Camera::SetPosition(const Math::Vector3f& position)
	{
		_position = glm::vec3(position.X(), position.Y(), position.Z());
		updateCameraVectors();
	}

	void Camera::SetFov(float fov)
	{
		_fov = fov;
		UpdateProjectionMatrix();
	}

	void Camera::SetNear(float near)
	{
		_near = near;
		UpdateProjectionMatrix();
	}

	void Camera::SetFar(float far)
	{
		_far = far;
	}

	void Camera::SetAspectRatio(float aspectRatio)
	{
		_aspectRatio = aspectRatio;
		UpdateProjectionMatrix();
	}

	void Camera::UpdateViewMatrix()
	{
		viewMatrix = glm::lookAt(_position, _position + _front, _up);
		UpdateViewProjectionMatrix();
	}

	void Camera::UpdateProjectionMatrix()
	{
		projectionMatrix = glm::perspective(glm::radians(_fov), _aspectRatio, _near, _far);
		projectionMatrix[1][1] *= -1;
		UpdateViewProjectionMatrix();
	}

	void Camera::UpdateViewProjectionMatrix()
	{
		viewProjectionMatrix = projectionMatrix * viewMatrix * glm::mat4(1.0f);
	}
}