//
// Created by Marc on 11/04/2018.
//

#pragma once

#include <iostream>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>


class Display
{
private:
	GLFWwindow	*m_window;

	std::string	m_title;
	int			m_width;
	int			m_height;

	Display();

public:
	Display(const std::string &title, const int &width, const int &height);
	virtual ~Display();

	bool closeRequested();
	void update();

	GLFWwindow *getWindow() const;
};
