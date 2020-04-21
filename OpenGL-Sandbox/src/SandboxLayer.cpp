#include "SandboxLayer.h"
#include "../glfw/include/GLFW/glfw3.h"
#include "stb_image/stb_image.h"
#include "../src/Platform/Windows/WindowsWindow.h"

using namespace GLCore;
using namespace GLCore::Utils;

unsigned int loadTexture(const char* path);
unsigned int loadSkyBox();



namespace cameraGlobal {
	float lastX = 0;
	float lastY = 0;
	bool firstMouse = true;

}

GLCore::Utils::PerspectiveCameraController g_CameraController(glm::vec3(0.0f, 0.0f, 3.0f),
	glm::vec3(0.0f, 1.0f, 0.0f), -90.0f ,0.0f);
using namespace cameraGlobal;
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}



	float xoffset = xpos - lastX;
	float yoffset = ypos - lastY ;

	lastX = xpos;
	lastY = ypos;
	g_CameraController.MouseProcess(xoffset, yoffset);
}

SandboxLayer::SandboxLayer()	
{
	
}

SandboxLayer::~SandboxLayer()
{
}

void SandboxLayer::OnAttach()
{
	EnableGLDebugging();


	m_shader = Shader::FromGLSLTextFiles("assets/shaders/vertex.vert", "assets/shaders/frag.frag");

	glfwSetCursorPosCallback((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow(),
		mouse_callback);
	glfwSetInputMode((GLFWwindow*)GLCore::Application::Get().GetWindow().GetNativeWindow()
			, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	float position[] = { 
		// positions            // normals         // texcoords
		 20.0f, -0.5f,  20.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
		-20.0f, -0.5f,  20.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
		-20.0f, -0.5f, -20.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,

		 20.0f, -0.5f,  20.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
		-20.0f, -0.5f, -20.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,
		 20.0f, -0.5f, -20.0f,  0.0f, 1.0f, 0.0f,  10.0f, 10.0f
	}; 

	m_textures.push_back(loadTexture("assets/textures/wood.png"));

	glUseProgram(m_shader->GetRendererID());
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(position), position, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glBindVertexArray(0);
	glEnable(GL_MULTISAMPLE);


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

	g_CameraController.OnUpdate(ts);



	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	glUseProgram(m_shader->GetRendererID());

	glBindTexture(GL_TEXTURE_2D, m_textures[0]);

	m_shader->setInt("floorTexture", 0);
	m_shader->setVec3("lightPos", glm::vec3(0.0f,1.0f,0.0f));
	m_shader->setMat4("model", glm::mat4(1.0f));
	m_shader->setMat4("view", g_CameraController.GetCamera().GetViewMatrix());
	m_shader->setMat4("projection", g_CameraController.GetCamera().GetProjectionMatrix());
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);




}

void SandboxLayer::OnImGuiRender()
{
	// ImGui here
}

unsigned int loadTexture(const char * path)
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
unsigned int loadSkyBox()
{

	std::vector<std::string> faces
	{
		"assets/textures/skybox/right.jpg",
		"assets/textures/skybox/left.jpg",
		"assets/textures/skybox/top.jpg",
		"assets/textures/skybox/bottom.jpg",
		"assets/textures/skybox/back.jpg",
		"assets/textures/skybox/front.jpg"
	};

	unsigned int textureID;

	glGenTextures(1, &textureID);


	int width, height, nrChannels;
	for (GLuint i = 0; i < faces.size(); i++) 
	{
		unsigned char * data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

			stbi_image_free(data);
		}

		else
		{
			std::cout << "Cubemap texture failed to load a texture from cube map set from : " << faces[i] << std::endl;
			stbi_image_free(data);
		}



	}
	
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	return textureID;
}