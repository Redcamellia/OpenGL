#pragma once
#include <GLCore.h>
#include <GLCoreUtils.h>
#include "Mesh.h"
#include <assimp/include/assimp/Importer.hpp> // remember to make these include directories clear
#include <assimp/include/assimp/scene.h>	  // remember to make these include directories clear
#include <assimp/include/assimp/postprocess.h>// remember to make these include directories clear
#include <stb_image/stb_image.h>
class Model
{
public :
	Model(const char* path , bool gamma = false )
	{
		gamma_correction = gamma;
		loadModel(path);
	}
	Model() {}
	void draw(GLCore::Utils::Shader& arg_shader);
	std::vector<Mesh> getMeshes();
	std::vector<Texture>& getTextures();
private:
	std::vector<Mesh> meshes;
	std::string directory;
	std::vector<Texture> textures_loaded;
	bool gamma_correction;


	unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma);
	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);


};