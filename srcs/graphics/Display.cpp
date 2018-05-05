//
// Created by Marc on 11/04/2018.
//

#include <iostream>

#include "Display.hpp"
#include "Core.hpp"

#include "exceptions/glfw/GlfwInitException.hpp"
#include "exceptions/glfw/GlfwWindowCreationException.hpp"
#include "exceptions/glew/GlewInitException.hpp"

Display::Display()
	: m_title(""), m_width(0), m_height(0)
{
}

Display::Display(const std::string &title, const int &width, const int &height)
	: m_title(title), m_width(width), m_height(height)
{
	try
	{
		if (!glfwInit())
			throw GlfwInitException();

//		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
//		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

		m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);
		if (!m_window)
			throw GlfwWindowCreationException();

		glfwMakeContextCurrent(m_window);
		glViewport(0, 0, m_width, m_height);

		glewExperimental = GL_TRUE;

		if (glewInit() != GLEW_OK)
			throw GlewInitException();

		glfwSwapInterval(1);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		glEnable(GL_ALPHA_TEST);
		glEnable (GL_BLEND);
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		float lineWidth[2];
		glGetFloatv(GL_LINE_WIDTH_RANGE, lineWidth);
		std::cout << lineWidth[0] << "   LOL   " << lineWidth[1] << std::endl;
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		Core::getInstance().terminate();
	}
}

Display::~Display()
{
	glfwTerminate();
}

bool Display::closeRequested()
{
	return glfwWindowShouldClose(m_window) ? true : false;
}

void Display::update()
{
	glfwSwapBuffers(m_window);
	glfwPollEvents();
}

GLFWwindow *Display::getWindow() const
{
	return m_window;
}
