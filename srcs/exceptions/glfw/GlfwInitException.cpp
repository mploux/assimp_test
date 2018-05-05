//
// Created by Marc on 11/04/2018.
//

#include "GlfwInitException.hpp"

GlfwInitException::GlfwInitException()
	: GlfwException()
{
	setName("GlfwInitException");
	setDescription("Unable to initialise GLFW");
}

GlfwInitException::~GlfwInitException() throw() {}