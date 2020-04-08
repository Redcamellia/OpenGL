#include "glpch.h"
#include "Shader.h"
#include "stb_image.h"
#include <fstream>

namespace GLCore::Utils {

	static std::string ReadFileAsString(const std::string& filepath)
	{
		std::string result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			result.resize((size_t)in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();
		}
		else
		{
			LOG_ERROR("Could not open file '{0}'", filepath);
		}

		return result;
	}

	Shader::~Shader()
	{
		/*glDeleteProgram(m_RendererID);*/
	}

	unsigned int Shader::loadTexture(const char* path)
	{
		unsigned int textureID;
		glGenTextures(1, &textureID);

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

	GLuint Shader::CompileShader(GLenum type, const std::string& source)
	{
		GLuint shader = glCreateShader(type);

		const GLchar* sourceCStr = source.c_str();
		glShaderSource(shader, 1, &sourceCStr, 0);

		glCompileShader(shader);

		GLint isCompiled = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

			glDeleteShader(shader);

			LOG_ERROR("{0}", infoLog.data());
			// HZ_CORE_ASSERT(false, "Shader compilation failure!");
		}

		return shader;
	}

	Shader* Shader::FromGLSLTextFiles(const std::string& vertexShaderPath, const std::string& fragmentShaderPath 
		, const std::string & geometryShaderPath )
	{
		Shader* shader = new Shader();
		shader->LoadFromGLSLTextFiles(vertexShaderPath, fragmentShaderPath , geometryShaderPath);
		return shader;
	}
	
	void Shader::LoadFromGLSLTextFiles(const std::string& vertexShaderPath, const std::string& fragmentShaderPath 
		, const std::string& geometryShaderPath )
	{
		std::string vertexSource = ReadFileAsString(vertexShaderPath);
		std::string fragmentSource = ReadFileAsString(fragmentShaderPath);
		std::string geometrySource;
		if(geometryShaderPath != "no")
		geometrySource = ReadFileAsString(geometryShaderPath);

		GLuint program = glCreateProgram();
		int glShaderIDIndex = 0;
			
		GLuint vertexShader = CompileShader(GL_VERTEX_SHADER, vertexSource);
		glAttachShader(program, vertexShader);
		if (geometryShaderPath != "no")
		{
			GLuint geometryShader = CompileShader(GL_GEOMETRY_SHADER, geometrySource);
			glAttachShader(program, geometryShader);
		}
		GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);
		glAttachShader(program, fragmentShader);

		glLinkProgram(program);

		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			glDeleteProgram(program);

			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			LOG_ERROR("{0}", infoLog.data());
			// HZ_CORE_ASSERT(false, "Shader link failure!");
		}
		
		glDetachShader(program, vertexShader);
		glDetachShader(program, fragmentShader);
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		m_RendererID = program;
	}

}