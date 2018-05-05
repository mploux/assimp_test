//
// Created by Marc on 11/04/2018.
//

#include "Shader.hpp"

#include <iostream>
#include <vector>

#include "Core.hpp"
#include "utils/FileUtils.hpp"
#include "exceptions/shaders/ShaderCreationException.hpp"
#include "exceptions/shaders/ShaderProgramCreationException.hpp"

Shader::Shader(const std::string &vertex_path, const std::string &fragment_path)
{
	try
	{
		std::string vertex_source = utils::loadFile(vertex_path);
		std::string fragment_source = utils::loadFile(fragment_path);
		m_program = createProgram(vertex_source.c_str(), nullptr, fragment_source.c_str());
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		Core::getInstance().terminate();
	}
}

Shader::Shader(const std::string &vertex_path, const std::string &geometry_path, const std::string &fragment_path)
{
	try
	{
		std::string vertex_source = utils::loadFile(vertex_path);
		std::string geometry_source = utils::loadFile(geometry_path);
		std::string fragment_source = utils::loadFile(fragment_path);
		m_program = createProgram(vertex_source.c_str(), geometry_source.c_str(), fragment_source.c_str());
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		Core::getInstance().terminate();
	}
}

Shader::~Shader()
{
	glDeleteProgram(m_program);
}

GLuint Shader::createProgram(const char *vertex_source, const char *geometry_source, const char *fragment_source)
{
	GLuint program = glCreateProgram();

	if (program == GL_FALSE)
		throw ShaderProgramCreationException();

	GLuint vertex_shader = createShader(vertex_source, GL_VERTEX_SHADER);
	glAttachShader(program, vertex_shader);
	GLuint geometry_shader = 0;
	if (geometry_source)
	{
		geometry_shader = createShader(geometry_source, GL_GEOMETRY_SHADER);
		glAttachShader(program, geometry_shader);
	}
	GLuint fragment_shader = createShader(fragment_source, GL_FRAGMENT_SHADER);
	glAttachShader(program, fragment_shader);

	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vertex_shader);
	if (geometry_source)
		glDeleteShader(geometry_shader);
	glDeleteShader(fragment_shader);

	return (program);
}

GLint Shader::getUniformLocation(const char *name)
{
	return glGetUniformLocation(m_program, name);
}

void Shader::setUniform(const char *name, GLint v)
{
	glUniform1i(getUniformLocation(name), v);
}

void Shader::setUniform(const char *name, GLfloat v)
{
	glUniform1f(getUniformLocation(name), v);
}

void Shader::setUniform(const char *name, Vec3<float> v)
{
	glUniform3f(getUniformLocation(name), v.getX(), v.getY(), v.getZ());
}

void Shader::setUniform(const char *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
	glUniform4f(getUniformLocation(name), x, y, z, w);
}


void Shader::setUniform(const char *name, Mat4<float> m)
{
	glUniformMatrix4fv(getUniformLocation(name), 1, false, m.getData());
}

GLuint Shader::createShader(const char *source, int type)
{
	GLuint shader;
	GLint result;

	shader = glCreateShader(type);
	if (shader == GL_FALSE)
		throw ShaderCreationException();

	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		GLint length;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
		std::vector<char> error(length);
		glGetShaderInfoLog(shader, length, &length, &error[0]);
		std::cout << &error[0] << std::endl;
		glDeleteShader(shader);
		return (0);
	}
	return (shader);
}
