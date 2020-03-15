#include "SandboxLayer.h"
#include "../glfw/include/GLFW/glfw3.h"
#include "stb_image/stb_image.h"
#include "../src/Platform/Windows/WindowsWindow.h"
using namespace GLCore;
using namespace GLCore::Utils;

double xpos = 0;
double ypos = 0;
double xoffset = 0;
double yoffset = 0;
bool first_mouse = true;

double prxpos = 0;
double prypos = 0;
void mouse_callback(GLFWwindow* window, double x, double y )
{
	if (first_mouse) {
		prxpos = x;
		prypos = y;
		first_mouse = false;
	
	}
	std::cout << x << "   " << y << std::endl;
	xoffset = x - prxpos;
	yoffset = y - prypos;

	prxpos = x;
	prypos = y;

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
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	glfwSetCursorPosCallback((GLFWwindow*)GLCore::Application::Get().GetWindow().GetNativeWindow()
		,mouse_callback);

	glfwSetInputMode((GLFWwindow*)GLCore::Application::Get().GetWindow().GetNativeWindow()
			, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
	glGenVertexArrays(1, &lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindVertexArray(lightVAO);

	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

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

	m_CameraController.OnUpdate(ts);
	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::translate(trans, glm::vec3(sin(glfwGetTime()), cos(glfwGetTime()), 0.0f));
	trans = glm::translate(trans, glm::vec3(0.0f, 0.0f, 0.0f));
	trans = glm::rotate(trans, ((float)glfwGetTime()), glm::vec3(1.0f, 1.0f, 0.0f));

	glm::mat4 model = glm::mat4(1.0f);
	m_shader->setMat4("model", model);

		m_CameraController.GetCamera().setCameraFront(glm::normalize(m_CameraController.GetCamera().GetCameraFront() + glm::vec3(xoffset / 6000, 0.0f, 0.0f)));
		m_CameraController.GetCamera().setCameraFront(glm::normalize(m_CameraController.GetCamera().GetCameraFront() + glm::vec3(0.0f, -yoffset / 6000, 0.0f)));


	m_shader->setMat4("projection", m_CameraController.GetCamera().GetProjectionMatrix());
	glm::mat4 tabdil = m_CameraController.GetCamera().GetViewMatrix();
	m_shader->setMat4("view", tabdil);


	glClearColor(0.621f, 0.648f, 0.628f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	timeTracker += ts.GetSeconds();

	m_shader->setVec3("ourColor", glm::vec3{ 1.0f, 0.3f, 0.2f });
	m_shader->setVec3("lightColor", glm::vec3{1.0f, 1.0f, 1.0f});
	trans = glm::mat4(1.0f);
	trans = glm::translate(trans, glm::vec3(6.5f, -5.75f, 19.0f));
	glm::vec3 cubePositions[] = {
	glm::vec3(2.0f, -1.5f, -7.0f)
	};
	m_shader->setVec3("lightPos", cubePositions[0]);
	m_shader->setVec3("camera_pos", m_CameraController.GetCamera().GetPosition() /*- glm::vec3(-0.5f , 0.4f ,0.0f)*/);
	//
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindVertexArray(VAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glDrawArrays(GL_TRIANGLES, 0, 36);

	



	model = glm::mat4(1.0f);
	model = glm::translate(model, -cubePositions[0]);


	glUseProgram(m_light_shader->GetRendererID());
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glBindVertexArray(lightVAO);
	glEnableVertexAttribArray(0);


	m_light_shader->setMat4("model", model);
	m_light_shader->setMat4("projection", m_CameraController.GetCamera().GetProjectionMatrix());
	m_light_shader->setMat4("view", tabdil);
	

	glDrawArrays(GL_TRIANGLES, 0, 36);


}

void SandboxLayer::OnImGuiRender()
{
	// ImGui here
}
