//
// Created by Marc on 11/04/2018.
//

#pragma once

#include <string>
#include <GL/glew.h>
#include <maths/Mat4.hpp>

class Shader
{
private:
	GLuint	m_program;

	GLuint	createProgram(const char *vertex_source, const char *geometrie_source, const char *fragment_source);
	GLuint	createShader(const char *source, int shader);

	GLint 	getUniformLocation(const char *name);

public:
	Shader(const std::string &vertex_path, const std::string &fragment_path);
	Shader(const std::string &vertex_path, const std::string &geometrie_path, const std::string &fragment_path);
	~Shader();

	void setUniform(const char *name, GLint v);
	void setUniform(const char *name, GLfloat v);
	void setUniform(const char *name, Vec3<float> v);
	void setUniform(const char *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
	void setUniform(const char *name, Mat4<float> m);

	inline void bind() { glUseProgram(m_program); };
	inline void unbind() { glUseProgram(0); };

	inline int getProgram() { return (m_program); }
};

