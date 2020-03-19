#pragma once
#include <GLCore.h>
#include "GLCore/Core/Timestep.h"
#include "GLCore/Events/ApplicationEvent.h"
#include "GLCore/Events/MouseEvent.h"


namespace GLCore::Utils {
	// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
	enum Camera_Movement {
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT,
		UPWARD,
		DOWN
	};

	// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
	class Camera
	{
	private:
		// Camera Attributes
		glm::vec3 Position;
		glm::vec3 Front;
		glm::vec3 Up;
		glm::vec3 Right;
		glm::vec3 WorldUp;
		glm::mat4 projection;
		float Yaw = -90.0f;
		float Pitch = 0.0f;
		float MovementSpeed = 2.5f;
		float MouseSensitivity = 0.1f;
		float Zoom = 45.0f;
	public:

		glm::vec3& GetPosition()
		{
			return Position;
		}
		// Constructor with vectors
		Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 20.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = -90.0f, float pitch = 0.0f) : Front(glm::vec3(0.0f, 0.0f, -1.0f))
		{
			Position = position;
			WorldUp = up;
			Yaw = yaw;
			Pitch = pitch;
			updateCameraVectors();
			projection = glm::perspective(glm::radians(40.0f), (float)(800 / 600), 0.1f, 100.0f);
		}

		// Returns the view matrix calculated using Euler Angles and the LookAt Matrix
		glm::mat4& Camera::GetViewMatrix()
		{
			return glm::lookAt(Position, Position + Front, Up);
		}

		void SetVelocity(float arg_velocity)
		{
			MovementSpeed = arg_velocity;
		}

		// the projection matrix of the camera ...
		glm::mat4& Camera::GetProjectionMatrix()
		{
			return projection;
		}
		void SetProjectionMatrix(glm::mat4& arg_projection);
		// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
		void ProcessKeyboard(Camera_Movement direction, float deltaTime);


		// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
		void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

		// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
		void ProcessMouseScroll(float yoffset);

	private:
		// Calculates the front vector from the Camera's (updated) Euler Angles
		void Camera::updateCameraVectors()
		{
			glm::vec3 front;
			front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
			front.y = -sin(glm::radians(Pitch));
			front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
			Front = glm::normalize(front);
			Right = glm::normalize(glm::cross(Front, WorldUp));
			Up = glm::normalize(glm::cross(Right, Front));
		}

	};
}