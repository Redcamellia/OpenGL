#include "SandboxLayer.h"
#include "../glfw/include/GLFW/glfw3.h"
#include "stb_image/stb_image.h"
#include "../src/Platform/Windows/WindowsWindow.h"

using namespace GLCore;
using namespace GLCore::Utils;

unsigned int loadTexture(char const* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	std::cout << "loading texture from : " << path << std::endl;

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}


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
	m_light_shader = Shader::FromGLSLTextFiles("assets/shaders/lightVertex.vert", "assets/shaders/lightFragment.frag");
	glUseProgram(m_shader->GetRendererID());

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindVertexArray(0);
	// plane VAO

	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindVertexArray(0);

	float grassPositions[] =
	{
		 0.5f , 0.5f  , 0.0f , 1.0f , 1.0f ,
		-0.5f , -0.5f , 0.0f , 0.0f , 0.0f ,
		 0.5f , -0.5f , 0.0f , 1.0f , 0.0f ,
		 0.5f , 0.5f  , 0.0f , 1.0f , 1.0f ,
		-0.5f , -0.5f , 0.0f , 0.0f , 0.0f ,
		-0.5f , 0.5f  , 0.0f , 0.0f , 1.0f
	};

	glGenVertexArrays(1, &grassVAO);
	glGenBuffers(1, &grassVBO);
	glBindVertexArray(grassVAO);
	glBindBuffer(GL_ARRAY_BUFFER, grassVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(grassPositions), &grassPositions, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	m_textures.push_back(loadTexture("assets/textures/marble.jpg"));
	m_textures.push_back(loadTexture("assets/textures/metal.png"));
	m_textures.push_back(loadTexture("assets/textures/blending_transparent_window.png"));

	m_shader->setInt("texture1", 0);
	glfwSetCursorPosCallback((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow(), mouse_callback);
	glfwSetInputMode((GLFWwindow*)GLCore::Application::Get().GetWindow().GetNativeWindow()
			, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);



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
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glUseProgram(m_shader->GetRendererID());

	std::vector<glm::vec3> vegetation;
	vegetation.push_back(glm::vec3(-1.6f, 0.0f, -0.28f));
	vegetation.push_back(glm::vec3(1.7f, 0.0f, 0.81f));
	vegetation.push_back(glm::vec3(0.0f, 0.0f, 0.7f));
	vegetation.push_back(glm::vec3(-0.3f, 0.0f, -2.3f));
	vegetation.push_back(glm::vec3(0.5f, 0.0f, -0.6f));

	glm::mat4 view = m_CameraController.GetCamera().GetViewMatrix();
	glm::mat4 projection = m_CameraController.GetCamera().GetProjectionMatrix();
	glm::mat4 model = glm::mat4(1.0f);

	glBindVertexArray(grassVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_textures[2]);
	
	for (int i = 0; i < vegetation.size(); i++)
	{
		model = glm::mat4(1.0f);
		model = glm::translate(model, vegetation[i]);
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		m_shader->setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}



	glBindVertexArray(planeVAO);
	glBindTexture(GL_TEXTURE_2D, m_textures[1]);
	m_shader->setMat4("model", glm::mat4(1.0f));
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	m_CameraController.OnUpdate(ts);
	m_CameraController.MouseProcess(cameraGlobal::xoffset, cameraGlobal::yoffset);


	m_shader->setMat4("view", view);

	m_shader->setMat4("projection", projection);
	// cubes




	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_textures[0]);
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
	m_shader->setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
	m_shader->setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	// floor

	












}

void SandboxLayer::OnImGuiRender()
{
	// ImGui here
}

