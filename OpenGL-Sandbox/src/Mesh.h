#pragma once
#include <GLCore.h>
#include <GLCoreUtils.h>
#include <vector>
#include <string>


struct Texture
{
	unsigned int m_id;
	std::string type;
	std::string path;
};

struct Vertex 
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
};


class Mesh
{

public : 
	std::vector<Vertex> vertices;
	std::vector<Texture> textures;
	std::vector<unsigned int> indices;

	Mesh(std::vector<Vertex> arg_vertices,
		std::vector<unsigned int> arg_indices,
		std::vector<Texture> arg_textures);
	void draw(GLCore::Utils::Shader arg_shader);
	GLuint VAO;

private :
	GLuint  VBO, EBO;
	void setupMesh();

};

