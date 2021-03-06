#ifndef __CAMERA__HPP
#define __CAMERA__HPP

#pragma once

namespace splineengine {

const unsigned int NUMBER_OF_CAMERAS = 2;

const unsigned int POV_CAMERA = 0;
const unsigned int TRACKBALL_CAMERA = 1;

const float cameraRotateSpeed = 0.4f;
const float cameraZoomSpeed = 1.f;
/// \brief Camera is an abstract class, parent of TrackballCamera and POVCamera
class Camera
{
	public:
		Camera();
		virtual ~Camera();

		/// \brief Zoom in and out (Trackball Camera only)
		/// \param delta : float value of the zoom
		virtual void moveFront(float delta) = 0;

		/// \brief Rotate on the horizontal axis
		/// \param degrees : float value of the rotation
		virtual void rotateLeft(float degrees) = 0;

		/// \brief Rotate on the vertical axis
		/// \param degrees : float value of the rotation
		virtual void rotateUp(float degrees) = 0;

		/// \brief Get the camera view matrix
		virtual glm::mat4 getViewMatrix() const = 0;

		/// \brief Update camera zoom
		virtual void update() = 0;

	private:
		/// \brief Position of the camera
		glm::vec3 _position;
};

}

#endif