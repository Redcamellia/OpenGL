#pragma once

#include <string>
#include <GLCore.h>
#include <glad/glad.h>

namespace GLCore::Utils {

	class Shader
	{
	public:
		~Shader();

		GLuint GetRendererID() { return m_RendererID; }

		static Shader* FromGLSLTextFiles(const std::string& vertexShaderPath, const std::string& fragmentShaderPath 
			, const std::string & geometryShaderPath = "no");



		unsigned int loadTexture(char const* path);

		void setBool(const std::string& name, bool value) const
		{
			glUniform1i(glGetUniformLocation(m_RendererID, name.c_str()), (int)value);
		}
		// ------------------------------------------------------------------------
		void setInt(const std::string& name, int value) const
		{
			glUniform1i(glGetUniformLocation(m_RendererID, name.c_str()), value);
		}
		// ------------------------------------------------------------------------
		void setFloat(const std::string& name, float value) const
		{
			glUniform1f(glGetUniformLocation(m_RendererID, name.c_str()), value);
		}
		// ------------------------------------------------------------------------
		void setVec2(const std::string& name, const glm::vec2& value) const
		{
			glUniform2fv(glGetUniformLocation(m_RendererID, name.c_str()), 1, &value[0]);
		}
		void setVec2(const std::string& name, float x, float y) const
		{
			glUniform2f(glGetUniformLocation(m_RendererID, name.c_str()), x, y);
		}
		// ------------------------------------------------------------------------
		void setVec3(const std::string& name, const glm::vec3& value) const
		{
			glUniform3fv(glGetUniformLocation(m_RendererID, name.c_str()), 1, &value[0]);
		}
		void setVec3(const std::string& name, float x, float y, float z) const
		{
			glUniform3f(glGetUniformLocation(m_RendererID, name.c_str()), x, y, z);
		}
		// ------------------------------------------------------------------------
		void setVec4(const std::string& name, const glm::vec4& value) const
		{
			glUniform4fv(glGetUniformLocation(m_RendererID, name.c_str()), 1, &value[0]);
		}
		void setVec4(const std::string& name, float x, float y, float z, float w)
		{
			glUniform4f(glGetUniformLocation(m_RendererID, name.c_str()), x, y, z, w);
		}
		// ------------------------------------------------------------------------
		void setMat2(const std::string& name, const glm::mat2& mat) const
		{
			glUniformMatrix2fv(glGetUniformLocation(m_RendererID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
		}
		// ------------------------------------------------------------------------
		void setMat3(const std::string& name, const glm::mat3& mat) const
		{
			glUniformMatrix3fv(glGetUniformLocation(m_RendererID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
		}
		// ------------------------------------------------------------------------
		void setMat4(const std::string& name, const glm::mat4& mat) const
		{
			glUniformMatrix4fv(glGetUniformLocation(m_RendererID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
		}
	private:
		Shader() = default;

		void LoadFromGLSLTextFiles(const std::string& vertexShaderPath, const std::string& fragmentShaderPath 
			, const std::string & geometryShaderPath = "no");
		GLuint CompileShader(GLenum type, const std::string& source);
	private:
		GLuint m_RendererID;
	};

}