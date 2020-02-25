#include "SandboxLayer.h"
#include "../glfw/include/GLFW/glfw3.h"
#include "stb_image/stb_image.h"
using namespace GLCore;
using namespace GLCore::Utils;

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
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

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
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	unsigned int indices[] = {
		  0 , 1 , 3
		, 1 , 2 , 3
	};

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);



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

	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::translate(trans, glm::vec3(sin(glfwGetTime()/2), cos(glfwGetTime()/2), 0.0f));
	trans = glm::rotate(trans,  ((float)glfwGetTime()), glm::vec3(1.0f, 1.0f, 0.0f));


	glm::mat4 model = glm::mat4(1.0f) ;
	model = glm::rotate(model, -35.0f, glm::vec3(1.0, 0.0, 0.0));
	unsigned int modelMatrix = glGetUniformLocation(m_shader->GetRendererID(), "model");
	glUniformMatrix4fv(modelMatrix, 1, GL_FALSE, glm::value_ptr(model));


	glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0, 0.0, -3.0f));
	unsigned int viewMatrix = glGetUniformLocation(m_shader->GetRendererID(), "view");
	glUniformMatrix4fv(viewMatrix, 1, GL_FALSE, glm::value_ptr(view));

	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(75.0f),(float)(1600 / 1200), 0.1f, 10.0f);
	unsigned int projMatrix = glGetUniformLocation(m_shader->GetRendererID(), "projection");
	glUniformMatrix4fv(projMatrix, 1, GL_FALSE, glm::value_ptr(projection));


	unsigned int transformLoc = glGetUniformLocation(m_shader->GetRendererID(), "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

	glClearColor(0.0f, 0.9f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		

	timeTracker += ts.GetSeconds();

	int vertexColorLocation = glGetUniformLocation(m_shader->GetRendererID(), "ourColor");
	glUniform4fv(vertexColorLocation,1 , glm::value_ptr(m_SquareColor));


	float vertices[] = {
				 0.5f , 0.5f, 0.0f , 1.0 , 1.0 ,// bottom right
			 	 0.5f ,-0.5f, 0.0f , 1.0 , 0.0 , // bottom left
				-0.5f ,-0.5f, 0.0f , 0.0f, 0.0 ,
				-0.5f , 0.5f, 0.0f , 0.0f, 1.0f};
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


	trans = glm::mat4(1.0f);
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


	//trans = glm::mat4(1.0f);
	//trans = glm::translate(trans, glm::vec3(0.0f, 0.5f, 0.0f));
	//glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
	//glDrawElements(GL_TRIANGLES, 6 , GL_UNSIGNED_INT , 0 );
}

void SandboxLayer::OnImGuiRender()
{
	// ImGui here
}
