//
// Created by arthur on 21/03/2023.
//

#include "Concerto/Graphics/Camera.hpp"

namespace Concerto::Graphics
{
	Camera::Camera(float fov, float near, float far, float aspectRatio) :
		_eulerAngles(),
		_velocity(),
		_position(),
		_movementSpeed(),
		_mouseSensitivity(),
		_fov(fov),
		_near(near),
		_far(far),
		_aspectRatio(aspectRatio),
		_clearColor(0.0f, 0.0f, 0.0f, 1.0f)
	{
		UpdateProjectionMatrix();
	}


	const EulerAnglesf& Camera::GetRotation() const
	{
		return _eulerAngles;
	}

	const Vector4f& Camera::GetClearColor() const
	{
		return _clearColor;
	}

	Vector3f Camera::GetPosition() const
	{
		return _velocity;
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

	void Camera::SetClearColor(const Vector4f& clearColor)
	{
		_clearColor = clearColor;
	}

	void Camera::SetPosition(const Vector3f& position)
	{
		_position = Vector3f(position.X(), position.Y(), position.Z());
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
		UpdateProjectionMatrix();
	}

	void Camera::SetAspectRatio(float aspectRatio)
	{
		_aspectRatio = aspectRatio;
		UpdateProjectionMatrix();
	}

	Vector<float, 3> operator*(const Matrix4f& mat, const Vector4f& vec)
	{
		Vector<float, 3> result;

		for (std::size_t widthIndex = 0; widthIndex < mat.GetWidth(); ++widthIndex) {
			const std::size_t finalWidthIndex = widthIndex * mat.GetHeight();

			for (std::size_t heightIndex = 0; heightIndex < mat.GetHeight(); ++heightIndex)
				result[heightIndex] += mat.Data()[finalWidthIndex + heightIndex] * vec[widthIndex];
		}

		return result;
	}

	void Camera::UpdateViewProjectionMatrix()
	{
		const Matrix4f translation = _position.ToTranslationMatrix();
		const Matrix4f cameraRotation = _eulerAngles.ToQuaternion().ToRotationMatrix<Matrix4f>();
		viewMatrix = (translation * cameraRotation ).Inverse();
		viewProjectionMatrix = projectionMatrix * viewMatrix;
	}

	void Camera::UpdateProjectionMatrix()
	{
		const float tanHalfFov = std::tan(_fov / 2.0f);
		projectionMatrix = Matrix4f();
		projectionMatrix(0, 0) = 1.f / (_aspectRatio * tanHalfFov);
		projectionMatrix(1, 1) = 1.f / tanHalfFov;
		projectionMatrix(2, 2) = _far / (_near - _far);
		projectionMatrix(2, 3) = -(2.f * _far* _near) / (_far - _near);
		projectionMatrix(3, 2) = -1.f;
		projectionMatrix(1, 1) *= -1.f;
	}

	void Camera::Rotate(double deltaX, double deltaY)
	{
		_eulerAngles.Yaw() -= deltaX;
		_eulerAngles.Pitch() += deltaY;

		if (_eulerAngles.Pitch() >= 89.0f)
			_eulerAngles.Pitch() = 89.0f;
		if (_eulerAngles.Pitch() <= -89.0f)
			_eulerAngles.Pitch() = -89.0f;
	}

	void Camera::Move(CameraMovement direction, float x)
	{
		switch (direction) {
		case CameraMovement::Forward:
			_position += _eulerAngles.ToQuaternion() * Vector3f::Forward() * x;
			break;
		case CameraMovement::Backward:
			_position += _eulerAngles.ToQuaternion() * Vector3f::Backward() * x;
			break;
		case CameraMovement::Left:
			_position += _eulerAngles.ToQuaternion() * Vector3f::Right() * x;
			break;
		case CameraMovement::Right:
			_position += _eulerAngles.ToQuaternion() * Vector3f::Left() * x;
			break;
		}
	}
}
