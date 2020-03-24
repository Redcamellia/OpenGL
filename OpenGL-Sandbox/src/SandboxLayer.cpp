#include "SandboxLayer.h"
#include "../glfw/include/GLFW/glfw3.h"
#include "stb_image/stb_image.h"
#include "../src/Platform/Windows/WindowsWindow.h"
using namespace GLCore;
using namespace GLCore::Utils;


namespace cameraGlobal {
	float lastX = 0;
	float lastY = 0;
	bool firstMouse = true;
	float xoffset = 0;
	float yoffset = 0;
}
using namespace cameraGlobal;
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	xoffset = xpos;
	yoffset = ypos;

}

SandboxLayer::SandboxLayer()
	: m_CameraController(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f)
{
	
}

SandboxLayer::~SandboxLayer()
{
}

void SandboxLayer::OnAttach()
{
	EnableGLDebugging();

	m_shader = Shader::FromGLSLTextFiles("assets/shaders/vertex.vert", "assets/shaders/frag.frag");
	m_light_shader = Shader::FromGLSLTextFiles("assets/shaders/lightVertex.vert", "assets/shaders/lightFragment.frag");
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glfwSetCursorPosCallback((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow(), mouse_callback);
	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
	};
	m_textures.push_back(m_shader->loadTexture("assets/textures/container2.png"));
	m_textures.push_back(m_shader->loadTexture("assets/textures/container2_specular.png"));
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glfwSetInputMode((GLFWwindow*)GLCore::Application::Get().GetWindow().GetNativeWindow()
			, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
	glGenVertexArrays(1, &lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindVertexArray(lightVAO);

	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// Init here
}

void SandboxLayer::OnDetach()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	// Shutdown here
}

void SandboxLayer::OnEvent(Event& event)
{
	//m_CameraController.OnEvent(event); // chains of responsibility

	EventDispatcher dispatcher(event);
	

}

void SandboxLayer::OnUpdate(Timestep ts)
{
	glUseProgram(m_shader->GetRendererID());

	glm::vec3 cubePositions[] = {
		  glm::vec3(3.0f,  9.0f,  2.0f),
		  glm::vec3(2.0f,  5.0f, -15.0f),
		  glm::vec3(-1.5f, -2.2f, -2.5f),
		  glm::vec3(-3.8f, -2.0f, -12.3f),
		  glm::vec3(2.4f, -0.4f, -3.5f),
		  glm::vec3(-1.7f,  3.0f, -7.5f),
		  glm::vec3(1.3f, -2.0f, -2.5f),
		  glm::vec3(1.5f,  2.0f, -2.5f),
		  glm::vec3(1.5f,  0.2f, -1.5f),
		  glm::vec3(-1.3f,  1.0f, -1.5f)
	};
	
	m_CameraController.OnUpdate(ts);
	m_CameraController.MouseProcess(cameraGlobal::xoffset, cameraGlobal::yoffset);
	glm::mat4 model = glm::mat4(1.0f);
	//model = glm::translate(model, cubePositions[0]);

	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_shader->setVec3("viewPos", m_CameraController.GetCamera().GetPosition());
	m_shader->setInt("material.diffuse", 0);
	m_shader->setInt("material.specular", 1);
	m_shader->setFloat("material.shininess", 32.0f);

	m_shader->setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
	m_shader->setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
	m_shader->setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
	m_shader->setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
	
	m_shader->setVec3("pointLights[0].position", pointLightPositions[0]);
	m_shader->setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
	m_shader->setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
	m_shader->setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
	m_shader->setFloat("pointLights[0].constant", 1.0f);
	m_shader->setFloat("pointLights[0].linear", 0.09);
	m_shader->setFloat("pointLights[0].quadratic", 0.032);
	
	m_shader->setVec3("pointLights[1].position", pointLightPositions[1]);
	m_shader->setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
	m_shader->setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
	m_shader->setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
	m_shader->setFloat("pointLights[1].constant", 1.0f);
	m_shader->setFloat("pointLights[1].linear", 0.09);
	m_shader->setFloat("pointLights[1].quadratic", 0.032);
	
	m_shader->setVec3("pointLights[2].position", pointLightPositions[2]);
	m_shader->setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
	m_shader->setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
	m_shader->setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
	m_shader->setFloat("pointLights[2].constant", 1.0f);
	m_shader->setFloat("pointLights[2].linear", 0.09);
	m_shader->setFloat("pointLights[2].quadratic", 0.032);
	
	m_shader->setVec3("pointLights[3].position", pointLightPositions[3]);
	m_shader->setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
	m_shader->setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
	m_shader->setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
	m_shader->setFloat("pointLights[3].constant", 1.0f);
	m_shader->setFloat("pointLights[3].linear", 0.09);
	m_shader->setFloat("pointLights[3].quadratic", 0.032);

	m_shader->setVec3("spotLight.position", m_CameraController.GetCamera().GetPosition());
	m_shader->setVec3("spotLight.direction", m_CameraController.GetCamera().getCameraFront());
	m_shader->setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
	m_shader->setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
	m_shader->setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
	m_shader->setFloat("spotLight.constant", 1.0f);
	m_shader->setFloat("spotLight.linear", 0.09);
	m_shader->setFloat("spotLight.quadratic", 0.032);
	m_shader->setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
	m_shader->setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));



	//
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindVertexArray(VAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_textures[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_textures[1]);
	glm::mat4 tabdil = m_CameraController.GetCamera().GetViewMatrix();
	
	for (int i = 0; i < 10; i++)
	{
		model = glm::translate(model, cubePositions[i]);
		float angle = 20.0f * i;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		m_shader->setMat4("model", model);
		m_shader->setMat4("projection", m_CameraController.GetCamera().GetProjectionMatrix());
		m_shader->setMat4("view", tabdil);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = glm::mat4(1.0f);
	}




	for (int i = 0; i < 4; i++)
	{
		model = glm::mat4(1.0f);
		model = glm::translate(model, pointLightPositions[i]);
		model = glm::scale(model, glm::vec3(0.2f));

		glUseProgram(m_light_shader->GetRendererID());
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glBindVertexArray(lightVAO);
		glEnableVertexAttribArray(0);


		m_light_shader->setMat4("model", model);
		m_light_shader->setMat4("projection", m_CameraController.GetCamera().GetProjectionMatrix());
		m_light_shader->setMat4("view", tabdil);


		glDrawArrays(GL_TRIANGLES, 0, 36);
	}



}

void SandboxLayer::OnImGuiRender()
{
	// ImGui here
}
