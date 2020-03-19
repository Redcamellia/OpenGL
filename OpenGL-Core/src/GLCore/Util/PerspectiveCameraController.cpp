#include "glpch.h"
#include "PerspectiveCameraController.h"
#include <iostream>


void GLCore::Utils::PerspectiveCameraController::MouseProcess(float arg_x, float arg_y)
{
	m_Camera.ProcessMouseMovement(arg_x, arg_y);

}

void GLCore::Utils::PerspectiveCameraController::SetCameraVelocity(float arg_velocity)
{
	m_Camera.SetVelocity(arg_velocity);
}
