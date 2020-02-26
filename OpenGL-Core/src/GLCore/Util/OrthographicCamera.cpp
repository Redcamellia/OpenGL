#include "../glfw/include/GLFW/glfw3.h"
#include "glpch.h"
#include "OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace GLCore::Utils {

	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		:
		m_ViewMatrix(glm::lookAt(m_Position
			, glm::vec3(0.0f, 0.0f, 0.0f)
			, glm::vec3(0.0f, 1.0f, 0.0f))),
		
		m_ProjectionMatrix(glm::perspective(glm::radians(40.0f) , (float)(1600/1200) , 0.1f , 100.0f))
	{
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
	{
		//m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		//m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::RecalculateViewMatrix()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));

		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

}