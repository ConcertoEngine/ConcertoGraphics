//
// Created by arthur on 16/12/2022.
//

#ifndef CONCERTOGRAPHICS_CAMERAVIEWER_HPP
#define CONCERTOGRAPHICS_CAMERAVIEWER_HPP

#include <glm/vec3.hpp>
#include <glm/ext/quaternion_float.hpp>
#include <Concerto/Core/Types.hpp>

namespace Concerto::Graphics
{
	class Camera;

	/**
	 * @brief Controls the viewing parameters of the camera.
	 * @details Support basic viewing operation like translation, rotation, zoom.
	 */
	class CONCERTO_PUBLIC_API CameraViewer
	{
	public:
		CameraViewer(const glm::vec3& viewPos, const glm::vec3& pivot, const glm::vec3& upVector, float fov,
				float aspectRatio);

		/**
		 * @brief Update the camera's view matrix.
		 * @param camera
		 */
		void UpdateCameraView(Camera& camera) const;

		/**
		 * @brief Translate the camera on a 2D space.
		 * @param mouseX The mouse's x position.
		 * @param mouseY The mouse's y position.
		 * @param keepParallel If true, the camera will keep parallel to the up vector.
		 */
		void Translate(float mouseX, float mouseY, bool keepParallel = true);

		/**
		 * @brief Rotate the camera around the pivot.
		 * @param mouseX The mouse's x position.
		 * @param mouseY The mouse's y position.
		 */
		void Rotate(float mouseX, float mouseY);

		/**
		 * @brief Zoom the camera in or out.
		 * @param zoom
		 */
		void Zoom(float zoom);
	private:
		void GetFrustrumInfo();
		glm::quat SetFromAxisAngle(glm::vec3& axis, float angle);

		glm::vec3 _viewPos;
		glm::vec3 _pivot;
		glm::vec3 _upVector;
		float _fov;
		float _aspectRatio;

		float _translationSpeed = 0.05f;
		float _rotationSpeed = 2.5f;
		float _zoomSpeed = 2.f;

		glm::vec3 _viewDirection;
		glm::vec3 _planeHorizontalDirection;
		glm::vec3 _planeVerticalDirection;

		float _displayWidth;
		float _displayHeight;
	};

} // Concerto::Graphics

#endif //CONCERTOGRAPHICS_CAMERAVIEWER_HPP
