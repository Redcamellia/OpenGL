#include "SandboxLayer.h"
#include "../glfw/include/GLFW/glfw3.h"
#include "stb_image/stb_image.h"
#include "../src/Platform/Windows/WindowsWindow.h"
using namespace GLCore;
using namespace GLCore::Utils;

double xpos;
double ypos;

void mouse_callback(GLFWwindow* window, double x, double y )
{
	xpos = x;
	ypos = y;
}


SandboxLayer::SandboxLayer()
: m_CameraController(16.0f / 9.0f)
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
	m_textures.push_back(m_light_shader->loadTexture("assets/textures/container2.png"));
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	glfwSetCursorPosCallback((GLFWwindow*)GLCore::Application::Get().GetWindow().GetNativeWindow()
		,mouse_callback);

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
	m_CameraController.OnEvent(event);

	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<MouseButtonPressedEvent>(
		[&](MouseButtonPressedEvent& e)
		{
			m_SquareColor = m_SquareAlternateColor;
			return false;
		});
	dispatcher.Dispatch<MouseButtonReleasedEvent>(
		[&](MouseButtonReleasedEvent& e)
		{
			m_SquareColor = m_SquareBaseColor;
			return false;
		});
	

}

void SandboxLayer::OnUpdate(Timestep ts)
{
	glUseProgram(m_shader->GetRendererID());
	if (glfwGetKey(static_cast<GLFWwindow*>(GLCore::Application::Get().GetWindow().GetNativeWindow())
		, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		exit(0);
	
	m_CameraController.OnUpdate(ts);
	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::translate(trans, glm::vec3(sin(glfwGetTime()), cos(glfwGetTime()), 0.0f));
	trans = glm::translate(trans, glm::vec3(0.0f, 0.0f, 0.0f));
	trans = glm::rotate(trans, ((float)glfwGetTime()), glm::vec3(1.0f, 1.0f, 0.0f));

	glm::mat4 model = glm::mat4(1.0f);
	m_shader->setMat4("model", model);



	m_CameraController.GetCamera().setViewMatrix(glm::rotate(m_CameraController.GetCamera().GetViewMatrix(), glm::radians(static_cast<float>(xpos / 100)), glm::vec3(0.0f, 1.0f, 0.0f)));
	m_CameraController.GetCamera().setViewMatrix(glm::rotate(m_CameraController.GetCamera().GetViewMatrix(), glm::radians(static_cast<float>(ypos / 100)), glm::vec3(1.0f, 0.0f, 0.0f)));


	m_shader->setMat4("projection", m_CameraController.GetCamera().GetProjectionMatrix());
	glm::mat4 tabdil = m_CameraController.GetCamera().GetViewMatrix();
	m_shader->setMat4("view", tabdil);


	glClearColor(0.621f, 0.648f, 0.628f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//m_shader->setVec3("ourColor", glm::vec3{ 1.0f, 0.3f, 0.2f });
	//m_shader->setVec3("lightColor", glm::vec3{1.0f, 1.0f, 1.0f});
	trans = glm::mat4(1.0f);
	trans = glm::translate(trans, glm::vec3(6.5f, -5.75f, 19.0f));
	glm::vec3 cubePositions[] = {
	glm::vec3(2.0f, -1.5f, -7.0f)
	};
	m_shader->setVec3("light.position", cubePositions[0]);
	m_shader->setVec3("camera_pos", m_CameraController.GetCamera().GetPosition() - glm::vec3(-0.5f , 0.4f ,0.0f));


	m_shader->setVec3("light.ambient", 0.2f , 0.2f , 0.2f);
	m_shader->setVec3("light.diffuse", 0.5f , 0.5f , 0.5f);
	m_shader->setVec3("light.specular", 1.0f, 1.0f, 1.0f);
	m_shader->setVec3("material.specular", 0.5f, 0.5f, 0.5f);
	m_shader->setFloat("material.shineiness", 32.0f);


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
	glBindTexture(GL_TEXTURE_2D, m_textures[0]);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	



	model = glm::mat4(1.0f);
	model = glm::translate(model, -cubePositions[0]);

	glBindTexture(GL_TEXTURE_2D, m_textures[1]);
	glUseProgram(m_light_shader->GetRendererID());
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glBindVertexArray(lightVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	m_light_shader->setMat4("model", model);
	m_light_shader->setMat4("projection", m_CameraController.GetCamera().GetProjectionMatrix());
	m_light_shader->setMat4("view", tabdil);
	

	glDrawArrays(GL_TRIANGLES, 0, 36);


}

void SandboxLayer::OnImGuiRender()
{
	// ImGui here
}
