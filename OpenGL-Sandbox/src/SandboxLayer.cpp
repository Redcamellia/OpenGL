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
	: m_CameraController(glm::vec3(0.0f, 0.0f, 5.0f),
		glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f) 
	
{
	
}

SandboxLayer::~SandboxLayer()
{
}

void SandboxLayer::OnAttach()
{
	EnableGLDebugging();

	m_shader = Shader::FromGLSLTextFiles("assets/shaders/vertex.vert", "assets/shaders/frag.frag");
	glUseProgram(m_shader->GetRendererID());
	ourModel = Model("assets/nanosuit/nanosuit.obj");
	//audiModel = Model("assets/textures/porsche 911 GT/Porsche_911_GT2.obj");
	anotherModel = Model("assets/textures/wall/wall.obj");
	grass = Model("assets/textures/grass/grass1/10450_Rectangular_Grass_Patch_v1_iterations-2.obj");
	audiModel = Model("assets/textures/3627_open3dmodel/NFSHP2 - Porsche Carrera GT/carrgt.obj");
	glfwSetCursorPosCallback((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow(), mouse_callback);

	

	glfwSetInputMode((GLFWwindow*)GLCore::Application::Get().GetWindow().GetNativeWindow()
			, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	glEnable(GL_DEPTH_TEST);

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

	EventDispatcher dispatcher(event);
	

}

void SandboxLayer::OnUpdate(Timestep ts)
{

	glUseProgram(m_shader->GetRendererID());

	m_CameraController.OnUpdate(ts);
	m_CameraController.MouseProcess(cameraGlobal::xoffset, cameraGlobal::yoffset);
	glm::mat4 model = glm::mat4(1.0f);

	glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 view = m_CameraController.GetCamera().GetViewMatrix();
	glm::mat4 projection = m_CameraController.GetCamera().GetProjectionMatrix();
	m_shader->setMat4("view", view);
	m_shader->setMat4("projection", projection);

	
	model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
	model = glm::scale(model, glm::vec3(0.09f, 0.09f, 0.09f));	// it's a bit too big for our scene, so scale it down
	m_shader->setMat4("model", model);

	ourModel.draw(*m_shader);

	drawWall(glm::vec3(0.0f, 0.0f, -1.0f));
	drawWall(glm::vec3(3.0f, 0.0f, -1.0f));
	drawWall(glm::vec3(6.0f, 0.0f, -1.0f));
	drawWall(glm::vec3(-3.0f, 0.0f, -1.0f));
	drawWall(glm::vec3(0.0f, -1.5f, -1.0f));
	drawWall(glm::vec3(3.0f, -1.5f, -1.0f));
	drawWall(glm::vec3(6.0f, -1.5f, -1.0f));
	drawWall(glm::vec3(-3.0f, -1.5f, -1.0f));
	

	m_shader->setMat4("view", view);
	m_shader->setMat4("projection", projection);


	model = glm::translate(model, glm::vec3(4.0f, 3.40f, 30.0f));
	model = glm::scale(model, glm::vec3(7.0f, 7.0f, 7.0f));
	model = glm::rotate(model, glm::radians(37.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	m_shader->setMat4("model", model);
	audiModel.draw(*m_shader);


	drawGrass(glm::vec3(0.0f, 0.0f, 0.0f));
	drawGrass(glm::vec3(3.0f, 0.0f, 0.0f));
	drawGrass(glm::vec3(-3.0f, 0.0f, 0.0f));
	drawGrass(glm::vec3(6.0f, 0.0f, 0.0f));
	drawGrass(glm::vec3(0.0f, 0.0f, 3.0f));
	drawGrass(glm::vec3(3.0f, 0.0f, 3.0f));
	drawGrass(glm::vec3(-3.0f, 0.0f, 3.0f));
	drawGrass(glm::vec3(6.0f, 0.0f, 3.0f));
	drawGrass(glm::vec3(0.0f, 0.0f, -3.0f));
	drawGrass(glm::vec3(3.0f, 0.0f, -3.0f));
	drawGrass(glm::vec3(-3.0f, 0.0f, -3.0f));
	drawGrass(glm::vec3(6.0f, 0.0f, -3.0f));


}

void SandboxLayer::OnImGuiRender()
{
	// ImGui here
}

void SandboxLayer::drawGrass(glm::vec3 position)
{
	glm::mat4 view(1.0f);
	glm::mat4 projection(1.0f);
	glm::mat4 model(1.0f);

	view = m_CameraController.GetCamera().GetViewMatrix();
	projection = m_CameraController.GetCamera().GetProjectionMatrix();
	m_shader->setMat4("view", view);
	m_shader->setMat4("projection", projection);
	model = glm::translate(model, position);
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.01, 0.01, 0.01));
	model = glm::translate(model, glm::vec3(0.0f, -0.5f, -179.75f));
	m_shader->setMat4("model", model);
	grass.draw(*m_shader);
}

void SandboxLayer::drawWall(glm::vec3 position)
{
	glm::mat4 view(1.0f);
	glm::mat4 projection(1.0f);
	glm::mat4 model(1.0f);

	view = m_CameraController.GetCamera().GetViewMatrix();
	projection = m_CameraController.GetCamera().GetProjectionMatrix();
	m_shader->setMat4("view", view);
	m_shader->setMat4("projection", projection);
	model = glm::scale(model, glm::vec3(0.5f));
	model = glm::translate(model, position);
	m_shader->setMat4("model", model);
	anotherModel.draw(*m_shader);
}
