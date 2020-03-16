#pragma once

#include <GLCore.h>
#include <GLCoreUtils.h>

class SandboxLayer : public GLCore::Layer
{
public:
	SandboxLayer();
	virtual ~SandboxLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnEvent(GLCore::Event& event) override;
	virtual void OnUpdate(GLCore::Timestep ts) override;
	virtual void OnImGuiRender() override;
private:
	GLCore::Utils::OrthographicCameraController m_CameraController;
	glm::vec4 m_SquareBaseColor = { 1.0f, 0.3f, 0.2f, 1.0f };
	glm::vec4 m_SquareAlternateColor = { 0.2f, 0.3f, 0.8f, 1.0f };
	glm::vec4 m_SquareColor = m_SquareBaseColor;
	std::vector<unsigned int > m_textures;
	GLCore::Utils::Shader * m_shader;
	GLCore::Utils::Shader* m_light_shader;
	GLuint VAO; // vertex array object
	GLuint VBO; // vertex buffer object
	GLuint EBO; // element buffer object
	GLuint lightVAO;
};