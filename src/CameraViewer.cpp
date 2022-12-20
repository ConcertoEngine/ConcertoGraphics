//
// Created by arthur on 16/12/2022.
//

#include <iostream>
#include <glm/ext/quaternion_trigonometric.hpp>
#include "glm/glm.hpp"
#include "CameraViewer.hpp"
#include "GPUData.hpp"

namespace Concerto::Graphics
{
	CameraViewer::CameraViewer(const glm::vec3& viewPos, const glm::vec3& pivot, const glm::vec3& upVector, float fov,
			float aspectRatio) : _viewPos(viewPos), _pivot(pivot), _upVector(glm::normalize(upVector)), _fov(fov),
								 _aspectRatio(aspectRatio)
	{
		GetFrustrumInfo();
	}

	void CameraViewer::UpdateCameraView(Camera& camera) const
	{
		camera.view = glm::lookAt(_viewPos, _pivot, _upVector);
		camera.proj = glm::perspective(_fov, _aspectRatio, 0.01f, 500.f);
		camera.proj[1][1] *= -1;
		camera.viewproj = camera.proj * camera.view * glm::mat4(1.0f);
	}

	void CameraViewer::Translate(float mouseX, float mouseY, bool keepParallel)
	{
		glm::vec3 translation;

		if (keepParallel)
		{

			translation = (_planeHorizontalDirection * (_displayWidth * mouseX)) +
						  (_planeVerticalDirection * (mouseY * _displayHeight));
		}
		else
		{
			translation = (_viewPos - _pivot) * mouseY;
		}
		translation *= _translationSpeed;

		_viewPos += translation;
		_pivot += translation;
	}

	void CameraViewer::Rotate(float mouseX, float mouseY)
	{

		float horizRotAngle = _rotationSpeed * mouseY;
		float vertRotAngle = -_rotationSpeed * mouseX;


		glm::quat rotationY = glm::angleAxis( horizRotAngle, _planeHorizontalDirection);
		glm::quat rotationX = glm::angleAxis(vertRotAngle, _planeVerticalDirection);

		_viewPos = _pivot + (rotationY * rotationX) * (_viewPos - _pivot);
	}

	void CameraViewer::Zoom(float zoom)
	{
		float scaleFactor = powf(2.0, -zoom * _zoomSpeed);
		_viewPos = _pivot + (_viewPos - _pivot) * scaleFactor;
		GetFrustrumInfo();
	}

	void CameraViewer::GetFrustrumInfo()
	{
		_viewDirection = _pivot - _viewPos;
		_viewDirection = glm::normalize(_viewDirection);

		_planeVerticalDirection = _upVector;
		_planeVerticalDirection -= glm::dot(_planeVerticalDirection, _viewDirection) * _viewDirection;
		_planeVerticalDirection = glm::normalize(_planeVerticalDirection);

		_planeHorizontalDirection = glm::cross(_viewDirection, _planeVerticalDirection);
		_planeHorizontalDirection = glm::normalize(_planeHorizontalDirection);

		_displayHeight = 2.0f * glm::length(_pivot - _viewPos) * std::tan(0.5f * _fov);
		_displayWidth = _displayHeight * _aspectRatio;
	}

	glm::quat CameraViewer::SetFromAxisAngle(glm::vec3& axis, float angle)
	{
		float cosAng = std::cosf(angle / 2.0f);
		float sinAng = std::sinf(angle / 2.0f);
		float norm = std::sqrt(axis.x * axis.x + axis.y * axis.y + axis.z * axis.z);
		return {
				(axis.x / norm) * sinAng,
				(axis.y / norm) * sinAng,
				(axis.z / norm) * sinAng,
				cosAng
		};

	};
} // Concerto::Graphics