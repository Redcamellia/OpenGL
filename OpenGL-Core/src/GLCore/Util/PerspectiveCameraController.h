#pragma once
#include "PerspectiveCamera.h"
#include "GLCore/Core/Timestep.h"

#include "GLCore/Events/ApplicationEvent.h"
#include "GLCore/Events/MouseEvent.h"
#include "../glfw/include/GLFW/glfw3.h"


#include <iostream>





namespace GLCore::Utils
{


	class PerspectiveCameraController
	{
	public:
		PerspectiveCameraController(glm::vec3& arg_position, glm::vec3& arg_up, float arg_pitch, float arg_yaw)
			:m_Camera(arg_position, arg_up, arg_pitch, arg_yaw)
		{

			std::cout << "perspective camera deplyed ..." << std::endl;
		}
		Camera& GetCamera() { return m_Camera; }
		const Camera& GetCamera() const { return m_Camera; }
		void MouseProcess(float arg_x, float arg_y);
		void SetCameraVelocity(float arg_velocity);
		void OnUpdate(Timestep ts)
		{
			
			GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

			if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
				glfwSetWindowShouldClose(window, true);

			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
				m_Camera.ProcessKeyboard(FORWARD, ts);
			if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
				m_Camera.ProcessKeyboard(BACKWARD, ts);
			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
				m_Camera.ProcessKeyboard(LEFT, ts);
			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
				m_Camera.ProcessKeyboard(RIGHT, ts);
			if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
				m_Camera.ProcessKeyboard(UPWARD, ts);
			if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
				m_Camera.ProcessKeyboard(DOWN, ts);

		}
		
	private:
			bool OnMouseScrolled(MouseScrolledEvent& e);
			bool OnWindowResized(WindowResizeEvent& e);
			Camera m_Camera;





		void OnEvent(Event& e);
	};






}