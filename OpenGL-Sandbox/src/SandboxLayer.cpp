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
	-0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,

	-0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,

	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,

	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,

	-0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f, -0.5f,

	-0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	int width, height, nrChannel;
	unsigned char * data = stbi_load("wallTexture.jpg", &width, &height, &nrChannel,0);
	GLuint texture;
	glGenTextures(1, &texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glfwSetCursorPosCallback((GLFWwindow*)GLCore::Application::Get().GetWindow().GetNativeWindow()
		,mouse_callback);

	glfwSetInputMode((GLFWwindow*)GLCore::Application::Get().GetWindow().GetNativeWindow()
			, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
	glGenVertexArrays(1, &lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindVertexArray(lightVAO);

	// we only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need (it's already bound, but we do it again for educational purposes)


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

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
	trans = glm::translate(trans, glm::vec3(5.0f, 0.0f, 1.5f));
	trans = glm::rotate(trans,  ((float)glfwGetTime()), glm::vec3(1.0f, 1.0f, 0.0f));
	m_shader->setMat4("tansform", trans);

	glm::mat4 model = glm::mat4(1.0f) ;
	model = glm::rotate(model, -35.0f, glm::vec3(1.0, 0.0, 0.0));
	m_shader->setMat4("model", model);

	

	m_CameraController.GetCamera().setViewMatrix(glm::rotate(m_CameraController.GetCamera().GetViewMatrix(), glm::radians(static_cast<float>(xpos/100)), glm::vec3(0.0f, 1.0f, 0.0f)));
	m_CameraController.GetCamera().setViewMatrix(glm::rotate(m_CameraController.GetCamera().GetViewMatrix(), glm::radians(static_cast<float>(ypos/100)), glm::vec3(1.0f, 0.0f, 0.0f)));
	//std::cout << "logging x coord : " << xpos << "  logging y coord : " << ypos << std::endl;
		

	m_shader->setMat4("projection", m_CameraController.GetCamera().GetProjectionMatrix());
	glm::mat4 tabdil = m_CameraController.GetCamera().GetViewMatrix();
	tabdil = glm::rotate(tabdil, glm::radians(-0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	m_shader->setMat4("view", tabdil);


	//glClearColor(0.621f, 0.648f, 0.628f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	timeTracker += ts.GetSeconds();

	m_shader->setVec4("ourColor", m_SquareColor);

	//
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glBindVertexArray(VAO);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glm::vec3 cubePositions[] = {
		//glm::vec3(2.0f, 5.0f, -15.0f),
		//glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(-1.7f, 3.0f, -7.5f)
								};



	for(unsigned int i = 0; i < 1; i++)
	{
		model = glm::mat4(1.0f);
		model = glm::translate(model, cubePositions[i]);
		float angle = 20.0f * i;
		model = glm::rotate(model, glm::radians(-angle), glm::vec3(1.0f, 0.0f, 0.0f));
		trans = glm::mat4(1.0f);
		trans = glm::translate(trans , glm::vec3(5.0f, 0.0f, 15.0f));
		m_shader->setMat4("transform", trans);
		m_shader->setMat4("model", model);
		m_shader->setVec4("ourColor", glm::vec4(0.0f, 0.7f, 0.4f, 1.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}


	glUseProgram(m_light_shader->GetRendererID());
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glBindVertexArray(lightVAO);
	glEnableVertexAttribArray(0);

	m_light_shader->setMat4("model", model);
	m_light_shader->setMat4("projection", m_CameraController.GetCamera().GetProjectionMatrix());
	m_light_shader->setMat4("view", tabdil);
	trans = glm::mat4(1.0f);
	trans = glm::translate(trans, glm::vec3(-0.5f, 0.75f, 0.0f));
	m_light_shader->setMat4("transform", trans);

	glDrawArrays(GL_TRIANGLES, 0, 36);


}

void SandboxLayer::OnImGuiRender()
{
	// ImGui here
}
