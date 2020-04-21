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
	std::vector<unsigned int> m_textures;
	GLuint skyboxTexture;
	GLCore::Utils::Shader * m_shader;
	GLCore::Utils::Shader* m_normal_shader;
	GLuint VAO; // vertex array object
	GLuint VBO; // vertex buffer object
	GLuint EBO; // element buffer object
	Model rock;
	Model planet;
	glm::mat4* modelMatrices;

private :
	bool gamma = false;

	
};