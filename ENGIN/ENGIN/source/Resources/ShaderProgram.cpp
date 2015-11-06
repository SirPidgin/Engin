#include "Engin\Resources\ShaderProgram.h"

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

namespace Engin
{
	namespace Resources
	{
		void ShaderProgram::load(const GLchar* vertexPath, const GLchar* fragmentPath)
		{
			std::string vertexTemp;
			std::string fragmentTemp;

			std::ifstream vertexFile;
			std::ifstream fragmentFile;

			vertexFile.exceptions(std::ifstream::badbit);
			fragmentFile.exceptions(std::ifstream::badbit);

			try
			{
				vertexFile.open(vertexPath);
				fragmentFile.open(fragmentPath);

				std::stringstream vertexStream;
				std::stringstream fragmentStream;

				vertexStream << vertexFile.rdbuf();
				fragmentStream << fragmentFile.rdbuf();

				vertexFile.close();
				fragmentFile.close();

				vertexTemp = vertexStream.str();
				fragmentTemp = fragmentStream.str();
			}
			catch (std::ifstream::failure error)
			{
				std::cout << "ERROR: Reading shader " << std::endl;
			}

			const GLchar* vertexCode = vertexTemp.c_str();
			const GLchar* fragmentCode = fragmentTemp.c_str();

			GLuint vertex;
			GLuint fragment;
			GLint success;
			GLchar infoLog[512];

			vertex = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vertex, 1, &vertexCode, nullptr);
			glCompileShader(vertex);

			glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);

			if (!success)
			{
				glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
				std::cout << "ERROR: Shader compilation: " << std::endl << infoLog << std::endl;
			}

			fragment = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fragment, 1, &fragmentCode, nullptr);
			glCompileShader(fragment);

			glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);

			if (!success)
			{
				glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
				std::cout << "ERROR: Shader compilation: " << std::endl << infoLog << std::endl;
			}

			program = glCreateProgram();
			glAttachShader(program, vertex);
			glAttachShader(program, fragment);
			glLinkProgram(program);

			glGetProgramiv(program, GL_LINK_STATUS, &success);

			if (!success)
			{
				glGetProgramInfoLog(program, 512, nullptr, infoLog);
				std::cout << "ERROR: Program linking: " << std::endl << infoLog << std::endl;
			}

			glDeleteShader(vertex);
			glDeleteShader(fragment);
		}

		ShaderProgram::~ShaderProgram()
		{
			glDeleteProgram(program);
		}

		void ShaderProgram::bind()
		{
			glUseProgram(program);
		}

		void ShaderProgram::unbind()
		{
			glUseProgram(0);
		}
	}
}
