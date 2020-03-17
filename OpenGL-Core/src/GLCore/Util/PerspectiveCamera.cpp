#include "glpch.h"
#include "PerspectiveCamera.h"


namespace GLCore::Utils {






	void Camera::SetProjectionMatrix(glm::mat4& arg_projection)
	{
		projection = arg_projection;
	}

	void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
	{

		float velocity = MovementSpeed * deltaTime;
		if (direction == FORWARD)
			Position += Front * velocity;
		if (direction == BACKWARD)
			Position -= Front * velocity;
		if (direction == LEFT)
			Position -= Right * velocity;
		if (direction == RIGHT)
			Position += Right * velocity;

		std::cout << Position.x << " " << Position.y << " " << Position.z << std::endl;
	}

	void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
	{
		xoffset = xoffset / 1000;
		yoffset = yoffset / 1000;

		// the bloody function pointer passing should be done in the camera controller ...
		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;

		Yaw += xoffset;
		Pitch += yoffset;

		// Make sure that when pitch is out of bounds, screen doesn't get flipped
		if (constrainPitch)
		{
			if (Pitch > 89.0f)
				Pitch = 89.0f;
			if (Pitch < -89.0f)
				Pitch = -89.0f;
		}

		// Update Front, Right and Up Vectors using the updated Euler angles
		updateCameraVectors();


	}

	void Camera::ProcessMouseScroll(float yoffset)
	{
		if (Zoom >= 1.0f && Zoom <= 45.0f)
			Zoom -= yoffset;
		if (Zoom <= 1.0f)
			Zoom = 1.0f;
		if (Zoom >= 45.0f)
			Zoom = 45.0f;
	}


}