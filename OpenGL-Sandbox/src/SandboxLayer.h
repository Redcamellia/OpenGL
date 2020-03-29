#pragma once

#include <GLCore.h>
#include <GLCoreUtils.h>
#include "Model.h"
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
	GLCore::Utils::PerspectiveCameraController m_CameraController;
	std::vector<unsigned int> m_textures;
	GLCore::Utils::Shader * m_shader;
	GLCore::Utils::Shader * m_light_shader;
	GLuint VAO; // vertex array object
	GLuint VBO; // vertex buffer object
	GLuint EBO; // element buffer object
	GLuint lightVAO;
	Model ourModel;
	Model anotherModel;
	Model grass;
	Model audiModel;
	void drawGrass(glm::vec3 position);
	void drawWall(glm::vec3 position);
};